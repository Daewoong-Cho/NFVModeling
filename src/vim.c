/**
VIM: deploy_server/queue, log_status
**/
#include <stdio.h>
#include <json.json.h>

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

int main(int argc, char *argv[])
{
  SERVER *new_server = (SERVER *)malloc(sizeof(SERVER));
  QUEUE *new_queue = (QUEUE *)malloc(sizeof(QUEUE));
  VNF_TYPE *new_vnf_type = (VNF_TYPE *)malloc(sizeof(VNF_TYPE));
  VNF *new_vnf = (VNF *)malloc(sizeof(VNF));
  json_object *cfg, *headerobj, *server, *queue, *vnf_type, *vnf, *field;
  int server_cnt, queue_cnt, vnf_type_cnt, vnf_cnt = 0;
  int index = 0;
  int server_key, alloc_cpu, alloc_mem, alloc_disk, parent_server_key = 0;
  char job_type;
  int queue_key, queue_size, queue_server_key = 0;
  int vnf_type_key, req_cpu, req_mem, req_disk = 0;
  int vnf_key, vnf_throughput, vnf_type, vnf_server_key, vnf_in_queue_key, vnf_out_queue_key = 0;
  char key[10];
  
  cfg = json_tokener_parse(buff);
  headerobj = json_object_object_get(cfg, "header");
  field = json_object_object_get(headerobj, "server_cnt");
  server_cnt = json_object_get_int(field);
  field = json_object_object_get(headerobj, "queue_cnt");
  queue_cnt = json_object_get_int(field);
  field = json_object_object_get(headerobj, "vnf_type_cnt");
  vnf_type_cnt = json_object_get_int(field);
  field = json_object_object_get(headerobj, "vnf_cnt");
  vnf_cnt = json_object_get_int(field);

  //Server Placement
  while(index++ < server_cnt)
  {  
    memset(key, 0x00, sizeof(key)):
    sprintf(key, "server%s", index);
    server = json_object_object_get(cfg, key);
    field = json_object_object_get(server, "key");
    server_key = json_object_get_int(field);
    field = json_object_object_get(server, "cpu");
    alloc_cpu = json_object_get_int(field);
    field = json_object_object_get(server, "mem");
    alloc_mem = json_object_get_int(field);
    field = json_object_object_get(server, "disk");
    alloc_disk = json_object_get_int(field);
    field = json_object_object_get(server, "parent");
    parent_server_key = json_object_get_int(field);
    field = json_object_object_get(server, "job_type");
    job_type = json_object_get_string(field);

    deploy_SERVER(server_key, alloc_cpu, alloc_mem, alloc_disk, getServer(parent_server_key), job_type);
  }

  //Queue Placement
  index = 0;
  while(index++ < queue_cnt)
  {  
    memset(key, 0x00, sizeof(key)):
    sprintf(key, "queue%s", index);
    queue = json_object_object_get(cfg, key);
    field = json_object_object_get(queue , "key");
    queue_key = json_object_get_int(field);
    field = json_object_object_get(queue , "size");
    queue_size = json_object_get_int(field);
    field = json_object_object_get(queue , "server");
    queue_server_key = json_object_get_int(field);

    deploy_QUEUE(queue_key, queue_size, getServer(queue_server_key));
  }

  //Get VNF_TYPE INFO
  index = 0;
  while(index++ < vnf_type_cnt)
  {  
    memset(key, 0x00, sizeof(key)):
    sprintf(key, "vnf_type%s", index);
    vnf_type = json_object_object_get(cfg, key);
    field = json_object_object_get(vnf_type, "key");
    vnf_type_key = json_object_get_int(field);
    field = json_object_object_get(vnf_type, "req_cpu");
    req_cpu = json_object_get_int(field);
    field = json_object_object_get(vnf_type, "req_mem");
    req_mem = json_object_get_int(field);
    field = json_object_object_get(vnf_type, "req_disk");
    req_disk = json_object_get_int(field);

    add_VNF_TYPE(vnf_type_key, req_cpu, req_mem, req_disk);
  }  

  //VNF Placement
  index = 0;
  while(index++ < vnf_cnt)
  {  
    memset(key, 0x00, sizeof(key)):
    sprintf(key, "vnf%s", index);
    vnf = json_object_object_get(cfg, key);
    field = json_object_object_get(vnf, "key");
    vnf_key = json_object_get_int(field);
    field = json_object_object_get(vnf, "throughput");
    vnf_throughput = json_object_get_int(field);
    field = json_object_object_get(vnf, "vnf_type");
    vnf_type_key = json_object_get_int(field);
    field = json_object_object_get(vnf, "server");
    vnf_server_key = json_object_get_int(field);
    field = json_object_object_get(vnf, "in_queue");
    vnf_in_queue_key = json_object_get_int(field);
    field = json_object_object_get(vnf, "out_queue");
    vnf_out_queue_key = json_object_get_int(field);

    deploy_VNF(vnf_key, vnf_throughput, getVNFType(vnf_type_key), getServer(vnf_server_key), getQueue(in_queue_key), getQueue(out_queue_key));
  }

  while(1)
  {
    log_ServerStatus();
    log_QueueStatus();
    log_VNFStatus();
    sleep(1); 
  }
}