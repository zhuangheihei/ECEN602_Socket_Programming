//system header
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/un.h>
#include <sstream>
#include <stdlib.h>

//network header
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

using namespace std;
#define str_size 10

//writen function
int writen(int socket_fd, char* buf, int N){
    int num_left=N;
    int num_written;
    char* ptr=buf;
    while(num_left>0){
        num_written=write(socket_fd,ptr,num_left);
        if(num_written<=0){
            if(num_written<0&&errno==EINTR){
                num_written=0;
            }
            else{
                return -1;
            }
        }
        num_left-=num_written;
        ptr+=num_written;
    }
    return N;
}

//readline function
int readline(int socket, char* pRead)
{
    char c = '0';
    int status = 0;
    int i = 0;

    while(true)
    {
        status = read(socket, &c, 1);

        if(status < 0){
            if(errno == EINTR){
                continue;
            }
                //socket error
                return -1;
        }
        else if(status == 0)
        {
            //EOF and return;
            return -1;
        }
        else if(status > 0)
        {
            pRead[i] = c;

            i++;

            if(c == '\n' || c == '\r\n')
            {
                break;
            }
        }
    }
    return i;
}


int main(){
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(12345);
    server_address.sin_addr.s_addr= inet_addr("127.0.0.1");
    
    char bufSend[str_size+1];
    char bufRecv[str_size+1];
    
        //create a socket
        int network_socket=socket(AF_INET,SOCK_STREAM,0);
        
        //call connect
        int connection_status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
        if(connection_status<0){
            cout<<"There is an error when connecting to server!"<<endl;
            cout << "Error number " << (int) errno << endl;
            exit(0);
        }
        cout << "Server connected!" << endl;
        
        //input
        cout<<"Input a String (or Control+D to stop the connection)! "<<endl;
    
    while(fgets(bufSend, sizeof(bufSend) + 1, stdin)){
        
        // call send() to send data to server;
        // int send_success=send(network_socket, bufSend, sizeof(bufSend), 0);
        int write_success=writen(network_socket,bufSend,sizeof(bufSend));
        
        if(write_success<0){
            cout<<"No sending now!"<<endl;
            break;
        }
        //call read to receive data from server
    
        //read(network_socket, bufRecv, sizeof(bufRecv));
        
        readline(network_socket, bufRecv);
        cout<<"Echoed message from server: " << bufRecv <<endl;
        
        //reset buffers
        memset(bufSend,0,sizeof(bufSend));
        memset(bufRecv,0,sizeof(bufRecv));
        
        //close socket
        close(network_socket);
        
        //create a socket
        int network_socket=socket(AF_INET,SOCK_STREAM,0);
        
        //call connect
        int connection_status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
        if(connection_status<0){
            cout<<"There is an error when connecting to server!"<<endl;
            cout << "Error number " << (int) errno << endl;
            exit(0);
        }
        //cout << "Server connected!" << endl;
        
        //input
        cout<<"Input a String (or Control+D to shut off the connection)! "<<endl;
        
    }
    close(network_socket);
    cout<<"You shut off the connection!";
    
}