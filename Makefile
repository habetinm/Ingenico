REQ_RECORD_CNT=100
REQ_CLIENT_CNT=10

app: main.c data.h client.o logger.o
	gcc -DCLIENT_CNT=$(REQ_CLIENT_CNT) -DRECORD_CNT=$(REQ_RECORD_CNT) client.o logger.o main.c -o app
	
client.o: client.c client.h data.h
	gcc -c -DRECORD_CNT=$(REQ_RECORD_CNT) client.c
	
logger.o: logger.c logger.h data.h
	gcc -c -DRECORD_CNT=$(REQ_RECORD_CNT) logger.c
	
.PHONY: clean
clean:
	rm -f main.o logger.o client.o a.out app log.txt

