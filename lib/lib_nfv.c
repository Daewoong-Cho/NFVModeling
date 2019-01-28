#include "nfv_header.h"
#include "/usr/include/mysql/mysql.h"

MYSQL MDB_session;

void initialize_MySQL()
{
	mysql_init(&MDB_session);
  if(!mysql_real_connect(&MDB_session, NULL, "root","dwjjang2", "nfv" ,3306, (char *)NULL, 0))
  {
  	printf("%s\n",mysql_error(&MDB_session));
    exit(1);
  }
  printf("Connection Succeed.\n") ;
}
VNF_TYPE *add_VNF_TYPE(int key, int req_cpu, int req_mem, int req_disk)
{
  VNF_TYPE *new_vnf_type;
  new_vnf_type = (VNF_TYPE *)malloc(sizeof(VNF_TYPE));

  new_vnf_type->key = ++(vnf_type_list->seq);
  new_vnf_type->req_cpu = req_cpu;
  new_vnf_type->req_mem = req_mem;
  new_vnf_type->req_disk = req_disk;

  if(vnf_type_list->head == NULL && vnf_type_list->tail == NULL)
  { 
    vnf_type_list->head = vnf_type_list->tail = new_vnf_type;
   }
   else
   {
    vnf_type_list->tail->next = new_vnf_type;
    vnf_type_list->tail = new_vnf_type;
   }
  return new_vnf_type;
}

VNF *deploy_VNF(int key, int throughput, VNF_TYPE *type, SERVER *server, QUEUE *in_queue, QUEUE *out_queue)
{
  VNF *new_vnf;
  new_vnf = (VNF *)malloc(sizeof(VNF));
  char cmd[256];

  if(server != NULL)
    if(check_Resource(type->req_cpu, type->req_mem, type->req_disk, server) < 0) return NULL;
  alloc_Resource(type->req_cpu, type->req_mem, type->req_disk, server);

  new_vnf->key = key; 

  new_vnf->processed_cnt = 0;
  new_vnf->throughput = throughput;
  new_vnf->avail = throughput;
  new_vnf->vnf_type = type;
  new_vnf->server = server;

  new_vnf->in_queue = in_queue;
  new_vnf->out_queue = out_queue;

  if(vnf_list->head == NULL && vnf_list->tail == NULL)
  { 
    vnf_list->head = vnf_list->tail = new_vnf;
   }
   else
   {
    vnf_list->tail->next = new_vnf;
    vnf_list->tail = new_vnf;
   }
  ++(vnf_list->seq);
  sprintf(cmd, "~/NFVModeling/bin/%s %d", "VNF", new_vnf->key);
  system(cmd);
  return new_vnf;
}

void revoke_VNF(VNF *vnf) //remove linked list & free resource
{
  free_Resource(vnf->server);
  vnf = vnf->next;
}

void log_VNFStatus(int time_stamp)
{
  char sqlStr[8192];
  VNF *vnf = vnf_list->head;
  while(vnf != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   VNF_LOG            "
      " (                    "
      "   TIME_STAMP        ,"
      "   VNF_KEY           ,"
      "   PROCESSED_CNT     ,"
      "   THROUGHPUT        ,"
      "   AVAIL             ,"
      "   VNF_USAGE         ,"
      "   SERVER_KEY        ,"
      "   VNF_TYPE_KEY      ,"
      "   IN_QUEUE_KEY      ,"
      "   OUT_QUEUE_KEY      "
      " )                    "
      " VALUES               "
      " (                    "
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %.2f              ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                 "
      " )                    "
      " ;                    ",
        time_stamp           ,
        vnf->key             ,
        vnf->processed_cnt   ,
        vnf->throughput      ,
        vnf->avail           ,
        vnf->vnf_usage       ,
        vnf->server->key     ,
        vnf->vnf_type->key   ,
        vnf->in_queue->key   ,
        vnf->out_queue->key       
      );
    if(mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
    {
      printf("VNF_LOG[%d] INSERT FAIL[%s][%s]\n", vnf->key, mysql_error(&MDB_session), sqlStr);
    }    
    vnf= vnf->next;
  }
}

SERVER *deploy_SERVER(int key, int alloc_cpu, int alloc_mem, int alloc_disk, SERVER *parent_server, char job_type)
{
  SERVER *new_server;
  new_server= (SERVER *)malloc(sizeof(SERVER));

  if(parent_server != NULL)
    if(check_Resource(alloc_cpu, alloc_mem, alloc_disk, parent_server) < 0) return NULL;

  alloc_Resource(alloc_cpu, alloc_mem, alloc_disk, parent_server);

  ++(server_list->seq);
  new_server->key = key;

  new_server->t_cpu = alloc_cpu;
  new_server->t_mem = alloc_mem;
  new_server->t_disk = alloc_disk;

  new_server->a_cpu = alloc_cpu;
  new_server->a_mem = alloc_mem;
  new_server->a_disk = alloc_disk;

  new_server->parent = parent_server;
  new_server->job_type = job_type;

  if(server_list->head == NULL && server_list->tail == NULL)
  { 
    server_list->head = server_list->tail = new_server;
    //server_list->head->next = server_list->tail->next = new_server;
   }
   else
   {
    server_list->tail->next = new_server;
    server_list->tail = new_server;
   }
  return new_server;
}

