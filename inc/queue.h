typedef struct _queue_
{
  int key;
  int size;
  int avail; 
  int processed_cnt;
  SERVER *server;
  PACKET_LIST *packet_list;
}QUEUE;

typedef struct _queue_list_
{
  int seq = 0;
  QUEUE *head;
  QUEUE *tail;
}QUEUE_LIST;

QUEUE_LIST *queue_list;

QUEUE *deploy_QUEUE(int key, int size, SERVER *server);
void revoke_QUEUE(QUEUE *queue);
void log_QueueStatus();