include $(HOME)/NFVModeling/env.make.linux

MY_INCLUDE = 

MY_NAME_1  = libnfv

CFLAGS  = -I$(INC_DIR) -I/usr/include/mysql
#LFLAGS  = -L$(LIB_DIR) -L/usr/local/lib/mysql -lmysqlclient

LIBSTL1 = $(MY_NAME_1).a

SECSRC  = lib_nfv.c lib_ipc.c lib_socket.c lib_errlog.c lib_udp.c lib_fep.c

all:        $(LIBSTL1)

$(LIBSTL1): $(LIBSTL1)(lib_nfv.o lib_ipc.o lib_socket.o lib_errlog.o lib_udp.o lib_fep.o)
	if [ -s /usr/bin/ranlib ]; then ranlib $(LIBSTL1); fi
        
clear:
clean:
	rm -f *.o *.a *.so
