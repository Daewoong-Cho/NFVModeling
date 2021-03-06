 CREATE TABLE SERVER_LOG(
   SEQ          INT(10) NOT NULL AUTO_INCREAMENT,
   TIME_STAMP   INT(10) NOT NULL,
   SERVER_KEY   INT(10) NOT NULL,
   T_CPU        INT(10) NOT NULL,
   T_MEM        INT(10) NOT NULL,
   T_DISK       INT(10) NOT NULL,
   A_CPU        INT(10) NOT NULL,
   A_MEM        INT(10) NOT NULL,
   A_DISK       INT(10) NOT NULL,
   CPU_USAGE    DOUBLE NOT NULL,
   MEM_USAGE    DOUBLE NOT NULL,
   DISK_USAGE   DOUBLE NOT NULL,
   P_SERVER_KEY INT(10),
   JOB_TYPE     CHAR(1),
   PRIMARY KEY(SEQ)
 );
 
  CREATE TABLE   QUEUE_LOG(
   SEQ              INT(10) NOT NULL AUTO_INCREAMENT,
   TIME_STAMP       INT(10) NOT NULL,
   QUEUE_KEY        INT(10) NOT NULL,
   SIZE             INT(10) NOT NULL,
   AVAIL            INT(10) NOT NULL,
   QUEUE_USAGE      DOUBLE  NOT NULL,
   PROCESSED_CNT    INT(10) NOT NULL,
   WAIT_CNT         INT(10) NOT NULL,
   SERVER_KEY       INT(10) NOT NULL,
   PRIMARY KEY(SEQ)
 );
 
  CREATE TABLE   VNF_LOG(
   SEQ              INT(10) NOT NULL AUTO_INCREAMENT,
   TIME_STAMP       INT(10) NOT NULL,
   VNF_KEY          INT(10) NOT NULL,
   PROCESSED_CNT    INT(10) NOT NULL,
   THROUGHPUT       INT(10) NOT NULL,
   AVAIL            INT(10) NOT NULL,
   VNF_USAGE        DOUBLE  NOT NULL,
   SERVER_KEY       INT(10) NOT NULL,
   VNF_TYPE_KEY     INT(10) NOT NULL,
   IN_QUEUE_KEY     INT(10) NOT NULL,
   OUT_QUEUE_KEY    INT(10) NOT NULL,
   PRIMARY KEY(SEQ)
 );
 
  CREATE TABLE   PACKET_LOG(
   SEQ              INT(10) NOT NULL AUTO_INCREAMENT,
   QUEUE_KEY        INT(10) NOT NULL,
   PACKET_SEQ       INT(10) NOT NULL,
   WAIT_TIME        INT(10) NOT NULL,
   PROCESS_TIME     INT(10) NOT NULL,
   SERVICE_TIME     INT(10) NOT NULL,
   PRIMARY KEY(SEQ)
 );