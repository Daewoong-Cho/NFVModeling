include $(HOME)/NFVModeling/env.make.linux

CFLAGS  = -I$(INC_DIR) -I/usr/include/mysql -g 
LFLAGS  = -L$(LIB_DIR) -ljson-c -lnfv -lmysqlclient -lpthread

MY_INCLUDE =

SECSRC  =  vnf.c vim.c packetgenerator.c

all:	vnf vim packetgenerator

vnf: vnf.o
	$(CC) -o $@ vnf.o $(CFLAGS) $(LFLAGS)
	mv $@ $(BIN_DIR)/$@
		  
vim: vim.o
	$(CC) -o $@ vim.o $(CFLAGS) $(LFLAGS)
	mv $@ $(BIN_DIR)/$@

packetgenerator: packetgenerator.o
	$(CC) -o $@ packetgenerator.o $(CFLAGS) $(LFLAGS)
	mv $@ $(BIN_DIR)/$@
clear:
clean:
	rm -f *.o *.a *.so
