/**
packet generator
packetgenerator [count per min] [sens_cpu] [sens_mem] [sens_disk] [packet_size]
**/
#include "nfv_header.h"

#define MICROSECOND         1000000
#define MAX_SENS            10
#define MAX_PACKET_COUNT    10000 //per min
#define MAX_PACKET_SIZE     10000

int getRandomInt(int Max)
{
  int seed; 
  seed = 10;
  srand(seed);

  return rand()%Max;
}

VNF *getVNFbyInQueue(QUEUE *queue)
{
  VNF *find_vnf = (VNF *)malloc(sizeof(VNF));

  find_vnf = vnf_list->head;
  while(find_vnf == NULL)
  {
    if(find_vnf->in_queue == queue) break;
    find_vnf = find_vnf ->next;
  }

  return find_vnf;	
}

int main(int argc, char *argv[])
{
  //if send == 0, then random
  int lcount_per_min, ccount_per_min = 0;
  int sens_cpu, sens_mem, sens_disk = 0;
  int MaxCPUSens, MaxMemSens, MaxDiskSens = 10;
  int seq = 0;
  int interval_check_begin = 0;
  int packet_size = 0;
  
  PACKET *packet = (PACKET *)malloc(sizeof(PACKET));
  
  if(argc < 3)
  {
    printf("packetgenerator [count per min] [sens_cpu] [sens_mem] [sens_disk] [packet_size]\n");
    printf("EX: packetgenerator 1000 5 0 0 1024\n");

    return -1;
  }

  lcount_per_min = atoi(argv[1]);
  if(lcount_per_min == 0) lcount_per_min = getRandomInt(MAX_PACKET_COUNT);
   
  interval_check_begin = getTime();

  while(1)
  {
    if(ccount_per_min > lcount_per_min) continue;
    memset(packet->data, 0x00 , sizeof(packet->data));
    
    if(atoi(argv[5]) > 0)
      packet_size  = atoi(argv[4]);
    else
      packet_size = getRandomInt(MAX_PACKET_SIZE);
      
    packet->seq = ++seq;
    memset(packet->data, '9' , packet_size);

    if(atoi(argv[2]) > 0)
      packet->sens_cpu  = atoi(argv[2]);
    else
      packet->sens_cpu  = getRandomInt(MAX_SENS);

    if(atoi(argv[3]) > 0)
      packet->sens_mem  = atoi(argv[3]);
    else
      packet->sens_mem  = getRandomInt(MAX_SENS);

    if(atoi(argv[4]) > 0)
      packet->sens_disk  = atoi(argv[4]);
    else
      packet->sens_disk = getRandomInt(MAX_SENS);

    packet->wait_time = packet->process_time = packet->service_time = 0;

    putPacket(packet, queue_list->head, getVNFbyInQueue(queue_list->head)); //Put the first registered queue
    ++ccount_per_min;

    if((getTime() - interval_check_begin) > (60 * MICROSECOND)) //60 sec
    {
      interval_check_begin= getTime();
      ccount_per_min = 0;
      if(atoi(argv[1]) == 0) lcount_per_min = getRandomInt(MAX_PACKET_COUNT); //where 0 is entered, keep updating lcount_per_min
    }
  }

  return 1;  
}
