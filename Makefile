app: main.c data.h client.o logger.o
	gcc -DCLIENT_CNT=10 -DRECORD_CNT=100 client.o logger.o main.c -o app
	
client.o: client.c client.h data.h
	gcc -c client.c
	
logger.o: logger.c logger.h data.h
	gcc -c logger.c
	
.PHONY: clean
clean:
	rm -f main.o logger.o client.o a.out app log.txt

