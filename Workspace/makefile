watch: server
	nodemon --exec "make runner || exit 1"


runReader:
	java -jar GitHub.jar

runOPC:
	./server

launch: runReader runOPC


run:
	make -j launch

copyServer:
	cp /home/multi/opcua/TempTag/OPCUAServer/server /home/multi/opcua/TempTag/Workspace/server
	chmod +x server

BuildServer: 
	/OPCUAServer/make runner