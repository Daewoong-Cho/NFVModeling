typedef struct _server_
{
  int key; //uniq_key
  int t_cpu; //total cpu cores
  int t_mem; //total memory (MB)
  int t_disk; //total disk size (MB)
  
  int a_cpu; //cpu core availability
  int a_mem; //memory availability
  int a_disk; //disk availability

  SERVER *parent; //if null, physical machine
  SERVER *next;

  char job_type; //C, M, D
}SERVER;

typedef struct _server_list_
{
  int seq = 0;
  SERVER *head;
  SERVER *tail;
}SERVER_LIST;

SERVER_LIST *server_list;

void log_ServerStatus();
int check_Resource(int rcpu, int rmem, int rdisk, SERVER *parent_server);
int alloc_Resource(int rcpu, int rmem, int rdisk, SERVER *pm);
SERVER *deploy_SERVER(int key, int alloc_cpu, int alloc_mem, int alloc_disk, SERVER *parent_server, char job_type);
void revoke_SERVER(SERVER *server);
void free_Resource(SERVER *server);