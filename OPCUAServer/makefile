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

runServer:
	./server

watch: server
	nodemon --exec "make run || exit 1"



run: clean server runServer