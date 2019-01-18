#include <stdio.h>
#include <queue.h>

QUEUE *deploy_QUEUE(int key, int size, SERVER *server)
{
  QUEUE *new_queue;
  new_queue = (QUEUE *)malloc(sizeof(QUEUE));

  if(server == NULL) return; 

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

void log_QueueStatus()
{
  char sqlStr[40960];
  QUEUE *queue = queue_list->head;
  while(queue != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   QUEUE_LOG         "
      " (                    "
      "   key               ,"
      "   size              ,"
      "   avail             ,"
      "   processed_cnt     ,"
      "   wait_cnt          ,"
      "   server_key         "
      " )                    "
      " VALUES               "
      " (                    "
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                 "
      " )                    "
      " ;                    ",
        queue->key              ,
        queue->size             ,
        queue->avail            ,
        queue->processed_cnt    ,
        queue->packet_list->cnt ,
        queue->server->key       
      );
    if mysql_query(&MDB_session, sqlStr) != 0) //Insert Error

    queue = queue->next;
  }
}