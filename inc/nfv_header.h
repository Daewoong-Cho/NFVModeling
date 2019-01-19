#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include "/usr/include/mysql/mysql.h"

#define MICROSECOND 1000000

MYSQL MDB_session;

typedef struct _server_
{
  int key; //uniq_key
  int t_cpu; //total cpu cores
  int t_mem; //total memory (MB)
  int t_disk; //total disk size (MB)
  
  int a_cpu; //cpu core availability
  int a_mem; //memory availability
  int a_disk; //disk availability

  struct _server_ *parent; //if null, physical machine
  struct _server_ *next;

  char job_type; //C, M, D
}SERVER;

typedef struct _server_list_
{
  int seq;
  SERVER *head;
  SERVER *tail;
}SERVER_LIST;

SERVER_LIST *server_list;


typedef struct _packet_
{
  int seq;
  char data[1024];

  int sens_cpu;
  int sens_mem;
  int sens_disk;

  int wait_time; //time in queue
  int process_time; //time in VNF
  int service_time;  // wait + process time

  int tmp_in_time;
  int tmp_out_time;
  struct _packet_ *next;
}PACKET;

typedef struct _packet_list_
{
  int cnt;
  PACKET *head;
  PACKET *tail;
}PACKET_LIST;

#if 0
PACKET_LIST *packet_list;
#endif


typedef struct _queue_
{
  int key;
  int size;
  int avail; 
  int processed_cnt;
  SERVER *server;
  PACKET_LIST *packet_list;
  struct _queue_ *next;
}QUEUE;

typedef struct _queue_list_
{
  int seq;
  QUEUE *head;
  QUEUE *tail;
}QUEUE_LIST;

QUEUE_LIST *queue_list;

typedef struct _vnf_type_
{
  int key;
  int req_cpu;
  int req_mem;
  int req_disk;
  struct _vnf_type_ *next;
}VNF_TYPE;

typedef struct _vnf_type_list_
{
  int seq;
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
  struct _vnf_type_ *vnf_type;
  SERVER *server;
  
  //long acml_wait_time; //us
  //long avg_wait_time; //us
  struct _vnf_ *next;
  QUEUE *in_queue;
  QUEUE *out_queue;
}VNF;

typedef struct _vnf_list_
{
  int seq;
  VNF *head;
  VNF *tail;
}VNF_LIST;

VNF_LIST *vnf_list;

VNF_TYPE *add_VNF_TYPE(int key, int req_cpu, int req_mem, int req_disk);
VNF *deploy_VNF(int key, int throughput, VNF_TYPE *type, SERVER *server, QUEUE *in_queue, QUEUE *out_queue);
void revoke_VNF(VNF *vnf);
void log_VNFStatus();

void log_ServerStatus();
int check_Resource(int, int, int, SERVER *);
int alloc_Resource(int, int, int, SERVER *);
SERVER *deploy_SERVER(int key, int alloc_cpu, int alloc_mem, int alloc_disk, SERVER *parent_server, char job_type);
void revoke_SERVER(SERVER *server);
void free_Resource(SERVER *server);

QUEUE *deploy_QUEUE(int key, int size, SERVER *server);
void revoke_QUEUE(QUEUE *queue);
void log_QueueStatus();

PACKET *getPacket(QUEUE *in_queue);
PACKET *popQueue(QUEUE *queue);
int calculateTime(int in_time, int out_time);
int putPacket(PACKET *packet, QUEUE *out_queue);
int pushQueue(QUEUE *queue, PACKET *packet);

SERVER *getServer(int server_key);
QUEUE *getQueue(int queue_key);
VNF_TYPE *getVNFType(int vnf_type_key);

int getTime();
