app: main.c data.h client.o logger.o
	gcc -D_REENTRANT -pthread -DTHREAD_CNT=5 -DRECORD_CNT=200 client.o logger.o main.c -o app
	
client.o: client.c client.h data.h
	gcc -c -D_REENTRANT -pthread client.c
	
logger.o: logger.c logger.h data.h
	gcc -c -D_REENTRANT -pthread logger.c
	
.PHONY: clean
clean:
	rm -f logger.o client.o a.out app log.txt