void revoke_SERVER(SERVER *server) //remove linked list & free resource
{
  free_Resource(server);
  server = server->next;
}

void free_Resource(SERVER *server)
{
  server->parent->a_cpu += server->t_cpu;
  server->parent->a_mem += server->t_mem;
  server->parent->a_disk += server->t_disk;
}

void log_ServerStatus(int time_stamp)
{
  char sqlStr[8192];
  SERVER *server = server_list->head;
  while(server != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   SERVER_LOG         "
      " (                    "
      "   TIME_STAMP        ,"
      "   SERVER_KEY        ,"
      "   T_CPU             ,"
      "   T_MEM             ,"
      "   T_DISK            ,"
      "   A_CPU             ,"
      "   A_MEM             ,"
      "   A_DISK            ,"
      "   CPU_USAGE         ,"
      "   MEM_USAGE         ,"
      "   DISK_USAGE        ,"
      "   P_SERVER_KEY      ,"
      "   JOB_TYPE           "
      " )                    "
      " VALUES               "
      " (                    "
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %.2f              ,"
      "   %.2f              ,"
      "   %.2f              ,"
      "   %d                ,"
      "   %c                 "
      " )                    "
      " ;                    ",
        time_stamp          ,
        server->key         ,
        server->t_cpu       ,
        server->t_mem       ,
        server->t_disk      ,
        server->a_cpu       ,
        server->a_mem       ,
        server->a_disk      ,
        server->cpu_usage   ,
        server->mem_usage   ,
        server->disk_usage  ,
        server->parent->key ,
        server->job_type
      );
    if(mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
    {
      printf("SERVER_LOG[%d] INSERT FAIL[%s][%s]\n", server->key, mysql_error(&MDB_session), sqlStr);
    }
    server = server->next;
  }
}

int check_Resource(int rcpu, int rmem, int rdisk, SERVER *parent_server)
{
  if((parent_server->a_cpu > rcpu) && 
     (parent_server->a_mem > rmem) && 
     (parent_server->a_disk > rdisk)) 
  {return -1;}
  else return 1;
}

int alloc_Resource(int rcpu, int rmem, int rdisk, SERVER *pm)
{
  pm->a_cpu -= rcpu;
  pm->a_mem -= rmem;
  pm->a_disk-= rdisk;
  
  calculate_server_usage(pm);
  return 1;
}

QUEUE *deploy_QUEUE(int key, int size, SERVER *server)
{
  QUEUE *new_queue;
  new_queue = (QUEUE *)malloc(sizeof(QUEUE));

  if(server == NULL) return NULL; 

  new_queue->key = key;

  new_queue->size = size;
  new_queue->avail = size;
  new_queue->processed_cnt = 0;
  new_queue->server = server;

  if(queue_list->head == NULL && queue_list->tail == NULL)
  { 
    queue_list->head = queue_list->tail = new_queue;
   }
   else
   {
    queue_list->tail->next = new_queue;
    queue_list->tail = new_queue;
   }
  ++(queue_list->seq);
  return new_queue;
}

void revoke_QUEUE(QUEUE *queue) //remove linked list & free resource
{
  //free_Resource(queue);
  queue = queue->next;
}

void log_QueueStatus(int time_stamp)
{
  char sqlStr[8192];
  QUEUE *queue = queue_list->head;
  while(queue != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   QUEUE_LOG         "
      " (                    "
      "   TIME_STAMP        ,"
      "   QUEUE_KEY         ,"
      "   SIZE              ,"
      "   AVAIL             ,"
      "   QUEUE_USAGE       ,"
      "   PROCESSED_CNT     ," //already processed packets
      "   WAIT_CNT          ," //waiting packets to be processed
      "   SERVER_KEY         "
      " )                    "
      " VALUES               "
      " (                    "
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %.2f              ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                 "
      " )                    "
      " ;                    ",
        time_stamp              ,
        queue->key              ,
        queue->size             ,
        queue->avail            ,
        queue->queue_usage      ,
        queue->processed_cnt    ,
        queue->packet_list->cnt ,
        queue->server->key       
      );
    if(mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
    {
      printf("QUEUE_LOG[%d] INSERT FAIL[%s][%s]\n", queue->key, mysql_error(&MDB_session), sqlStr);
    }
    
    queue = queue->next;
  }
}

