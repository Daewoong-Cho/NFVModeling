/**
  VNF [vnf_key]
**/

#include <stdio.h>

void processing_Delay(SERVER *server, PACKET *packet)
{
  switch (server->job_type)
  {
    case 'C':
      while(packet->sens_cpu--){continue;}
      break;
    case 'D': 
      while(packet->sens_disk--){continue;}
      break;
    case 'M': 
      char *point;
      while(packet->sens_mem--)
      {
        point = malloc(8);
        free(point);
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
    if(find_vnf>key == vnf_key) break;
    find_vnf = find_vnf ->next;
  }

  return find_vnf;
}

int main(int argc, char *argv[])
{
  int vnf_key = 0;
  VNF *vnf = (VNF *)malloc(sizeof(VNF));
  PACKET *packet = (PACKET *)malloc(sozeof(PACKET));
  
  vnf_key = atoi(argv[1]));
  vnf = getVNF(vnf_key);

  while(1)
  {
    packet = getPacket(vnf->in_queue);
    in_queue->packet_list->head = in_queue->packet_list->head->next; //Extract the top queue
    if(packet != NULL) 
    {
      processing_Delay(vnf->server, packet);
      if(vnf->out_queue != NULL)
        putPacket(vnf->out_queue, packet);
    }
    else usleep(10);
  }
  return 1;
}
