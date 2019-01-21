/*------------------------------------------------------------------------
# Author  : David Cho
#	Module	: operate message queue - create, send, receive and remove
#           operate shared memory - create, attach, detach and remove
#	File	: lib_ipc.c
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
	Header Files
------------------------------------------------------------------------*/
#include	"common.h"

/*************************************************************************
	Function		: . create message queue
	Parameters IN	: . p_msgkey	: message queue key
	Parameters OUT	: .
	Return Code		: . int (message queue ID: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		creat_msg (key_t p_msgkey)
/*----------------------------------------------------------------------*/
{
	int		rt;

	rt = msgget (p_msgkey, 0664 | IPC_CREAT);

	return (rt);
}	/* End of creat_msg ()	*/

/*----------------------------------------------------------------------*/
int		creat_excl_msg (key_t p_msgkey)
/*----------------------------------------------------------------------*/
{
	int		rt;

	rt = msgget (p_msgkey, 0664 | IPC_CREAT | IPC_EXCL);

	return (rt);
}	/* End of creat_excl_msg ()	*/

/*************************************************************************
	Function		: . send message queue
	Parameters IN	: . p_msgid	: message queue identifier
					  . p_msg	: message
					  . p_size	: message size
					  . p_flag	: message flags
	Parameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		send_msg (int p_msgid, void *p_msg, int p_size, int p_flag)
/*----------------------------------------------------------------------*/
{
	int     rt;

	rt = msgsnd (p_msgid, p_msg, p_size, p_flag);

	return (rt);
}	/* End of send_msg ()	*/

/*************************************************************************
	Function		: . receive message queue
	Parameters IN	: . p_msgid	: message queue identifier
					  . p_msg	: message pointer
					  . p_size	: message size
					  . p_type	: message type
					  . p_flag	: message flags
	Parameters OUT	: . p_msg	: message
	Return Code		: . int (>= 0: length of message, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		recv_msg (p_msgid, p_msg, p_size, p_type, p_flag, ttime)
int		p_msgid;
void	*p_msg;
int		p_size;
int		p_type;
int		p_flag;
int		ttime;
/*----------------------------------------------------------------------*/
{
	int		rt;

	alarm (ttime);

	rt = msgrcv (p_msgid, p_msg, p_size, p_type, p_flag);
	if (rt == -1)
		return (rt);

	alarm (0);

	return (rt);
}	/* End of recv_msg ()	*/

/*************************************************************************
	Function		: . remove message queue identifier
	Parameters IN	: . p_msgid	: message queue identifier
	Parameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		remove_msg (int p_msgid)
/*----------------------------------------------------------------------*/
{
	int     rt;

	rt = msgctl (p_msgid, IPC_RMID, (struct msqid_ds *)0);

	return (rt);
}	/* End of remove_msg ()	*/

/*************************************************************************
	Function		: . create shared memory
	Parameters IN	: . p_shmkey	: shared memory key
					  . p_shmsize	: size of shared memory segment
	Parameters OUT	: .
	Return Code		: . int (shared memory identifier: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		creat_shm (key_t p_shmkey, size_t p_shmsize)
/*----------------------------------------------------------------------*/
{
	int     rt;

	rt = shmget (p_shmkey, p_shmsize, 0664 | IPC_CREAT);

	if (rt == -1)
		return (creat_excl_shm (p_shmkey, p_shmsize));

	return (rt);
}	/* End of creat_shm ()	*/

/*----------------------------------------------------------------------*/
int		creat_excl_shm (key_t p_shmkey, size_t p_shmsize)
/*----------------------------------------------------------------------*/
{
	int     rt;

	rt = shmget (p_shmkey, p_shmsize, 0664 | IPC_CREAT | IPC_EXCL);

	return (rt);
}	/* End of creat_excl_shm ()	*/

/*************************************************************************
	Function		: . attach shared memory
	Parameters IN	: . p_shmid	: shared memory identifier
	Parameters OUT	: .
	Return Code		: . char * (data segment start address of the attached
						shared memory segment: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
char	*attach_shm (int p_shmid)
/*----------------------------------------------------------------------*/
{
	char    *rt;

	rt = shmat (p_shmid, (char *)0, 0);

	return (rt);
}	/* End of attach_shm ()	*/

/*************************************************************************
	Function		: . detach shared memory
	Parameters IN	: . p_shmptr	: shared memory segment start address
	Parameters OUT	: .
	Return Code		: . void
*************************************************************************/
/*----------------------------------------------------------------------*/
void	detach_shm (char *p_shmptr)
/*----------------------------------------------------------------------*/
{
	if (p_shmptr != NULL)
		shmdt (p_shmptr);
	
	return;
}	/* End of detach_shm ()	*/

/*************************************************************************
	Function		: . remove shared memory identifier
	Parameters IN	: . p_shmid	: shared memory identifier
	Parameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		remove_shm (int p_shmid)
/*----------------------------------------------------------------------*/
{
	int		rt;

	rt = shmctl (p_shmid, IPC_RMID, (struct shmid_ds *)0);
	
	return (rt);
}	/* End of remove_shm ()	*/

/*************************************************************************
	End of Program (lib_ipc.c)
*************************************************************************/
