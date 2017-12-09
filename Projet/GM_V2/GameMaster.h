/***********
    NE PAS ENLEVER CE HEADER !!

    PROJET REVERSI

    OWNERS : Paul SÉRAGÉ, Victor LAM, Alexis DE LAUNAY, & PAS TOI

    Copyright © NE PAS ENLEVER CE HEADER SOUS PEINE DE POURSUITES JUDICIAIRES!


*********/




#ifndef GAMEMASTER_H_INCLUDED
#define GAMEMASTER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 1024
#define PORT 8888
#define MAX_CLIENT 2
#define BLACK_PLAYER 0
#define WHITE_PLAYER 1
#define CONTINUOUS 0
#define STEP_BY_STEP 1
#define EMPTY_SQUARE -1
#define BLACK_PAWN 0
#define WHITE_PAWN 1

#define HAUT 0
#define BAS 1
#define GAUCHE 2
#define DROITE 3
#define DIAGHG 4
#define DIAGHD 5
#define DIAGBG 6
#define DIAGBD 7

#ifdef WIN32 /* si vous êtes sous Windows */

#include <Windows.h>

#elif defined (linux) /* si vous êtes sous Linux */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif


clock_t temps;

typedef struct
{
   int port;
   char name;
}Client;

typedef struct
{
    SOCKET sock;
    char name[BUFFER_SIZE];
    int score;
    int time_reflexion;
    int qui;
}Cliente;

 void initia (void);
 void fin (void);
 //int init_connection(Client qui);
 int init_connection();
 int lire (SOCKET sock, unsigned char* client);
 void ecrire(SOCKET sock, const unsigned char* ecrit, int octets_a_transmit);
 //void corps (Client qui);
 void application();

#endif // GAMEMASTER_H_INCLUDED
