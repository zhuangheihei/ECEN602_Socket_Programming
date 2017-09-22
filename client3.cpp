//system header
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/un.h>
#include <sstream>
#include <stdlib.h>
#include <stdio_ext.h>
void  __fpurge(FILE *stream);
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

//readline 
int readline(int socket, char* buf, int n){
    char str[str_size + 1];
    int len;
    int bytesRead;
    char *b;
    char c;
    
    if (n <= 0 || buf == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    b = buf;
    len = 0;
    
    while(true){
        bytesRead = (int)read(socket, &c, 1);
        if(bytesRead == -1){
            if(errno == EINTR){ //EINTR, do it again.
                continue;
            }else {
                return -1;
            }
            //Check for EOF
        }else if(bytesRead == 0){
            if (len == 0)           // No bytes read, return 0 
                return 0;
            else                    //Some bytes read, end with '\0'
                break;

        } else {                    // bytesRead must be 1  
            if (len < n - 1) {      // Discard > (n - 1) bytes 
                len++;
                *b = c;
                b++;
            }
            if(c==EOF) return -1;
            if (c == '\n')
                break;
        }
    }

    *b = '\0';
    return len;
}


int main(){
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(12345);
    server_address.sin_addr.s_addr= inet_addr("127.0.0.1");
    
    char bufSend[str_size+1];
    char bufRecv[str_size+1];
    
    memset(bufSend,0,sizeof(bufSend));
    memset(bufRecv,0,sizeof(bufRecv));
    
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
    cout<<"Input a String (or Control+D twice to stop the connection)! "<< endl;
    cout << "The maximum length is " << str_size - 1 << endl;
    
    while(fgets(bufSend, sizeof(bufSend), stdin)){
        //If user want to terminate the client after type in message, press ctrl + D twice.
        if(feof(stdin)){
            cout << '\n' << "EOF detected, the client will be terminated and not send message." << endl;
            break;
        }
        bufSend[str_size] = '\0';
        if(strlen(bufSend) == sizeof(bufSend) -1 && bufSend[str_size-1] != '\n') {
            cout << "Error: Input size exceed the maximum capacity." << endl;
            cout << "Please type again. The maximum length is " << str_size - 1 << endl;
            //Clear the stdin buffer
            __fpurge(stdin);
            continue;
        }else{
        
            int write_success=writen(network_socket,bufSend,sizeof(bufSend));
            
            if(write_success<0){
                cout<<"No sending now!"<<endl;
                break;
            }
            
            readline(network_socket, bufRecv,str_size);
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
    }
    close(network_socket);
    cout<<"You shut off the connection!";
    
}