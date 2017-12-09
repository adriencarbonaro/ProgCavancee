/*
    C ECHO client example using sockets
    MAJ : 29/11/17 16:07
*/
#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr


/**************/
/* PROTOTYPES */
/**************/
char* sendConnectMessage(char* Nom);

/********/
/* MAIN */
/********/
int main(int argc , char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char /*message[1000] ,*/ server_reply[2000];
    
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

    char* message;
        
        message = sendConnectMessage("Paul");
        printf("taille message après = %d\n", strlen(message));  
        int i=0;
        for(i=0; i<10; i++)
        {
            printf("Caractère %d: %x\n", i, message[i]);
        }

    //keep communicating with server
    while(1) {
	
        /*
        message[10] = (message[2]+message[3] // CheckSum
            +message[4]+message[5]+message[6]+message[7]+message[8]+ message[9]) & 0xff;
        */


        //-------------------------------------------//
        // Send some data
        //-------------------------------------------//
        // write(sock , message , strlen(message))
        // sock : socket used to write data
        // message: buffer containing data to send
        // strlen(message) : number of bytes to send 
        //-------------------------------------------//
        if(write(sock , message , strlen(message) , 0) < 0) {
            printf("WRITE");
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
            printf("FAIL");
            puts("read failed");
            break;
        }
    
        printf("Server reply : %s\n\n", server_reply);
        memset(server_reply, '\0', sizeof(server_reply));
    }

    close(sock);
    return 0;
}


char* sendConnectMessage(char* Nom){
    unsigned int length = strlen(Nom);
	printf("Nom = %s\n", Nom);
    printf("Taille Nom = %d\n", length);
	int i=0;
    char *message;

	message[0] = 0x55; // Syncro
    message[1] = length+1; // Length
    message[2] = 0x01; // Connect
    message[3] = length; // Name Length
	for(i=0; i<length; i++)
	{
		printf("Caractère n°%d = %c\n", i, Nom[i]);
		message[i+4] = Nom[i];
    }
    // 0x55  0
    // 5     1
    // 0x01  2
    // 4     3
    // P     4
    // a     5
    // u     6
    // l     7
    // CheckCRC 8

    // 0x55  0
    // 7     1
    // 0x01  2
    // 6     3
    // A     4
    // d     5
    // r     6
    // i     7
    // e     8
    // n     9
    // CheckCRC 10
    for(i=2; i<length+3; i++)
    {
            message[length+4] += message[i];
    }

    message[length+4] = message[length+4] & 0xff;
    /*
    message[length+4] = (message[2]+message[3] // CheckSum
        +message[4]+message[5]+message[6]+message[7]+message[8]+ message[9]) & 0xff;
        */
        printf("taille message avant : %d\n", strlen(message));
    return message;

}
