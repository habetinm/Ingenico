REQ_RECORD_CNT=100000
REQ_CLIENT_CNT=10

all: app

app: main.c data.h client.o logger.o
	gcc -D_REENTRANT -pthread -DCLIENT_CNT=$(REQ_CLIENT_CNT) -DRECORD_CNT=$(REQ_RECORD_CNT) client.o logger.o main.c -o app
	
client.o: client.c client.h data.h
	gcc -c -D_REENTRANT -pthread -DRECORD_CNT=$(REQ_RECORD_CNT) client.c
	
logger.o: logger.c logger.h data.h
	gcc -c -D_REENTRANT -pthread -DRECORD_CNT=$(REQ_RECORD_CNT) logger.c
	
.PHONY: clean
clean:
	rm -f main.o logger.o client.o a.out app log.txt log*.txt
