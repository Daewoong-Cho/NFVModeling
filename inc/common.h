#ifndef _COMMON_H_
#define _COMMON_H_

#include	<stdio.h>

#include	<memory.h>
#include	<stdarg.h>
#include	<time.h>
#include	<signal.h>
#include	<sys/poll.h>
#include	<sys/select.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<math.h>

#include	<errno.h>

/* IPC headers */
#include	<sys/ipc.h>
#include	<sys/msg.h>
#include	<sys/shm.h>

/* TCP/IP headers   */
#include	<sys/socket.h>
#include 	<netinet/in.h>
#include 	<arpa/inet.h>
#include 	<netdb.h>

#include	<pthread.h>
#include 	<time.h>

#include "vnf.h"
#include "queue.h"
#include "server.h"
#include "packet.h"

/*------------------------------------------------------------------------
	Define
------------------------------------------------------------------------*/
#define MICROSECOND 1000000

/*------------------------------------------------------------------------
	Function Prototypes
------------------------------------------------------------------------*/
int		creat_msg (key_t);
int		creat_excl_msg (key_t);
int		send_msg (int, void *, int, int);
int		recv_msg (int, void *, int, int, int, int);
int		remove_msg (int);

int		creat_shm (key_t, size_t);
int		creat_excl_shm (key_t, size_t);
char	*attach_shm (int);
void	detach_shm (char *);
int		remove_shm (int);

void	_DispLog(char *, char *, ...);
void	_GetDTime(int *, int *, int);
	
int		(*proc_ptr)(char *);


/*------------------------------------------------------------------------
	Structure
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
	Extern
------------------------------------------------------------------------*/
