include $(HOME)/NFV/env.make.linux

CFLAGS  = -I$(INC_DIR) -g -lm
LFLAGS  = -L$(LIB_DIR)

MY_INCLUDE =

SECSRC  = cfg_load.c proc_daemon.c instance.c placement.c workload.c

all:	cfg_load proc_daemon instance placement workload

cfg_load: cfg_load.o
	$(CC) -o $@ cfg_load.o $(CFLAGS) $(LFLAGS) -ldavid
	mv $@ $(BIN_DIR)/$@
		  
proc_daemon: proc_daemon.o
	$(CC) -o $@ proc_daemon.o $(CFLAGS) $(LFLAGS) -ldavid
	mv	$(BIN_DIR)/$@ $(BIN_DIR)/$@.old
	mv $@ $(BIN_DIR)/$@

instance: instance.o
	$(CC) -o $@ instance.o $(CFLAGS) $(LFLAGS) -ldavid
	mv $@ $(BIN_DIR)/$@

placement: placement.o
	$(CC) -o $@ placement.o $(CFLAGS) $(LFLAGS) -ldavid
	mv $@ $(BIN_DIR)/$@

workload: workload.o
	$(CC) -o $@ workload.o $(CFLAGS) $(LFLAGS) -ldavid
	mv $@ $(BIN_DIR)/$@
clear:
clean:
	rm -f *.o *.a *.so
