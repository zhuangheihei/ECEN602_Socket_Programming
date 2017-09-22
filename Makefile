all: server client1 client2 client3
server: server.cpp 
	g++ -o server server.cpp 

client1: client1.cpp 
	g++ -o client1 client1.cpp
	
client2: client2.cpp 
	g++ -o client2 client2.cpp
	
client3: client3.cpp 
	g++ -o client3 client3.cpp


clean:
	rm -rf *.o
