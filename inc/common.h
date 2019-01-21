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

/*------------------------------------------------------------------------
	Define
------------------------------------------------------------------------*/
#define MICROSECOND 1000000

#define TM_YEAR_BASE	1900
#define	SYS_NO			errno
#define	SYS_STR			strerror(errno)

#define	FAIL			-1
#define	SUCCESS			1
#define	TCP_TIME_OUT	30
#define	POLL_TIME_OUT	20 * 1000
#define	TIME_OUT_FLAG	0

#define	TCP_SIZE_LEN	1024
#define	BUF_SIZE_LEN	4096

#define	MAX_CONNECT_TRY	3
#define	POLL_SEND		"POLLSEND"
#define	POLL_RECV		"POLLRECV"
#define	BIN_PATH		"/home/david/bin"

#define DEF_VM			"VM"
#define DEF_VNIC		"vNIC"
#define DEF_PNIC		"pNIC"
#define DEF_VSWITCH		"vSwitch"
#define DEF_PSWITCH		"pSwitch"

#define PTP_TCP_SVR		"TCP_SERVER"
#define PTP_TCP_CLT		"TCP_CLIENT"

#define PTP_UDP_SVR		"UDP_SERVER"
#define PTP_UDP_CLT		"UDP_CLIENT"


#define	Q_KEY			30000

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
