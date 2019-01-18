#include "common.h"

void log_Packet(PACKET *packet, int queue_key)
{
  char sqlStr[40960];
  memset(sqlStr, 0x20, sizeof(sqlStr));
  sprintf(sqlStr, 
    " INSERT INTO          "
    "   PACKET_LOG         "
    " (                    "
    "   queue_key         ,"
    "   packet_seq        ,"
    "   wait_time         ,"
    "   process_time      ,"
    "   service_time       "
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
      packet->packet_seq    ,
      packet->wait_time     ,
      packet->process_time  ,
      packet->service_time       
  );
  if mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
}

//After packet pop
//queue->packet_list->head = queue->packet_list->head->next;

PACKET *getPacket(QUEUE *in_queue)
{
  return popQueue(in_queue);
}

PACKET *popQueue(Queue *queue)
{
  PACKET *current_packet;
  current_packet = queue->packet_list->head;

  queue->queue_avail++;
  queue->processed_cnt++;

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
  log_Packet(current_packet, queue->key);
  return current_packet;
}

int calculateTime(int in_time, int out_time)
{
  return out_time - in_time;
}

int putPacket(PACKET *packet, QUEUE *out_queue)
{
  pushQueue(packet, out_queue);
}

int pushQueue(QUEUE *queue, PACKET *packet)
{
  if(queue->queue_avail <= 0) return -1

  queue->queue_avail--;

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
  queue->packet_list->tail = packet;
  queue->packet_list->tail->next = packet;
  log_Packet(packet, queue->key);
  return 1;
}