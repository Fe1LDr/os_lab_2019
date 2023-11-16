CC=gcc
CFLAGS=-I. # для подключения библиотек
DEFINES = -BUFSIZE
objects = tcp udp

all: $(objects)

tcp:
	$(CC) -o tcpc tcpclient.c -pthread $(CFALGS) -D gerg=100
	$(CC) -o tcps tcpserver.c -pthread $(CFALGS) -D gerg=100 -D serv=10050
	./tcps &
	./tcpc 127.0.0.1 10050

udp:
	$(CC) -o udps udpserver.c -pthread $(CFALGS) -D gerg=1024 -D serv=20002
	$(CC) -o udpc udpclient.c -pthread $(CFALGS) -D gerg=1024 -D serv=20002
	./udps &
	./udpc 127.0.0.1:20002

clean : clean_tcp clean_udp

clean_tcp:
	fuser -k 10050/tcp
	rm tcpc tcps

clean_udp:
	fuser -k udps
	rm udpc udps