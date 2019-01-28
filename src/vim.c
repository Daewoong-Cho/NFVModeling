/**
VIM: deploy_server/queue, log_status 
**/
#include "nfv_header.h"
#include <json-c/json.h>

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
  char *job_type;
  int queue_key, queue_size, queue_server_key = 0;
  int vnf_type_key, req_cpu, req_mem, req_disk = 0;
  int vnf_key, vnf_throughput, vnf_server_key, vnf_in_queue_key, vnf_out_queue_key = 0;
  char key[10];
  char buff[81920];
  int time_stamp = 0;
  int fd = -1;
  char filepath[8192] = {0,};

	sprintf(filepath, "%s/cfg/env.json", getenv("HOME"));
  fd = open(filepath, "a");
  read(fd, buff, sizeof(buff));  
	 
  memset(buff, 0x00, sizeof(buff));
  
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
    memset(key, 0x00, sizeof(key));
    sprintf(key, "server%d", index);
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
    job_type = (char *) json_object_get_string(field);

    deploy_SERVER(server_key, alloc_cpu, alloc_mem, alloc_disk, getServer(parent_server_key), job_type[0]);
  }

  //Queue Placement
  index = 0;
  while(index++ < queue_cnt)
  {  
    memset(key, 0x00, sizeof(key));
    sprintf(key, "queue%d", index);
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
    memset(key, 0x00, sizeof(key));
    sprintf(key, "vnf_type%d", index);
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
    memset(key, 0x00, sizeof(key));
    sprintf(key, "vnf%d", index);
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

    deploy_VNF(vnf_key, vnf_throughput, getVNFType(vnf_type_key), getServer(vnf_server_key), getQueue(vnf_in_queue_key), getQueue(vnf_out_queue_key));
  }

  while(1)
  {
    log_ServerStatus(++time_stamp);
    log_QueueStatus(time_stamp);
    log_VNFStatus(time_stamp);
    sleep(1); 
  }
}
