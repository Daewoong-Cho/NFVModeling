//extern int proc_A0034(char *org_data);
extern _DECLARE_PROC_SHM(char *, ST_INFO,           CFG_PROC_SHM,           sizeof(struct ST_INFO))
extern _DECLARE_PROC_SHM(char *, VM_INFO,           CFG_VM_SHM,           sizeof(struct VM_INFO))
extern _DECLARE_PROC_SHM(char *, NF_INFO,           CFG_NF_SHM,           sizeof(struct NF_INFO))

extern struct  ST_INFO *ptr;
extern struct  VM_INFO	*pvm;
extern struct  NF_INFO	*pnf;
extern int	   OFFSET;
