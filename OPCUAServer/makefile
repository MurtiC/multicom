server: ./open62541/open62541.o server.o
	gcc ./open62541/open62541.o server.o -o server

./open62541/open62541.o: ./open62541/open62541.c
	gcc -c -std=c99 ./open62541/open62541.c -o ./open62541/open62541.o

server.o: server.c
	gcc -c server.c

clean:
	rm *.o server

run: server
	./server

watch: server
	nodemon --exec ./server

runner:
	make -B -j 1 -o server.o server.o && make run