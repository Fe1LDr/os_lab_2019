CC=gcc
CFLAGS=-I. # для подключения библиотек

objects = tcp udp

all: $(objects)

tcp:
	$(CC) -o tcpc tcpclient.c -pthread $(CFALGS)
	$(CC) -o tcps tcpserver.c -pthread $(CFALGS)
	./tcps &
	./tcpc 127.0.0.1 10050

udp:
	$(CC) -o udpc tcpclient.c -pthread $(CFALGS)
	$(CC) -o udps tcpserver.c -pthread $(CFALGS)
	./udps &
	./udpc 127.0.0.1 10050

clean :
	rm tcpc tcps udpc udps
	fuser -k 10050/tcp
	fuser -k 10050/udp
