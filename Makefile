CFLAGS = -l sqlite3 -lfcgi

all: main.o
	gcc main.o -o main.cgi ${CFLAGS}
	kill $(shell cat main.pid)
	spawn-fcgi -a 127.0.0.1 -p 5000 -f ${CURDIR}/main.cgi -P main.pid

main.o: main.c routers.h utils.h
	gcc -c main.c

clean:
	 rm ./*.o

init:
	gcc main.o -o main.cgi ${CFLAGS}
	spawn-fcgi -a 127.0.0.1 -p 5000 -f ${CURDIR}/main.cgi -P main.pid

