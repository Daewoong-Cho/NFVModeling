/**
packet generator
PG [count per min] [sens_cpu] [sens_mem] [sens_disk]
**/
#include "common.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int getTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * MICROSECOND + tv.tv_usec;
}

int getRandomInt(int Max)
{
  int seed; 
  seed = 10;
  srand(seed);

  return rand()%Max;
}

int main(int argc, char *argv[])
{
  //if send == 0, then random
  int lcount_per_min, ccount_per_min = 0;
  int sens_cpu, sens_mem, sens_disk = 0;
  int MaxCPUSens, MaxMemSens, MaxDiskSens = 10;
  int seq = 0;
  int interval_check_begin = 0;
  PACKET *packet = (PACKET *)malloc(sizeof(PACKET));
  
  if(argc < 3) return -1;
  lcount_per_min = atoi(argv[1]);
   
  interval_check_begin = getTime();

  while(1)
  {
    if(ccount_per_min > lcount_per_min) continue;

    packet->seq = ++seq;
    memset(packet->data, '9' , sizeof(packet->data));

    if(atoi(argv[2]) > 0)
      packet->sens_cpu  = atoi(argv[2]);
    else
      packet->sens_cpu  = getRandomInt(MaxCPUSens);

    if(atoi(argv[3]) > 0)
      packet->sens_mem  = atoi(argv[3]);
    else
      packet->sens_mem  = getRandomInt(MaxMemSens);

    if(atoi(argv[4]) > 0)
      packet->sens_disk  = atoi(argv[4]);
    else
      packet->sens_disk = getRandomInt(MaxDiskSens);

    packet->wait_time = packet->process_time = packet->service_time = 0;

    putPacket(packet, queue_list->head); //Put the first registered queue
    ++ccount_per_min;

    if((getTime() - interval_check_begin) > (60 * MICROSECOND)) //60 sec
    {
      interval_check_begin= getTime();
      ccount_per_min = 0;
    }
  }

  return 1;  
}