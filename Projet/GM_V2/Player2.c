/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr

int main(int argc , char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    //-------------------------------------------------------------------------//
    // Create socket
    //-------------------------------------------------------------------------//    
    // int socket(int domain, int type, int protocol)
    //-------------------------------------------------------------------------//    
    // AF_INET pour utiliser de l'IPv4
    //-------------------------------------------------------------------------//
    // SOCK_STREAM = TCP
    // = Support de dialogue garantissant l'intégrité,
    // fournissant un flux de données binaires,
    // et intégrant un mécanisme pour les transmissions de données hors-bande.
    //-------------------------------------------------------------------------//
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Define the socket remote address : 127.0.0.1:8888
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    //keep communicating with server
    while(1) {
	

        char *message;
        message[0] = 0x55; // Syncro
        message[1] = 5; // Length
        message[2] = 0x01; // Connect
        message[3] = 4; // Name Length
        message[4] = 'P'; // Name
        message[5] = 'a'; // Name
        message[6] = 'u'; // Name
        message[7] = 'l'; // Name
        message[8] = (message[2]+message[3] // CheckSum
            +message[4]+message[5]+message[6]+message[7]) & 0xff;



        //-------------------------------------------//
        // Send some data
        //-------------------------------------------//
        // write(sock , message , strlen(message))
        // sock : socket used to write data
        // message: buffer containing data to send
        // strlen(message) : number of bytes to send 
        //-------------------------------------------//
        if(write(sock , message , strlen(message) , 0) < 0) {
            puts("write failed");
            return 1;
        }

        //-------------------------------------------------------//
        // Receive a reply from the server
        //-------------------------------------------------------//
        // read(sock, server_reply, nbBytesRead)
        // sock : socket used to read data
        // server_reply: buffer used to put data coming
        // from socket
        // nbBytesRead : number of bytes to read from the socket
        //-------------------------------------------------------//
        if(read(sock , server_reply , 2000 ) < 0) {
            puts("read failed");
            break;
        }
    
        printf("Server reply : %s\n\n", server_reply);
        memset(server_reply, '\0', sizeof(server_reply));
    }

    close(sock);
    return 0;
}
