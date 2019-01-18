#include	"common.h"

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

  if(server != NULL)
    if(check_Resource(type->req_cpu, type->req_mem, type->req_disk, server) < 0) return NULL;
  alloc_Resource(type->req_cpu, type->req_mem, type->req_disk, server);

  new_vnf->vnf_key = key; 

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
  return new_vnf;
}

void revoke_VNF(VNF *vnf) //remove linked list & free resource
{
  free_Resource(vnf->server);
  vnf = vnf->next;
}

void log_VNFStatus()
{
  char sqlStr[40960];
  VNF *vnf = vnf_list->head;
  while(vnf != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   VNF_LOG            "
      " (                    "
      "   vnf_key           ,"
      "   processed_cnt     ,"
      "   throughput        ,"
      "   avail             ,"
      "   server_key        ,"
      "   vnf_type_key      ,"
      "   in_queue_key      ,"
      "   out_queue_key      "
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
      "   %d                 "
      " )                    "
      " ;                    ",
        vnf->key             ,
        vnf->processed_cnt   ,
        vnf->throughput      ,
        vnf->avail           ,
        vnf->server->key     ,
        vnf->vnf_type->key   ,
        vnf->in_queue->key   ,
        vnf->out_queue->key       
      );
    if mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
    vnf= vnf->next;
  }
}