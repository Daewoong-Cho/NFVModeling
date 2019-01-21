/*------------------------------------------------------------------------
#	System  : David Trading
#	Module	: socket - create an endpoint for communication (TCP/IP)
#	File	: lib_socket.c
------------------------------------------------------------------------*/

/*------------------------------------------------------------------------
	Header Files
------------------------------------------------------------------------*/
#include	"common.h"

/*************************************************************************
	Function		: . socket - create an endpoint for communication
	Parameters IN	: .
	Parameters OUT	: .
	Return Code		: . int (socket file descriptor: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		creat_socket (void)
/*----------------------------------------------------------------------*/
{
    int		sfd, on=1;
	struct	linger	ling;

    sfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
	  return sfd;
	
	/* for removing socket TIME_WAIT status */
	ling.l_onoff = 1; 
	ling.l_linger = 0;

	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
	setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

    return (sfd);
}	/* End of creat_socket ()	*/

/*************************************************************************
	Function		: . accept a connection on a socket
	Parameters IN	: . p_sfd	: socket file descriptor
	Parameters OUT	: . p_ip	: client IP address
					  . p_port	: client port
	Return Code		: . int (a descriptor for the accepted socket: success,
						-1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		accept_connection (int p_sfd, char *p_ip, u_short *p_port)
/*----------------------------------------------------------------------*/
{
	char				*hostname, *hostip;
	int					rt, on=1;
#if defined __hpux || _AIX
	socklen_t			addrlen;
#else
	size_t				addrlen;
#endif
	struct sockaddr_in	address;
	struct hostent		*hp;

	addrlen = sizeof (struct sockaddr);
	memset ((char *)&address, 0, sizeof (address));

	rt = accept (p_sfd, (struct sockaddr *)&address, &addrlen);
	if (rt < 0)
		return (rt);

	setsockopt(p_sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	hp = gethostbyaddr ((char *)&address.sin_addr,
		sizeof (struct in_addr), address.sin_family);
	hostip = (char *)inet_ntoa (address.sin_addr);
	*p_port = ntohs (address.sin_port);
	sprintf (p_ip, "%s", hostip);

	return (rt);
}	/* End of accept_connection ()	*/

/*************************************************************************
	Function		: . bind a name to a socket
	Parameters IN	: . p_sfd		: file descriptor of the socket to be bound
					  . p_port_no	: port number
	Parameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		bind_socket (int p_sfd, int p_port_no)
/*----------------------------------------------------------------------*/
{
    int					rt;
    struct sockaddr_in	address;

    memset ((char *)&address, 0, sizeof (address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl (INADDR_ANY);
    address.sin_port = htons (p_port_no);

    rt = bind (p_sfd, (struct sockaddr *)&address, sizeof (struct sockaddr));

    return (rt);
}	/* End of bind_socket ()	*/

/*************************************************************************
    Function        : . listen for socket connections
    Parameters IN   : . p_sfd   : file descriptor associated with the socket
    Parameters OUT  : .
    Return Code     : . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int     listen_socket (int p_sfd)
/*----------------------------------------------------------------------*/
{
    int     rt;

    rt = listen (p_sfd, 5);

    return (rt);
}   /* End of listen_socket () */

/*************************************************************************
	Function		: . connect to a socket
	Parameters IN	: . p_sfd		: file descriptor associated with the socket
					  . p_ip_addr	: IP address
					  . p_port_no	: port number
	Prameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		connect_socket (int p_sfd, char *p_ip_addr, int p_port_no)
/*----------------------------------------------------------------------*/
{
	int					rt;
	struct sockaddr_in	address;

	memset ((char *)&address, 0, sizeof (address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr (p_ip_addr);
	address.sin_port = htons (p_port_no);

	rt = connect (p_sfd, (struct sockaddr *)&address, sizeof (struct sockaddr));

	return (rt);
}	/* End of connect_socket ()	*/

/*************************************************************************
	Function		: . connect to a socket
	Parameters IN	: . p_sfd		: file descriptor associated with the socket
					  . p_ip_addr	: IP address
					  . p_port_no	: port number
					  . p_timeout	: alarm timeout
	Prameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		connect_socket_timeout (int p_sfd, char *p_ip_addr, int p_port_no, int p_timeout)
/*----------------------------------------------------------------------*/
{
	int					rt;
	struct sockaddr_in	address;

	memset ((char *)&address, 0, sizeof (address));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr (p_ip_addr);
	address.sin_port = htons (p_port_no);

	alarm (p_timeout);

	rt = connect (p_sfd, (struct sockaddr *)&address, sizeof (struct sockaddr));
	if (rt != 0)
	{
		alarm (0);

		return (-1);
	}

	alarm (0);

	return (rt);
}	/* End of connect_socket_timeout ()	*/

/*************************************************************************
    Function        : . send a message on a socket
    Parameters IN   : . p_sfd   : file descriptor associated with the socket
                      . p_buf   : data buffer
                      . p_len   : data size
    Parameters OUT  : .
    Return Code     : . int (number of bytes sent: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int     send_socket (int p_sfd, char *p_buf, int p_len)
/*----------------------------------------------------------------------*/
{
    int     nleft, nsend, pt, cnt;

    cnt = nleft = p_len;
    pt = 0;

    while (nleft > 0)
    {
        nsend = send (p_sfd, p_buf+pt, nleft, 0);
        if (nsend <= 0)
            return (nsend);

        nleft -= nsend;
        pt += nsend;
        cnt --;

        if (cnt < 0)
            return (0);
    }

    return (p_len - nleft);
}   /* End of send_socket ()  */

/*************************************************************************
	Function		: . receive a message from a socket
	Parameters IN	: . p_sfd	: file descriptor associated with the socket
					  . p_len	: data size
	Parameters OUT	: . p_buf	: data buffer
	Return Code		: . int (number of bytes received: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		recv_socket (int p_sfd, char *p_buf, int p_len)
/*----------------------------------------------------------------------*/
{
	int		nleft, nrecv;

	nleft = p_len;
	
	while (nleft > 0) 
	{
		nrecv = recv (p_sfd, p_buf, nleft, 0);
		if (nrecv < 0) 
			return (nrecv);
		else if (nrecv == 0)
			break;

		nleft -= nrecv;
		p_buf += nrecv;
	}
	
	return (p_len - nleft);
}	/* End of recv_socket ()	*/

/*************************************************************************
	Function		: . select - test file descriptors
	Parameters IN	: . p_sockfd	: file descriptor associated with the socket
	Parameters OUT	: .
	Return Code		: . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int		select_recv (int p_sockfd, char *p_buf, int p_len, int *timeout_flag)
/*----------------------------------------------------------------------*/
{
    int             rt;
    fd_set          read_set;
    struct timeval  timeout;

    FD_ZERO (&read_set);
    FD_SET (p_sockfd, &read_set);
    timeout.tv_sec = TCP_TIME_OUT;
    timeout.tv_usec = 0;
    *timeout_flag = 0;

    rt = select (p_sockfd+1, &read_set, NULL, NULL, &timeout);
    if (rt < 0)
    {
        return (rt);
    }

    if (FD_ISSET (p_sockfd, &read_set))
        return (recv_socket (p_sockfd, p_buf, p_len));

    *timeout_flag = 1;

    return (TIME_OUT_FLAG);
}	/* End of select_send ()	*/
#if 0
{
    int				rt;
    fd_set			readfds, writefds, exceptfds;
    struct timeval	timeout;

    timeout.tv_sec = p_time_s;
    timeout.tv_usec = p_time_ms;

    FD_ZERO (&readfds);
    FD_ZERO (&writefds);
    FD_ZERO (&exceptfds);

    FD_SET (p_sfd, &readfds);
    FD_SET (p_sfd, &writefds);
    FD_SET (p_sfd, &exceptfds);

    rt = select (p_sfd+1, (fd_set *)&readfds,
		(fd_set *)&writefds, (fd_set *)&exceptfds, &timeout);
    if (rt == -1) 
        return (-1);

    return (0);
}	/* End of select_socket ()	*/
#endif

/*************************************************************************
    Function        : . select and send a packet
    Prameters IN    : . p_sfd       : socket file descriptor
                      . p_send      : data buffer
                      . p_length    : data size
    Parameters OUT  : .
    Return Code     : . int (0: success, -1: failure)
*************************************************************************/
/*----------------------------------------------------------------------*/
int     select_send (int p_sfd, char *p_send, int p_length)
/*----------------------------------------------------------------------*/
{
    int             rt;
    fd_set          write_set;
    struct timeval  timeout;

    FD_ZERO (&write_set);
    FD_SET (p_sfd, &write_set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 200000;

    rt = select (p_sfd+1, NULL, &write_set, NULL, &timeout);
    if(rt < 0)
    {
        return (FAIL);
    }

    if (!FD_ISSET (p_sfd, &write_set))
    {
        return (FAIL);
    }

    rt = send_socket (p_sfd, p_send, p_length);
    if (rt <= 0)
    {
        return (FAIL);
    }

    return (SUCCESS);
}   /* End of select_send ()    */

/*************************************************************************
	End of Program (lib_socket.c)
*************************************************************************/
