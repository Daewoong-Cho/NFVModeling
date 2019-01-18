#include	"fo.h"

#define     KOSPI200_OPT_SHM    0x0040001000
#define     KO_LIST_COUNT       1000

typedef struct
{
    char    shrn_iscd[ 9+1];        /* 단축코드 KEY     (9) */
    char    stnd_iscd[12+1];        /* 표준코드             */
    int     offs;                   /* 종목MASTER DATA POINT */
} CODEIDX_OPT;

struct KOSPI200_OPT_INFO
{
    int             market_date;
    int             tot_lst_cnt;
    STRUC_OPT_MAST  master[KO_LIST_COUNT];   /* Option Master                            */
	STRUC_OPT_ASK	ask[KO_LIST_COUNT];      /* Option Ask                               */
	STRUC_OPT_FILL	fill[KO_LIST_COUNT];     /* Option Fill                              */
    CODEIDX_OPT     fcodeidx[KO_LIST_COUNT]; /* 종목 코드 index                          */
    CODEIDX_OPT     scodeidx[KO_LIST_COUNT]; /* 단축 코드 index                          */
};

#define SZ_KOSPI200_OPT_SHM sizeof(struct KOSPI200_OPT_INFO)

struct KOSPI200_OPT_INFO *koshm;
