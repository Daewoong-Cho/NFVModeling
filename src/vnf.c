/**
  VNF [vnf_key]
**/

#include "nfv_header.h"
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS_COUNT 10
#define NEXT_PACKET_SEARCH_USLEEP 10

pthread_t pthread[2];

PACKET_LIST *packet_internal_queue;

void *pakcet_receive(VNF *);
void *pakcet_process(VNF *);

void processing_Delay(SERVER *server, PACKET *packet)
{
	char *tmp;
  switch (server->job_type)
  {
    case 'C':
      while(packet->sens_cpu--){continue;}
      break;
    case 'D': 
      while(packet->sens_disk--){continue;}
      break;
    case 'M': 
      while(packet->sens_mem--)
      {
        tmp = (char *)malloc(8);
        free(tmp);
      }
      break;
    default:
      break;
  }
}

VNF *getVNF(int vnf_key)
{
  VNF *find_vnf = (VNF *)malloc(sizeof(VNF));

  find_vnf = vnf_list->head;
  while(find_vnf == NULL)
  {
    if(find_vnf->key == vnf_key) break;
    find_vnf = find_vnf ->next;
  }

  return find_vnf;
}

//thread packet_receive
void *packet_receive(void *arg)
{
  PACKET *packet = (PACKET *)malloc(sizeof(PACKET));
  VNF *vnf = arg;
  
  while(1)
  {
    packet = getPacket(vnf->in_queue, vnf);    
    if(packet == NULL) 
      usleep(NEXT_PACKET_SEARCH_USLEEP);
    else 
    {
      if(packet_internal_queue->head == NULL && packet_internal_queue->tail == NULL)
        packet_internal_queue->head = packet_internal_queue->tail = packet;
      else 
      {
        packet_internal_queue->tail->next = packet;  
        packet_internal_queue->tail = packet;
      }
      
      vnf->in_queue->packet_list->head = vnf->in_queue->packet_list->head->next; //To the next packet
    }
  }
}

//thread packet_process
void *packet_process(void *arg)
{
  PACKET *packet = (PACKET *)malloc(sizeof(PACKET));
  VNF *vnf = arg;
  
  while(1)
  {
    packet = packet_internal_queue->head; 
    if(packet == NULL)
      usleep(NEXT_PACKET_SEARCH_USLEEP);
    else
    {
      processing_Delay(vnf->server, packet);
      if(vnf->out_queue != NULL)
      {
        while(1)
        {
          if(putPacket(packet, vnf->out_queue, vnf) < 0) //queue full
          {
            usleep(NEXT_PACKET_SEARCH_USLEEP);
            continue;
          }
          else 
           break;
        }
      }
      packet_internal_queue->head = packet_internal_queue->head->next;
    }
  }
  pthread_exit((void *) 0);
}

int main(int argc, char *argv[])
{
  int vnf_key = 0;
  VNF *vnf = (VNF *)malloc(sizeof(VNF));
  int rc;
  int status;
  
  vnf_key = atoi(argv[1]);
  vnf = getVNF(vnf_key);

  //create packet receive thread
  pthread_create(&pthread[0], NULL, &packet_receive, (VNF *)vnf);
  rc = pthread_join(pthread[0], (void **)&status);
  if(rc == 0) return -1; //Fail
  pthread_create(&pthread[1], NULL, &packet_process, (VNF *)vnf);
  rc = pthread_join(pthread[1], (void **)&status);
  if(rc == 0) return -1; //Fail
  
  return 1;
}