void log_Packet(PACKET *packet, int queue_key)
{
  char sqlStr[8192];
  memset(sqlStr, 0x20, sizeof(sqlStr));
  sprintf(sqlStr, 
    " INSERT INTO          "
    "   PACKET_LOG         "
    " (                    "
    "   QUEUE_KEY         ,"
    "   PACKET_SEQ        ,"
    "   WAIT_TIME         ,"
    "   PROCESS_TIME      ,"
    "   SERVICE_TIME       "
    " )                    "
    " VALUES               "
    " (                    "
    "   %d                ,"
    "   %d                ,"
    "   %d                ,"
    "   %d                ,"
    "   %d                 "
    " )                    "
    " ;                    ",
      queue_key             ,
      packet->seq           ,
      packet->wait_time     ,
      packet->process_time  ,
      packet->service_time       
  );
  if(mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
  {
    printf("PACKET_LOG[%d] INSERT FAIL[%s][%s]\n", packet->seq, mysql_error(&MDB_session), sqlStr);
  }    
}

//After packet pop
//queue->packet_list->head = queue->packet_list->head->next;

PACKET *getPacket(QUEUE *in_queue, VNF *vnf)
{
  return popQueue(in_queue, vnf);
}

PACKET *popQueue(QUEUE *queue, VNF *vnf)
{
  PACKET *current_packet;
  current_packet = queue->packet_list->head;

  //vnf doesn't have space
  if(vnf->avail - strlen(current_packet->data) < 0) return NULL;
  
  queue->avail += strlen(current_packet->data);
  queue->processed_cnt++;
  calculate_queue_usage(queue);

  //Queue out -> wait time
  if(current_packet->tmp_in_time > 0)
  {
    current_packet->tmp_out_time = getTime();
    current_packet->wait_time += calculateTime(current_packet->tmp_in_time, current_packet->tmp_out_time);
    current_packet->service_time += current_packet->wait_time;
  }
  else
    current_packet->tmp_out_time = getTime();

  --(queue->packet_list->cnt);

  vnf->avail -= strlen(current_packet->data);
  calculate_vnf_usage(vnf);
  
  log_Packet(current_packet, queue->key);
  return current_packet;
}

int calculateTime(int in_time, int out_time)
{
  return out_time - in_time;
}

int putPacket(PACKET *packet, QUEUE *out_queue, VNF *vnf)
{
  pushQueue(out_queue, packet, vnf);
}

int pushQueue(QUEUE *queue, PACKET *packet, VNF *vnf)
{
  if((queue->avail - strlen(packet->data)) < 0) return -1;

  queue->avail -= strlen(packet->data);
  calculate_queue_usage(queue);
  
  //queue out -> process time
  if(packet->tmp_out_time > 0)
  {
    packet->tmp_in_time = getTime();
    packet->process_time += calculateTime(packet->tmp_in_time, packet->tmp_out_time);
    packet->service_time += packet->process_time;
  }
  else
    packet->tmp_in_time = getTime();

  ++(queue->packet_list->cnt);

  vnf->avail += strlen(packet->data);
  calculate_vnf_usage(vnf);
  
  queue->packet_list->tail = packet;
  queue->packet_list->tail->next = packet;
  log_Packet(packet, queue->key);
  return 1;
}

SERVER *getServer(int server_key)
{
  SERVER *find_server = (SERVER *)malloc(sizeof(SERVER));

  find_server = server_list->head;
  while(find_server == NULL)
  {
    if(find_server->key == server_key) break;
    find_server = find_server->next;
  }

  return find_server;
}

QUEUE *getQueue(int queue_key)
{
  QUEUE *find_queue = (QUEUE *)malloc(sizeof(QUEUE));

  find_queue = queue_list->head;
  while(find_queue == NULL)
  {
    if(find_queue->key == queue_key) break;
    find_queue = find_queue ->next;
  }

  return find_queue;
}

VNF_TYPE *getVNFType(int vnf_type_key)
{
  VNF_TYPE *find_vnf_type = (VNF_TYPE *)malloc(sizeof(VNF_TYPE));

  find_vnf_type = vnf_type_list->head;
  while(find_vnf_type == NULL)
  {
    if(find_vnf_type->key == vnf_type_key) break;
    find_vnf_type = find_vnf_type ->next;
  }

  return find_vnf_type;
}

int getTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * MICROSECOND + tv.tv_usec;
}

int getDate()
{
  int Idate, Itime;
  _GetDTime(&Idate, &Itime, 100);
  return Idate;
}

void calculate_server_usage(SERVER *server)
{
  server->cpu_usage = (float)((server->t_cpu-server->a_cpu)/server->t_cpu);
  server->mem_usage = (float)((server->t_mem-server->a_mem)/server->a_mem);
  server->disk_usage = (float)((server->t_disk-server->a_disk)/server->a_disk);
}

void calculate_queue_usage(QUEUE *queue)
{
  queue->queue_usage = (float)((queue->size-queue->avail)/queue->size);
}

void calculate_vnf_usage(VNF *vnf)
{
  vnf->vnf_usage = (float)((vnf->throughput-vnf->avail)/vnf->throughput);
}
