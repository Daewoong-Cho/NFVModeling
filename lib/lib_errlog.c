#include  "common.h"

#define A_DOT         '.'
#define A_SPACE       ' '
#define A_TILDE       '~'

void
_DispLog(char *Pfnm, char *Pfmt, ...)
{
    va_list ap;
    FILE    *fd;
    int Idate, Itime;

    fd = fopen(Pfnm, "a+");
    if(fd == NULL) {
        return;
    }
    _GetDTime(&Idate, &Itime, 100);
    va_start(ap, Pfmt);
    fprintf(fd, "%08d:%08d |", Idate, Itime);
    vfprintf(fd, Pfmt, ap);
    fprintf(fd, "\n");
    fclose(fd);
    va_end(ap);
}

void _DispHexLog(char *Pfnm, char *title,char *msgbuf,int msglen)
{
    int dcnt;
    int indx = 0;
    int leftcnt;
    int indexptr = 0;
    unsigned char ch;
    unsigned char *bp;
    unsigned char *bufaddr;

    char buf[1024];
    int  buf_len;

    int    fd = -1;

	fd = open(Pfnm,O_WRONLY|O_APPEND|O_CREAT,0666);
	if(fd == -1) {
		return;
	}


    bufaddr = (unsigned char*)msgbuf;

    buf_len = sprintf(buf, "# hexdump1 (%s): length=[%d]\n", title, msglen);
    write(fd, buf, buf_len);

    buf_len = sprintf(buf, "# ------------------------------------------------------------------------------\n");
    write(fd, buf, buf_len);

    buf_len = sprintf(buf, "#       : 00 01 02 03 04 05 06 07-08 09 0A 0B 0C 0D 0E 0F     0123456789ABCDEF\n");
    write(fd, buf, buf_len);

    for (leftcnt=msglen; leftcnt > 0; leftcnt -= 16, bufaddr += 16) {
        dcnt = (leftcnt > 16) ? 16 : leftcnt;
        buf_len = sprintf(buf, "# 0x%04X: ", indexptr);
        write(fd, buf, buf_len);

        indexptr += 16;
        for (indx=0, bp=bufaddr; indx < dcnt; indx++) {
        if (indx == 7) {
            buf_len = sprintf(buf, "%02x", *bp++);
            write(fd, buf, buf_len);

            buf_len = sprintf(buf, " ");
            write(fd, buf, buf_len);
        } else
            buf_len = sprintf(buf, "%02x ", *bp++);
            write(fd, buf, buf_len);
        }
        for (indx=dcnt; indx < 16; indx++)
        {
            buf_len = sprintf(buf, "   ");
            write(fd, buf, buf_len);
        }
        buf_len = sprintf(buf, "    ");
        write(fd, buf, buf_len);

        for (indx=0, bp=bufaddr; indx < dcnt; indx++) {
            ch = *bp++ & 0xff;
            buf_len = sprintf(buf, "%c", ((ch < A_SPACE || ch > A_TILDE) ? A_DOT : ch));
            write(fd, buf, buf_len);
        }
        buf_len = sprintf(buf, "\n");
        write(fd, buf, buf_len);
    }
    buf_len = sprintf(buf, "# ------------------------------------------------------------------------------\n\n");
    write(fd, buf, buf_len);
    close(fd);
}

void
_GetDTime(int *dt, int *ctm, int val)
{
    struct timeval tp;
    struct tm   *n_tm;
    int msec;

    *dt = *ctm = 0;
    if(gettimeofday(&tp, NULL) < 0)
        return;
    msec = tp.tv_usec / 1000;
    n_tm = (struct tm *)localtime(&tp.tv_sec);

    *dt = (n_tm->tm_year+TM_YEAR_BASE) * 10000 +
            (n_tm->tm_mon+1) * 100 + n_tm->tm_mday;

    if(val == 10000)    /* get_current-micro-time */
        *ctm = (n_tm->tm_min - (n_tm->tm_min / 10 * 10)) * 100000000 +
                        n_tm->tm_sec * 1000000 + tp.tv_usec;
    else if(val == 1000)
        *ctm = n_tm->tm_hour * 10000000 +
                n_tm->tm_min * 100000 + n_tm->tm_sec * 1000 + msec;
    else if(val == 100) /* get-current-millisec-time */
        *ctm = n_tm->tm_hour * 1000000 +
                n_tm->tm_min * 10000 + n_tm->tm_sec * 100 + msec/10;
    else    /* get-current-sec-time */
        *ctm = n_tm->tm_hour * 10000 + n_tm->tm_min * 100 + n_tm->tm_sec;

}
