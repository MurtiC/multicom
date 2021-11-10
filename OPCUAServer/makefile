server: ./open62541/open62541.o server.o
	gcc ./open62541/open62541.o server.o -o server

./open62541/open62541.o: ./open62541/open62541.c
	gcc -c -std=c99 ./open62541/open62541.c -o ./open62541/open62541.o

server.o: server.c
	gcc -c server.c

clean:
	rm -f *.o

clearport:
	fuser -k 4840/tcp

run:
	./server

watch: server
	nodemon --exec "make runner || exit 1"



runner: clean server run