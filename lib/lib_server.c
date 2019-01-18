#include "common.h"

SERVER *deploy_SERVER(int key, int alloc_cpu, int alloc_mem, int alloc_disk, SERVER *parent_server, char job_type)
{
  SERVER *new_server;
  new_server= (SERVER *)malloc(sizeof(SERVER));

  if(pm != NULL)
    if(check_Resource(alloc_cpu, alloc_mem, alloc_disk, parent_server) < 0) return NULL;

  alloc_Resource(alloc_cpu, alloc_mem, alloc_disk, parent_server);

  ++(server_list->seq);
  new_server->server_key = key;

  new_server->t_cpu = alloc_cpu;
  new_server->t_mem = alloc_mem;
  new_server->t_disk = alloc_disk;

  new_server->a_cpu = alloc_cpu;
  new_server->a_mem = alloc_mem;
  new_server->a_disk = alloc_disk;

  new_server->parent_server = parent_server;
  new_server->type = type;

  if(server_list->head == NULL && server_list->tail == NULL)
  { 
    server_list->head = server_list->tail = new_server;
    //server_list->head->next = server_list->tail->next = new_server;
   }
   else
   {
    server_list->tail->next = new_server;
    server_list->tail = new_server;
   }
  return new_server;
}

void revoke_SERVER(SERVER *server) //remove linked list & free resource
{
  free_Resource(server);
  server = server->next;
}

void free_Resource(SERVER *server)
{
  server->parent->a_cpu += server->t_cpu;
  server->parent->a_mem += server->t_mem;
  server->parent->a_disk += server->t_disk;
}

void log_ServerStatus()
{
  char sqlStr[40960];
  SERVER *server = server_list->head;
  while(server != NULL) 
  {
    memset(sqlStr, 0x20, sizeof(sqlStr));
    sprintf(sqlStr, 
      " INSERT INTO          "
      "   SERVER_LOG         "
      " (                    "
      "   key               ,"
      "   t_cpu             ,"
      "   t_mem             ,"
      "   t_disk            ,"
      "   a_cpu             ,"
      "   a_mem             ,"
      "   a_disk            ,"
      "   p_server_key       "
      " )                    "
      " VALUES               "
      " (                    "
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                ,"
      "   %d                 "
      " )                    "
      " ;                    ",
        server->key         ,
        server->t_cpu       ,
        server->t_mem       ,
        server->t_disk      ,
        server->a_cpu       ,
        server->a_mem       ,
        server->a_disk      ,
        server->parent->key       
      );
    if mysql_query(&MDB_session, sqlStr) != 0) //Insert Error
    server = server->next;
  }
}

int check_Resource(int rcpu, int rmem, int rdisk, SERVER *parent_server)
{
  if((pm->a_cpu > rcpu) && 
     (pm->a_mem > rmem) && 
     (pm->a_disk > rdisk)) 
  {return -1;}
  else return 1;
}

int alloc_Resource(int rcpu, int rmem, int rdisk, SERVER *pm)
{
  pm->a_cpu -= rcpu;
  pm->a_mem -= rmem;
  pm->a_disk-= rdisk;
  return 1;
}
