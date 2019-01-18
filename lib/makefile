include $(HOME)/env.make.linux

MY_INCLUDE = 

MY_NAME_1  = libnfv

CFLAGS  = -I$(INC_DIR) -g -l/usr/include/mysql
LFLAGS  = -L$(LIB_DIR) -L/usr/lib64/mysql -lmysqlclient

LIBSTL1 = $(MY_NAME_1).a

SECSRC  = lib_server.c lib_queue.c lib_vnf.c lib_packet.c lib_ipc.c lib_socket.c lib_errlog.c lib_udp.c lib_fep.c

all:        $(LIBSTL1)

$(LIBSTL1): $(LIBSTL1)(lib_server.o lib_queue.o lib_vnf.o lib_packet.o lib_ipc.o lib_socket.o lib_errlog.o lib_udp.o lib_fep.o)
	if [ -s /usr/bin/ranlib ]; then ranlib $(LIBSTL1); fi
        
clear:
clean:
	rm -f *.o *.a *.so
