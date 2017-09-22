all: server client
server: server.cpp 
	g++ -o server server.cpp 

client: client.cpp 
	g++ -o client client.cpp


clean:
	rm -rf *.o
