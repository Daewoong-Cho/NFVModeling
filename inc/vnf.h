typedef struct _vnf_type_
{
  int key;
  int req_cpu;
  int req_mem;
  int req_disk;
  VNF_TYPE *next;
}VNF_TYPE;

typedef struct _vnf_type_list_
{
  int seq = 0;
  VNF_TYPE *head;
  VNF_TYPE *tail;
}VNF_TYPE_LIST;

VNF_TYPE_LIST *vnf_type_list;

typedef struct _vnf_
{
  int key;
  int processed_cnt;
  int throughput;
  int avail;
  VNF_TYPE *vnf_type;
  SERVER *server;
  
  //long acml_wait_time; //us
  //long avg_wait_time; //us
  VNF *next;
  QUEUE *in_queue;
  QUEUE *out_queue;
}VNF;

typedef struct _vnf_list_
{
  int seq = 0;
  VNF *head;
  VNF *tail;
}VNF_LIST;

VNF_LIST *vnf_list;

VNF_TYPE *add_VNF_TYPE(int key, int req_cpu, int req_mem, int req_disk);
VNF *deploy_VNF(int key, int throughput, VNF_TYPE *type, SERVER *server, QUEUE *in_queue, QUEUE *out_queue);
void revoke_VNF(VNF *vnf);
void log_VNFStatus();