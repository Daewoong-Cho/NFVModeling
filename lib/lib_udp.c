#include	"common.h"

struct      sockaddr_in     NetAddr;
struct      sockaddr_in     NetAddrCli[100];

int open_mcast(char *address, int port_no)
{
    struct  sockaddr_in mcast_group;
    struct  ip_mreq ipmreq;
    char    Iuttl;
    int     sockfd,rval;
    unsigned int set = 1;
    char *opt;


    /* Creat UDP Socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return (-1);
    }

	/* Allocated Multicast Group IP */
    memset(&mcast_group, 0, sizeof(mcast_group));
    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(port_no);
    mcast_group.sin_addr.s_addr = inet_addr(INADDR_ANY);

    rval = bind(sockfd, (struct sockaddr *)&mcast_group, sizeof(mcast_group));
    if (rval < 0) {
        close(sockfd);
        return(-3);
    }

    /* Defined ip_mreq Structure */
    memset(&ipmreq, 0x00, sizeof(ipmreq));
    ipmreq.imr_multiaddr.s_addr = inet_addr(address);
    ipmreq.imr_interface.s_addr = htonl(INADDR_ANY);

    /* Join Multicast Group */
    rval = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmreq, sizeof(ipmreq));

    if (rval < 0) {
        close(sockfd);
        return (-2);
    }


#if 0
    /* SOCKET REUSEADDR*/
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(set));
#endif


    return(sockfd);
}

int open_mcast_sender(char *address, int port_no)
{
    int     sockfd,rval;
    unsigned int set = 1;
	int		multi_TTL = 64;
	struct	in_addr	ltLocalIF;
	int		liTtl = 10;

	int		idx = 0;

	memset(&NetAddrCli[idx], 0x00, sizeof(NetAddrCli[idx]));

	NetAddrCli[idx].sin_family = AF_INET;
	NetAddrCli[idx].sin_port = htons(port_no);
	inet_aton(address, &NetAddrCli[idx].sin_addr);

    /* Creat UDP Socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return (-1);
    }

#if 0
    /* Creat UDP Socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return (-1);
    }

	memset(&NetAddrCli[idx], 0x00, sizeof(NetAddrCli[idx]));

	NetAddrCli[idx].sin_family = AF_INET;
	NetAddrCli[idx].sin_addr.s_addr = htonl(INADDR_ANY);
	//NetAddrCli[idx].sin_port = htons(0);
	NetAddrCli[idx].sin_port = htons(port_no);

#if 0
    /* Defined ip_mreq Structure */
    memset(&ipmreq, 0x00, sizeof(ipmreq));
    ipmreq.imr_multiaddr.s_addr = inet_addr(address);
    ipmreq.imr_interface.s_addr = htonl(INADDR_ANY);
#endif


	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&set, sizeof(set));

	memset((void *)&ltLocalIF, 0x00, sizeof(ltLocalIF));

	ltLocalIF.s_addr = inet_addr(address);

	setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ltLocalIF, sizeof(ltLocalIF));

	rval = setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&liTtl, sizeof(liTtl)); 

    if (rval < 0) {
        close(sockfd);
        return (-2);
    }

#if 0
	setsockopt(liSockFd, SOL_SOCKET, SO_SNDBUF, (void*)&piBufLen, sizeof(piBufLen);
#endif

    rval = bind(sockfd, (struct sockaddr *)&NetAddrCli[idx], sizeof(NetAddrCli[idx]));
    if (rval < 0) {
        close(sockfd);
        return(-3);
    }

#endif
    return(sockfd);
}

int open_udp(ipaddr, port_no)
char	*ipaddr;
int		port_no;
{
    int    rval, sock_fd;

    errno = 0;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error (%d : %s)\n", errno, strerror(errno));
        return(-1);
    }

    memset(&NetAddr, 0, sizeof(NetAddr));

    NetAddr.sin_family = AF_INET;
    //NetAddr.sin_addr.s_addr = htonl(ipaddr);
    NetAddr.sin_addr.s_addr = inet_addr(ipaddr);
    NetAddr.sin_port = htons(port_no);

    rval = bind(sock_fd, (struct sockaddr *)&NetAddr, sizeof(NetAddr));
    if (rval < 0)
    {
        close(sock_fd);
        printf( "bind error (%d : %s)\n", errno, strerror(errno));
        return(-2);
    }
    return(sock_fd);
}

int recv_udp(int sock_fd, char* result_buffer)
{
    int     rcv_len, from_len;
    struct  sockaddr_in from_addr;

    errno = 0;

    from_len = sizeof(from_addr);
    rcv_len = recvfrom(sock_fd, result_buffer, 8192, 0, (struct sockaddr *)&from_addr, (int *)&from_len);
    if (rcv_len == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            printf("udp recvfrom : EWOULDBLOCK (Operation would block)\n");
        }
        else if (errno == EINTR)
        {
            printf("udp recvfrom : EINTR (Interrupt system call)\n");
        }
        else
        {
            printf("socket recv error (%d : %s)...\n", errno, strerror(errno));
        }
    }

    return(rcv_len);
}

int send_udp(idx, sock_fd, send_buff, snd_size)
short   idx;
int     sock_fd;
char    send_buff[8192];
int     snd_size;
{
    int     size, tot_snd_size, error_loop;
    char    send_buffer[8192];

    errno = 0;
    error_loop = 0;
    tot_snd_size = 0;

    memset(send_buffer, 0x00, sizeof(send_buffer));
    memcpy(&send_buffer[0], &send_buff[0], snd_size);

    do
    {
        size = sendto(sock_fd, send_buffer, snd_size, 0,
                (struct sockaddr *)&NetAddrCli[idx], sizeof(NetAddrCli[idx]));
        if (size == -1)
        {
            error_loop++;
            if (errno == EWOULDBLOCK)
            {
                printf("socket send EWOULDBLOCK (%d : %s)...\n", errno, strerror(errno));
            }
            else
            {
                printf("socket send error (%d : %s)...\n", errno, strerror(errno));
                break;
            }
        }
        else
        {
            tot_snd_size += snd_size;
        }
    } while (tot_snd_size < snd_size && error_loop < 10);

    return(tot_snd_size);
}

int open_client_udp(idx, address, port_no)
short   idx;
char    *address;
int     port_no;
{
    int    rval = 1, sock_fd;

    errno = 0;
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error (%d)\n", errno);
        return(-1);
    }

    memset(&NetAddrCli[idx], 0, sizeof(NetAddrCli[idx]));

    NetAddrCli[idx].sin_family = AF_INET;
    NetAddrCli[idx].sin_addr.s_addr = inet_addr(address);
    NetAddrCli[idx].sin_port = htons(port_no);

    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, (char *)&rval, sizeof(rval));
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&rval, sizeof(rval));

    return(sock_fd);
}

void close_udp(sock_fd)
int     sock_fd;
{
    close(sock_fd);
}


int join_mcast(int Isockfd, char *addr)
{
    struct  ip_mreq ipmreq;
    char    Iuttl;

    bzero((char *)&ipmreq, sizeof(ipmreq));
    ipmreq.imr_multiaddr.s_addr = inet_addr(addr);
    ipmreq.imr_interface.s_addr = INADDR_ANY;

    if(setsockopt(Isockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                (char *)&ipmreq, sizeof(struct ip_mreq)) < 0) {
        printf("McastJoin setsockopt %s error=%d\n", addr, errno);
        return(-3);
    }
    return(1);
}
