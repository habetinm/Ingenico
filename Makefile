app: main.c data.h client.o logger.o
	gcc -D_REENTRANT -pthread -DTHREAD_CNT=5 client.o logger.o main.c -o app

client.o: client.c client.h
	gcc -c -DREENTRANT -pthread client.c
	
logger.o: logger.c logger.h
	gcc -c -DREENTRANT -pthread logger.c
	
.PHONY: clean
clean:
	rm -f logger.o client.o a.out app log.txt

