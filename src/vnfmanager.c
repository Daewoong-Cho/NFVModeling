/**
 VNF Manager: Algorithms, VNF reallocation strategy
 
 For Performance
 1. VNF slicing
 2. VNF scale-out
 
 
**/

#define THRESHOLD 0.5

int main(int argc, char *argv[])
{
  VNF *vnf = (VNF *)malloc(sizeof(VNF));
  vnf = vnf_list->head;
    
  while(1)
  {
   if(vnf->vnf_usage < THRESHOLD && vnf->in_queue->queue_usage > THRESHOLD)
   {
     VNF *new_vnf = (VNF *)malloc(sizeof(VM *));
     n
     //change setting
   }
  }
  return 1;
}
