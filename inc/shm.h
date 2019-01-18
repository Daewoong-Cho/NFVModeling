#ifndef	_SHM_H_
#define	_SHM_H_

#include	"shm_def.h"

/*------------------------------------------------------------------------
	Structure
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
    Shared Memory
------------------------------------------------------------------------*/

#define _PROC_SHM_SET_(shm_key, shm_max_size) {\
	      int uniq_key; \
	      int shm_size; \
	      int shm_id; \
	      char   *shm_addr; \
	      uniq_key = shm_key; \
	      shm_size = shm_max_size; \
	      if( 0> (shm_id= shmget(uniq_key, shm_size, IPC_CREAT | 0666) ) ) \
	          return( (char *)-1); \
	      if( (char *)NULL== (shm_addr= (char *)shmat(shm_id, 0, 0) ) ) \
	          return( (char *)-1); \
	      return( shm_addr ); \
}

#define _DECLARE_PROC_SHM(type, _func_name, shm_key, shm_max_size) \
	      type Set##_func_name() _PROC_SHM_SET_(shm_key, shm_max_size)

_DECLARE_PROC_SHM(char *, KOSharedMemory,     KOSPI200_OPT_SHM,           sizeof(struct KOSPI200_OPT_INFO))

#endif
