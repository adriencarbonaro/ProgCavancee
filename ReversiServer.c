/******************
 * 
 * Reversi Server
 * 
 *******************/
#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write

int main(int argc , char *argv[]) {
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    unsigned int q = 0;

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
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //-------------------------------------------------------------------//
    // Bind
    //-------------------------------------------------------------------//
    // bind(socket_desc,(struct sockaddr *)&server , sizeof(server))
    //-------------------------------------------------------------------//
    // Set the socket parameter and affect them to the socket descriptor
    //-------------------------------------------------------------------//
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //--------------------------------------------------------//
    // Listen
    //--------------------------------------------------------//
    // int listen(int s, int backlog);
    //--------------------------------------------------------//
    // Wait for incoming sockets (clients). 3 sockets can be
    // opened simultaneously
    //--------------------------------------------------------//
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //---------------------------------------------------------------------//
    // Accept connection from an incoming client
    //---------------------------------------------------------------------//
    // int accept(int sock, struct sockaddr *adresse, socklen_t *longueur);
    //---------------------------------------------------------------------//
    // Accept the incoming connection and
    // affect it to the new descriptor « client_sock »
    // « client_sock » will be used to communicate with the new client
    // « server » can be used to wait another incoming connection
    //---------------------------------------------------------------------//
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //-------------------------------------------------------//
    // Receive a reply from the server
    //-------------------------------------------------------//
    // read(sock, server_reply, nbBytesRead)
    // sock : socket used to read data
    // server_reply: buffer used to put data coming
    // from socket
    // nbBytesRead : number of bytes to read from the socket
    //-------------------------------------------------------//
    while((read_size = read(client_sock , client_message , 2000)) > 0 && q != 1) {
        if(strcmp(client_message, "quit") == 0){
             puts("Client quitted");
             return 0;
        }
        // display the message
        puts(client_message);
        // Send the message back to client
        write(client_sock , client_message , strlen(client_message));
        // Remet le buffer à 0
        memset(client_message, '\0', sizeof(client_message));
    }

    if(read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    }
    
    else if(read_size == -1) {
        perror("recv failed");
    }

    return 0;
}