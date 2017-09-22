### This project contains a TCP Echo Server and Client
 
* This project is written in C/C++.

* The IDE is cloud 9.

* Hong Zhuang implements the server program, readline() function.
* Ying Wang implements the client program, writen() function, makefile, and conducted testing.

* How to run:
    * Run make to generate executable files for the project, client and server. Run make clean to remove all the 
object code from the directory. 
    * To start the server, type the following command: ./server PORTNUMBER
    * To start the client, type the following command: ./client IP PORTNUMBER
    * In this project, Port number is 12345, and IP address is 127.0.0.1

* Server side:
    * Server side mainly consists of socket(), bind(), listen(), accept(), fork(),readline() and writen() function.
    * First create socket, and then bind the socket to the assigned IP address and port, and then listen to the port, when client connects 
to the server, accept it and then create a child process to receive and send message back to client.

* Client side:
    * Client side mainly consists of socket(), connect(), writen() and readline().
    * First create socket, then connect the client to server, then using the fgets() to get characters from standard input, and then 
send the data to server, finally receive the echoed message from server and display it on screen.


*Errata:
Server side: socket create error, bind error, litsen error, accept client error, receive and send message error.
Client side: connect error, send and receive message error