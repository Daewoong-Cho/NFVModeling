typedef struct _packet_
{
  int packet_seq;
  char data[1024];

  int sens_cpu;
  int sens_mem;
  int sens_disk;

  int wait_time; //time in queue
  int process_time; //time in VNF
  int service_time;  // wait + process time

  int tmp_in_time;
  int tmp_out_time;
  PACKET *next;
}PACKET;

typedef struct _packet_list_
{
  int cnt = 0;
  PACKET *head;
  PACKET *tail;
}PACKET_LIST;

#if 0
PACKET_LIST *packet_list;
#endif
