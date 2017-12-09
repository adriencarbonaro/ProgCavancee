/***********
    NE PAS ENLEVER CE HEADER !!

    PROJET REVERSI

    OWNERS : Paul SÉRAGÉ, Victor LAM, Alexis DE LAUNAY, & PAS TOI

    Copyright © NE PAS ENLEVER CE HEADER SOUS PEINE DE POURSUITES JUDICIAIRES!


*********/


#include "GameMaster.h"

int taille = 8;
int coups_joues = 0; /// ATTENTION !!!! A REMETTRE A 0 EN DEBUT DE PARTIE

void initia (void)
{
#ifdef WIN32
    WSADATA Data;
    int err = WSAStartup (MAKEWORD (2, 2), &Data);
    if (err < 0)
    {
        puts ("WSAStartup failed !");
        exit (EXIT_FAILURE);
    }
#endif
}

void fin (void)
{
#ifdef WIN32
    WSACleanup();
#endif
}


int init_connection()
{
    SOCKET sock_server;

    SOCKADDR_IN server;

    // Création du socket
    sock_server = socket (AF_INET, SOCK_STREAM, 0);

    if (sock_server == -INVALID_SOCKET)
    {
        printf ("Could not create socket");
        exit (EXIT_FAILURE);
    }

    printf ("Server socket created\n");

    //Préparons la structure sockaddr_in
    server.sin_addr.s_addr = htons (INADDR_ANY);
    server.sin_port = htons (PORT);
    server.sin_family = AF_INET;

    //Lions
    if (bind (sock_server, (SOCKADDR *) &server, sizeof (server) ) == SOCKET_ERROR)
    {
        perror ("Socket()");
        exit (EXIT_FAILURE);
    }

    //Ecoutons

    if (listen (sock_server, MAX_CLIENT) == SOCKET_ERROR)
    {
        perror ("listen()");
        exit (EXIT_FAILURE);
    }

    return sock_server;

}

int lire (SOCKET sock, unsigned char* message)
{
    int n = 0;
    if ( (n = recv (sock, message, BUFFER_SIZE - 1, 0) ) < 0)
    {
        printf ("Un probleme incongru est apparu\n");
        //exit(EXIT_FAILURE);
        //return-2;
    }
    message[n] = '\0';
    return n;
}

void ecrire (SOCKET sock, const unsigned char* ecrit, int octets_a_transmit)
{
    if (send (sock, ecrit, octets_a_transmit, 0) < 0)
    {
        printf ("Probleme d'ecriture");
        exit (EXIT_FAILURE);
    }
}

// On supprime un PLAYER plus précisément : la suppresion du controler se fait d'une autre façon ...
void suppression_client (Cliente *clients, int a_enlever, int *actual)
{
    // On retire le client en question de notre liste
    memmove (clients + a_enlever, clients + a_enlever + 1, (*actual - a_enlever - 1) * sizeof (Cliente) );
    (*actual) --; // vu qu'on en a retiré un
}


/// Attention, ne concerne que les players pour nous, pas le controler puisque n'est pas situé dans la liste Clients mais à part
void broadcast (Cliente *clients, Cliente envoyeur, int actual, const unsigned char *buffer, unsigned char from_server)
{
    int i = 0;
    unsigned char message[BUFFER_SIZE];
    message[0] = 0;
    for (i = 0 ; i < actual ; i++)
    {
        // On envoie à tout le monde sauf à l'envoyeur
        if (envoyeur.sock != clients[i].sock)
        {
            // Pour customizer notre envoi
            // On rentre ici si notre envoyeur ne s'est pas déconnecté
            if (from_server == 0)
            {
                strcpy (message, envoyeur.name);
                strcat (message, " : ");
            }
            strcat (message, buffer);
            ecrire (clients[i].sock, message,1024);
        }
    }
}

// travail en bit :
// 1er bit = joueur 1 connecte
// 2e bit = joueur 2 connecte
// 3e bit = controler connecte

void maj_var_affi (int actual, int nbr_connexion, int*affichage)
{
    //pas de players
    if (actual == 0)
    {
        // mais le controler
        if (nbr_connexion == 1)
            *affichage = 4;
        else
            *affichage = 0;
    }
    // 1 joueur connecté (par défaut = joueur 1 normalement)
    else if (actual == 1)
    {
        // + Controler
        if (nbr_connexion == 2)
            *affichage = 5;
        else
            *affichage = 1;
    }
    // 2 joueurs
    else if (actual == 2)
    {
        // + C
        if (nbr_connexion == 3)
            *affichage = 7;
        else
            *affichage = 3;
    }
}

void etat_connexion (int qui_connecte, Cliente clients[], Cliente controler)
{
    switch (qui_connecte)
    {
    case 0:
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t[] Joueur1\n\t[] Joueur2\n\t[] Controler");
        break;
    case 1: // Joueur 1 connecté
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t%c %s\n\t[] Joueur2\n\t[] Controler", 220, clients[0].name);
        break;
    case 2: // Joueur 2 connecté --> ne devrait pas arriver
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t[] Joueur1\n\t%c %s\n\t[] Controler", 220, clients[1].name);
        break;
    case 3: // JoueurS connectés
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif
        system ("cls");
        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t%c %s\n\t%c %s\n\t[] Controler", 220, clients[0].name, 220, clients[1].name);
        break;
    case 4: // Controler connecté
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t[] Joueur1\n\t[] Joueur2\n\t%c CONTROLER!", 220);
        break;
    case 5: // C + J1
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t%c %s\n\t[] Joueur2\n\t%c CONTROLER!", 220, clients[0].name, 220);
        break;
    case 6: // C + J2 --> ne devrait pas arriver
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t[] Joueur1\n\t%c %s\n\t%c CONTROLER!", 220, clients[1].name, 220);
        break;
    case 7: // EVERYBODY
#ifdef WIN32
        system ("cls");
#elif defined (linux)
        system ("clear");
#endif

        printf ("\n\n\n\n\n\t\tCLIENTS CONNECTES : \n\n");
        printf ("\t%c %s\n\t%c %s\n\t%c CONTROLER!", 220, clients[0].name, 220, clients[1].name, 220);
        break;
    }
}

int recherche_max (int max, int actual, Cliente clients[], Cliente controler)
{
    int i = 0;
    int new_max = max;
    for (i = 0 ; i < actual ; i++)
    {
        if (new_max < clients[i].sock)
        {
            new_max = clients[i].sock;
        }
    }

    if (new_max < controler.sock)
    {
        new_max = controler.sock;
    }

    return new_max;
}

int maj_a_qui_de_jouer (int a_qui)
{
    int a_qui_de_jouer = a_qui;

    if (a_qui_de_jouer == BLACK_PLAYER)
        a_qui_de_jouer = WHITE_PLAYER;
    else
        a_qui_de_jouer = BLACK_PLAYER;

    return a_qui_de_jouer;
}

int change_GameMode (int gameMode)
{
    int game_mode = gameMode;
    if (game_mode == CONTINUOUS)
        game_mode = STEP_BY_STEP;
    else
        game_mode = CONTINUOUS;

    return game_mode;
}

int check_CRC (unsigned char* buffer)
{
    int i, fin;
    unsigned char sum;
    fin = (int) buffer[1] + 3;
    //printf ("fin=%d", fin);
    for (i = 2; i < fin; i++)
        sum += buffer[i];
    sum = sum & 0xff;
    //printf ("CRC = %d", sum);

    if (sum == buffer[fin])
        return 1;
    return -1;

}

void init_plateau(int** plateau)
{
    int i,j;
    plateau = realloc (plateau,sizeof (char*) *taille);
    for (i = 0; i < taille; i++)
        plateau[i] = realloc (plateau[i], sizeof (char) *taille);
    for (i=0 ; i<taille ; i++)
    {
        for (j=0 ; j<taille ; j++)
        {
            plateau[i][j]=EMPTY_SQUARE;
        }
    }
    plateau[taille/2-1][taille/2-1]=BLACK_PAWN;
    plateau[taille/2][taille/2-1]=WHITE_PAWN;
    plateau[taille/2-1][taille/2]=WHITE_PAWN;
    plateau[taille/2][taille/2]=BLACK_PAWN;
}

int mvmnt_valide(int **plateau, int x, int y)
{
    int ok=-1;
    // Dans le plateau
    if ( ((x>=0) && (x<=taille-1)) && ((y>=0) && (y<=taille-1)) )
    {
        // Case vide
        if (plateau[x][y] == EMPTY_SQUARE)
            ok=0;
    }
    return ok;
}

int capturable(int **plateau, int x, int y, int which_player)
{
    int which_way=0; // 8 en tout : haut bas gauche droite + les 4 diagonales
    int oui_non=0;
    int i,j;

    printf("JOUEUR : %d",which_player);

    while (which_way<8)
    {
        if (which_player==BLACK_PLAYER)
        {
            if ( (which_way==HAUT) && (x>0) && (plateau[x-1][y]==WHITE_PAWN) )
                oui_non=encercle(plateau,x-1,y,which_player,-1,0);
            else if ( (which_way==BAS) && (x<(taille-1)) && (plateau[x+1][y]==WHITE_PAWN) )
                oui_non=encercle(plateau,x+1,y,which_player,1,0);

            else if ( (which_way==GAUCHE) && (y>0) && (plateau[x][y-1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x,y-1,which_player,0,-1);

            else if ( (which_way==DROITE) && (y<(taille-1)) && (plateau[x][y+1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x,y+1,which_player,0,1);

            else if ( (which_way==DIAGHG) && (x>0) && (y>0) && (plateau[x-1][y-1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x-1,y-1,which_player,-1,-1);

            else if ( (which_way==DIAGHD) && (y<(taille-1)) && (x>0) && (plateau[x-1][y+1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x-1,y+1,which_player,-1,1);

            else if ( (which_way==DIAGBG) && (y>0) && (x<(taille-1)) && (plateau[x+1][y-1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x+1,y-1,which_player,+1,-1);

            else if ( (which_way==DIAGBD) && (x<(taille-1)) && (y<(taille-1)) && (plateau[x+1][y+1]==WHITE_PAWN) )
                oui_non=encercle(plateau,x+1,y+1,which_player,1,1);
        }

        else if (which_player==WHITE_PLAYER)
        {
            if ( (which_way==HAUT) && (x>0) && (plateau[x-1][y]==BLACK_PAWN) )
                oui_non=encercle(plateau,x-1,y,which_player,-1,0);

            else if ( (which_way==BAS) && (x<(taille-1)) && (plateau[x+1][y]==BLACK_PAWN) )
                oui_non=encercle(plateau,x+1,y,which_player,1,0);


            else if ( (which_way==GAUCHE) && (y>0) && (plateau[x][y-1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x,y-1,which_player,0,-1);


            else if ( (which_way==DROITE) && (y<(taille-1)) && (plateau[x][y+1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x,y+1,which_player,0,1);

            else if ( (which_way==DIAGHG) && (x>0) && (y>0) && (plateau[x-1][y-1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x-1,y-1,which_player,-1,-1);

            else if ( (which_way==DIAGHD) && (y<(taille-1)) && (x>0) && (plateau[x-1][y+1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x-1,y+1,which_player,-1,1);

            else if ( (which_way==DIAGBG) && (y>0) && (x<(taille-1)) && (plateau[x+1][y-1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x+1,y-1,which_player,+1,-1);

            else if ( (which_way==DIAGBD) && (x<(taille-1)) && (y<(taille-1)) && (plateau[x+1][y+1]==BLACK_PAWN) )
                oui_non=encercle(plateau,x+1,y+1,which_player,1,1);
        }

        if (oui_non==1)
            break;

        which_way++;
    }

    return oui_non;
}

int encercle( int**plateau, int x, int y, int which_player, int dx, int dy)
{
    int oui_non=0;
    int x_parcours = x;
    int y_parcours = y;
    int encadre=0;
    // Tant qu'on ne sort pas du plateau
    while ( ((x_parcours+dx)>=0) && ((x_parcours+dx)<=(taille-1)) && ((y_parcours+dy)>=0) && ((y_parcours+dy)<=(taille-1)) )
    {
        x_parcours+=dx;
        y_parcours+=dy;

        if (which_player==BLACK_PLAYER)
        {
            if (plateau[x_parcours][y_parcours]==BLACK_PLAYER)
            {
                oui_non=1; // C'est effectivement encerclé;
                break;
            }
            else if (plateau[x_parcours][y_parcours]==EMPTY_SQUARE)
                break; // Ce n'est pas encerclé
        }

        else if (which_player==WHITE_PLAYER)
        {
            if (plateau[x_parcours][y_parcours]==WHITE_PLAYER)
            {
                oui_non=1; // C'est effectivement encerclé;
                break;
            }
            else if (plateau[x_parcours][y_parcours]==EMPTY_SQUARE)
                break; // Ce n'est pas encerclé
        }
    }

    return oui_non;
}

// Simplement là pour fractionner le code (sert pour les test de capture de pions adverses)
void capture(int **plateau, int x, int y, int which_player, int which_way)
{

    if (which_player==BLACK_PLAYER)
    {
        if ( (which_way==HAUT) && (x>0) && (plateau[x-1][y]==WHITE_PAWN) )
            retournement_pion(plateau,x-1,y,which_player,-1,0);
        else if ( (which_way==BAS) && (x<(taille-1)) && (plateau[x+1][y]==WHITE_PAWN) )
            retournement_pion(plateau,x+1,y,which_player,1,0);

        else if ( (which_way==GAUCHE) && (y>0) && (plateau[x][y-1]==WHITE_PAWN) )
            retournement_pion(plateau,x,y-1,which_player,0,-1);

        else if ( (which_way==DROITE) && (y<(taille-1)) && (plateau[x][y+1]==WHITE_PAWN) )
            retournement_pion(plateau,x,y+1,which_player,0,1);

        else if ( (which_way==DIAGHG) && (x>0) && (y>0) && (plateau[x-1][y-1]==WHITE_PAWN) )
            retournement_pion(plateau,x-1,y-1,which_player,-1,-1);

        else if ( (which_way==DIAGHD) && (y<(taille-1)) && (x>0) && (plateau[x-1][y+1]==WHITE_PAWN) )
            retournement_pion(plateau,x-1,y+1,which_player,-1,1);

        else if ( (which_way==DIAGBG) && (y>0) && (x<(taille-1)) && (plateau[x+1][y-1]==WHITE_PAWN) )
            retournement_pion(plateau,x+1,y-1,which_player,+1,-1);

        else if ( (which_way==DIAGBD) && (x<(taille-1)) && (y<(taille-1)) && (plateau[x+1][y+1]==WHITE_PAWN) )
            retournement_pion(plateau,x+1,y+1,which_player,1,1);
    }

    else if (which_player==WHITE_PLAYER)
    {
        if ( (which_way==HAUT) && (x>0) && (plateau[x-1][y]==BLACK_PAWN) )
            retournement_pion(plateau,x-1,y,which_player,-1,0);
        else if ( (which_way==BAS) && (x<(taille-1)) && (plateau[x+1][y]==BLACK_PAWN) )
            retournement_pion(plateau,x+1,y,which_player,1,0);

        else if ( (which_way==GAUCHE) && (y>0) && (plateau[x][y-1]==BLACK_PAWN) )
            retournement_pion(plateau,x,y-1,which_player,0,-1);

        else if ( (which_way==DROITE) && (y<(taille-1)) && (plateau[x][y+1]==BLACK_PAWN) )
            retournement_pion(plateau,x,y+1,which_player,0,1);

        else if ( (which_way==DIAGHG) && (x>0) && (y>0) && (plateau[x-1][y-1]==BLACK_PAWN) )
            retournement_pion(plateau,x-1,y-1,which_player,-1,-1);

        else if ( (which_way==DIAGHD) && (y<(taille-1)) && (x>0) && (plateau[x-1][y+1]==BLACK_PAWN) )
            retournement_pion(plateau,x-1,y+1,which_player,-1,1);

        else if ( (which_way==DIAGBG) && (y>0) && (x<(taille-1)) && (plateau[x+1][y-1]==BLACK_PAWN) )
            retournement_pion(plateau,x+1,y-1,which_player,+1,-1);

        else if ( (which_way==DIAGBD) && (x<(taille-1)) && (y<(taille-1)) && (plateau[x+1][y+1]==BLACK_PAWN) )
            retournement_pion(plateau,x+1,y+1,which_player,1,1);
    }

}

/// IMPORTANT : le x et le y correspondent à la position du pion posé + 1 !!!!!
void retournement_pion(int **plateau, int x, int y, int which_player, int dx, int dy)
{
    int x_parcours = x;
    int y_parcours = y;
    int pion_captures=0;
    int i=0;
    int encadre=0; // on incremente les pion_capture sans savoir s'ils le seront,d'où la nécessité de savoir à la fin s'ils étaient encerclés

    while (plateau[x_parcours][y_parcours]!=EMPTY_SQUARE)
    {
        // Si on ne sort pas du plateau
        if ( ((x_parcours+dx)>=0) && ((x_parcours+dx)<=(taille-1)) && ((y_parcours+dy)>=0) && ((y_parcours+dy)<=(taille-1)) )
        {
            x_parcours+=dx;
            y_parcours+=dy;

            if ( (which_player==BLACK_PLAYER) && (plateau[x_parcours][y_parcours]==WHITE_PLAYER) )
                pion_captures++;
            else if ( (which_player==BLACK_PLAYER) && (plateau[x_parcours][y_parcours]==BLACK_PLAYER) )
            {
                pion_captures++;    // ON s'arrête car on a tout capturé
                encadre=1;
                break;
            }

            else if ( (which_player==WHITE_PLAYER) && (plateau[x_parcours][y_parcours]==BLACK_PLAYER) )
                pion_captures++;
            else if ( (which_player==WHITE_PLAYER) && (plateau[x_parcours][y_parcours]==WHITE_PLAYER) )
            {
                pion_captures++;    // ON s'arrête car on a tout capturé
                encadre=1;
                break;
            }
        }
        else
            break;
    }



    if (encadre==1)
    {
        //Maintenant qu'on a fini de tout checker, place à la modification ! = capture
        // On se repositionne à x et y car ses coord correspondent aux coord du pion placé + 1 ! (donc déjà capturable)
        x_parcours=x;
        y_parcours=y;

        for (i=0 ; i <pion_captures ; i++)
        {
            if (which_player == BLACK_PLAYER)
                plateau[x_parcours][y_parcours]=BLACK_PAWN;
            else if (which_player == WHITE_PLAYER)
                plateau[x_parcours][y_parcours]=WHITE_PAWN;
            x_parcours+=dx;
            y_parcours+=dy;
        }
    }
}

void maj_plateau(int **plateau, int x, int y, int which_player)
{
    int x_base=x;
    int y_base=y;
    int i,j;
    int directions_checkees=0; // 8 en tout : haut bas gauche droite + les 4 diagonales

    if(which_player==BLACK_PLAYER)
    {
        plateau[x][y] = BLACK_PAWN;
    }

    else if(which_player==WHITE_PLAYER)
    {
        plateau[x][y] = WHITE_PAWN;
    }


    while (directions_checkees<8)
    {
        capture(plateau,x,y,which_player,directions_checkees);
        directions_checkees++;
    }

    for (i=0; i<taille; i++)
    {
        for(j=0; j<taille; j++)
        {
            printf("%d ",plateau[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");

}


void creation_NextTurn(unsigned char* nextTurn, int** plateau, int x, int y)
{
    int i,j;
    int cases_traitees=0;
    int temp;
    int current=7;
    temp=0;
    nextTurn[0] = 0x55;
    nextTurn[1] = 4+taille*taille/4;
    nextTurn[2] = 0x05;
    nextTurn[3] = x;
    nextTurn[4] = y;
    nextTurn[5] = taille;
    nextTurn[6] = taille;

    for (i=0 ; i< taille ; i++)
    {
        for (j=0 ; j<taille ; j++)
        {
            if ((cases_traitees%4)==0) // 1er bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    nextTurn[current]=64;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    nextTurn[current]=128;
                }
                else if (plateau[i][j] == EMPTY_SQUARE)
                {
                    nextTurn[current]=0;
                }
            }

            else if ((cases_traitees%4)==1) // 2e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    nextTurn[current]+=16;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    nextTurn[current]+=32;
                }
            }

            else if ((cases_traitees%4)==2) // 3e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    nextTurn[current]+=4;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    nextTurn[current]+=8;
                }
            }

            else if ((cases_traitees%4)==3) // 4e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    nextTurn[current]+=1;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    nextTurn[current]+=2;
                }
            }
            cases_traitees++;
            if ((cases_traitees%4)==0) // si on a traité 4 cases
            {
                current++;
            }
        }
    }

    // CRC
    for (i=2; i<current; i++)
    {
        temp+=nextTurn[i];
    }
    nextTurn[i] = temp & 0xff;
}

void creation_Status1(unsigned char* status1, int** plateau, int x, int y)
{
    int i,j;
    int current=7;
    int temp=0;
    int cases_traitees=0;
    status1[0] = 0x55;
    status1[1] = 4+taille*taille/4;
    status1[2] = 0x06;
    status1[3] = x;
    status1[4] = y;
    status1[5] = taille;
    status1[6] = taille;

    for (i=0 ; i< taille ; i++)
    {
        for (j=0 ; j<taille ; j++)
        {
            if ((cases_traitees%4)==0) // 1er bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    status1[current]=64;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    status1[current]=128;
                }
                else if (plateau[i][j] == EMPTY_SQUARE)
                {
                    status1[current]=0;
                }
            }

            else if ((cases_traitees%4)==1) // 2e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    status1[current]+=16;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    status1[current]+=32;
                }
            }

            else if ((cases_traitees%4)==2) // 3e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    status1[current]+=4;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    status1[current]+=8;
                }
            }

            else if ((cases_traitees%4)==3) // 4e bit sur 4 traité
            {
                if (plateau[i][j] == BLACK_PAWN) // 01
                {
                    status1[current]+=1;
                }
                else if (plateau[i][j] == WHITE_PAWN) // 10
                {
                    status1[current]+=2;
                }
            }
            cases_traitees++;
            if ((cases_traitees%4)==0) // si on a traité 4 cases
            {
                current++;
            }
        }
    }

    for (i=2; i<current; i++)
    {
        temp+=status1[i];
    }
    status1[i] = temp & 0xff;
}

void creation_Status2(unsigned char* status2, int **plateau, Cliente* clients)
{
    int i,j;
    int current=7;
    int temp;
    int cases_traitees;

    status2[0] = 0x55;
    status2[1] = 8+strlen(clients[0].name)+strlen(clients[1].name);
    status2[2] = 0x07;

    status2[3] = clients[0].score;
    // MSB
    status2[4] = clients[0].time_reflexion >> 8; // On récupère les 8 derniers bits
    // LSB
    status2[5] = clients[0].time_reflexion - (status2[4]*256); // Résultat final moins les 8 derniers bits
    status2[6] = strlen(clients[0].name);
    for (i=0 ; i<strlen(clients[0].name) ; i++)
    {
        status2[current] = clients[0].name[i];
        current++;
    }
    status2[current] = clients[1].score;
    current++;
    status2[current] = clients[1].time_reflexion >> 8;
    current++;
    status2[current] = clients[1].time_reflexion - (status2[current-1]*256);
    current++;
    status2[current] = strlen(clients[1].name);
    current++;
    for (i=0 ; i < strlen(clients[1].name) ; i++)
    {
        status2[current] = clients[1].name[i];
        current++;
    }

    for (i=2 ; i < current; i++)
    {
        temp+=status2[i];
    }
    status2[i] = temp & 0xff;
}


void score_plateau(Cliente *clients, int** plateau)
{
    int i,j;

    for (i=0; i<taille ; i++)
    {
        for (j=0 ; j<taille ; j++)
        {
            if (plateau[i][j]==WHITE_PAWN)
                clients[1].score+=1;
            else if (plateau[i][j]==BLACK_PAWN)
                clients[0].score+=1;
        }
    }

    printf("Score %s : %d \nScore %s : %d\n\n",clients[0].name,clients[0].score,clients[1].name,clients[1].score);
}

void score_final(Cliente *clients, int** plateau)
{
    /// A CODER MAIS J AI PAS D IDEES
}

void application()
{
    SOCKET sock_server = init_connection(); // on récupère le socket serveur
    unsigned char buffer[BUFFER_SIZE]; // c'est dans ce tableau que l'on lira/écrira les données envoyées/reçues
    int actual = 0; // nombre de socket players!
    int max = sock_server; // nécessaire de connaître le socket avec le plus grand descripteur pour utilisation correcte de select
    int nbr_connexions = 0; // car on différencie player et controler, mais bon ...
    int i;
    int** plateau=NULL;

    plateau = malloc (sizeof (unsigned char*) *taille);
    for (i = 0; i < taille; i++)
        plateau[i] = malloc (sizeof (unsigned char) * taille);

    Cliente clients[MAX_CLIENT]; // c'est dans cet array que nous regrouperons tous nos clients
    Cliente controler; // On différencie nos players de notre controler pour simplifier la compréhension du code et son utilisation
    strcpy (controler.name, "none");
    controler.sock = 0;

    fd_set rdfs; // initialisation de l'ensemble de lecture (pour savoir quand ya arrivée de donnée à lire)

    int affichage = 0;

    etat_connexion (affichage, clients, controler);

    /// Tant qu'il n'y a pas 3 connexions, on bloque jusqu'à ce que tout le monde soit connecté
    while (nbr_connexions != MAX_CLIENT)
    {
        int i = 0;
        /// NB : explication du code dans le while suivant
        // Code pour écouter et récupérer les connexions
        FD_ZERO (&rdfs);
        FD_SET (sock_server, &rdfs);

        for (i = 0 ; i < actual ; i++)
        {
            FD_SET (clients[i].sock, &rdfs);
        }

        if (strcmp (controler.name, "none") != 0)
        {
            FD_SET (controler.sock, &rdfs);
        }

        if (select (max + 1, &rdfs, NULL, NULL, NULL) == -1)
        {
            perror ("select()");
            exit (EXIT_FAILURE);
        }

        // S'il y a changement d'état du descripteur de sock_server dans rdfs
        /// = quelqu'un a voulu se connecter (ça impact sur le sock du serveur)
        if (FD_ISSET (sock_server, &rdfs) )
        {
            // Nouvelle connexion
            // On initialise ce dont on a besoin pour établir la connexion
            SOCKADDR_IN du_client;
            size_t client_size = sizeof (du_client);
            // On accepte la connexion
            int sock_client = accept (sock_server, (SOCKADDR *) &du_client, &client_size);
            if (sock_client == SOCKET_ERROR)
            {
                perror ("accept()");
                continue;
            }

            // La connexion s'ensuit forcément de l'envoie de son pseudo
            // Donc on le lit
            if (lire (sock_client, buffer) == -1)
            {
                printf ("Client directement deconnecte\n");
                continue;
            }

            // Si le message de connexion n'a pas été envoyé proprement, on envoie une erreur de connexion et le client n'est pas reconnu
            if (buffer[0] != 0x55 || buffer[2] != 0x01 || check_CRC (buffer) != 1) /// IL FAUT AUSSI METTRE EN PLACE UN TIMEOUT + éventuelle closesocket() ?
            {
                unsigned char nok[5];
                int e, temp;
                temp = 0;
                nok[0] = 0x55;
                nok[1] = 1;
                nok[2] = 0x02;
                nok[3] = 0x00;
                for (e = 2; e < 4; e++)
                    temp += nok[e];
                nok[4] = temp;
                ecrire (sock_client, nok,5);
            }
            // Si client reconnu
            else
            {
                // On s'assure de maj notre max pour qu'il vaille toujours ... le max
                max = sock_client > max ? sock_client : max;
                FD_SET (sock_client, &rdfs);

                // S'il s'agit du controler qui vient de se connecter
                if (strncmp (&buffer[3], "controler", 9) == 0)
                {
                    controler.sock = sock_client;
                    controler.name[0] = 0;
                    strncpy (controler.name, &buffer[3], (int) buffer[1]);

                    /// MESSAGE
                    // on lui fait un send OK pour lui dire qu'il s'est bien déconnecté
                    unsigned char ok[5];
                    int e, temp;
                    temp = 0;
                    ok[0] = 0x55;
                    ok[1] = 1;
                    ok[2] = 0x02; // On utilise un OK pour le controler car on n'a pas à lui attribuer une couleur
                    ok[3] = 0x01;
                    for (e = 2; e < 4; e++)
                        temp += ok[e];
                    ok[4] = temp & 0xff;
                    ecrire (sock_client, ok,5);

                }
                // Il s'agit d'un player qui vient de se connecter
                else
                {

                    // On crée proprement notre client pour l'ajouter dans notre liste de clients players
                    Cliente new_client;
                    new_client.sock = sock_client;
                    new_client.name[0] = 0;
                    strncpy (new_client.name, &buffer[3], (int) buffer[1]);
                    new_client.name[(int)buffer[3]]='\0';
                    if (actual==0) // c le premier à se connecter
                        new_client.qui= BLACK_PLAYER;
                    else if (actual==1)
                        new_client.qui = WHITE_PLAYER;
                    new_client.score=0;
                    new_client.time_reflexion=0;
                    clients[actual] = new_client;

                    actual++;


                    /// MESSAGE
                    // on lui fait un send OK pour lui dire qu'il s'est bien connecté
                    unsigned char ok[5];
                    int e, temp;
                    temp = 0;
                    ok[0] = 0x55;
                    ok[1] = 1;
                    ok[2] = 0x10;
                    if (actual==1) // 1 seul player connecté
                        ok[3] = 0x01; // black = first player
                    else if (actual==2) // 2 players
                        ok[3] = 0x02; // white player
                    for (e = 2; e < 4; e++)
                        for (e = 2; e < 4; e++)
                            temp += ok[e];
                    ok[4] = temp & 0xff;
                    ecrire (sock_client, ok,5);

                }
                nbr_connexions++;
                maj_var_affi (actual, nbr_connexions, &affichage);
                etat_connexion (affichage, clients, controler);
            }
            strcpy (buffer, ""); // on clear notre buffer
        }

        // Puisqu'on ne gère ici que les connexions et déconnexions
        // --> si ce n'est pas une connexion, c'est une .... bravoooo :)
        else
        {
            //Controler ?
            if (FD_ISSET (controler.sock, &rdfs) )
            {
                int c = lire (controler.sock, buffer); // on récupère le return pour connaître l'état du client
                if ( (c == 0) || (c < 0) )
                {
                    // On ferme son socket
                    closesocket (controler.sock);
                    // On le réinitialise
                    controler.sock = 0;
                    strcpy (controler.name, "none");
                    strcpy (buffer, "");
                    nbr_connexions--;
                    maj_var_affi (actual, nbr_connexions, &affichage);
                    etat_connexion (affichage, clients, controler);
                    // il faut ensuite recalculer le max pour s'assurer la bonne utilisation du select
                    max = recherche_max (max, actual, clients, controler);
                }
            }

            // Ou players ?
            else
            {
                for (i = 0; i < actual; i++)
                {
                    if (FD_ISSET (clients[i].sock, &rdfs) )
                    {
                        int c = lire (clients[i].sock, buffer); // on récupère le return pour connaître l'état du client
                        // Client déconnecté
                        if ( (c == 0) || (c < 0) )
                        {
                            closesocket (clients[i].sock);
                            suppression_client (clients, i, &actual);
                            nbr_connexions--;
                            maj_var_affi (actual, nbr_connexions, &affichage);
                            etat_connexion (affichage, clients, controler);
                            // il faut ensuite recalculer le max pour s'assurer la bonne utilisation du select
                            max = recherche_max (max, actual, clients, controler);


                            if (actual==1) // Si ya plus qu'un seul player connecté
                            {
                                if (clients[0].qui == WHITE_PLAYER) // Si c le white qui reste, on le change en black
                                {
                                    ///MESSAGE
                                    unsigned char ok[5];
                                    int e, temp;
                                    temp = 0;
                                    ok[0] = 0x55;
                                    ok[1] = 1;
                                    ok[2] = 0x10;
                                    ok[3] = 0x01; // black = first player
                                    for (e = 2; e < 4; e++)
                                        for (e = 2; e < 4; e++)
                                            temp += ok[e];
                                    ok[4] = temp & 0xff;
                                    ecrire (clients[0].sock, ok,5);
                                }
                            }
                        }
                    }
                }
            }
            strcpy(buffer,"");
        }

        // Si tout le monde s'est connecté
        if (nbr_connexions == MAX_CLIENT)
        {
            int a_qui_de_jouer = BLACK_PLAYER;
            int gameMode = CONTINUOUS;
            unsigned char* nextTurn = NULL;
            unsigned char* status1 = NULL;
            unsigned char* status2 = NULL;
            nextTurn=malloc(sizeof(unsigned char)*(7+taille*taille/4+1));
            status1=malloc(sizeof(unsigned char)*(7+taille*taille/4+1)); // 0x55 + length + Type + LMX + LMY + BSizeX + BSizeY + Board State 0 à N + CRC
            status2=malloc(sizeof(unsigned char)*(12+strlen(clients[0].name)+strlen(clients[1].name)));
            // Les coordonnées du mouvement qui vient d'être fait par un des players
            // Initialisé à -1 de base = Pas de mouvement particulier
            int x=-1;
            int y=-1;
            // On peut commencer à travailler
            fflush (stdin);

            init_plateau(plateau);

            creation_NextTurn(nextTurn,plateau,x,y);
            //strcpy(nextTurn,creation_NextTurn(plateau,x,y));
            //nextTurn[strlen(nextTurn)]='\0';
            printf("\nEnvoi NEXT_TURN a %s : ",clients[0].name);
            for (i=0; i<(7+taille*taille/4+1); i++)
            {
                printf("%x ",nextTurn[i]);
            }
            printf("\n");
            ecrire(clients[0].sock,nextTurn,7+taille*taille/4+1);


            while (nbr_connexions == MAX_CLIENT)
            {
                int i = 0;
                /// NB : à chaque boucle, il faudra ajouter de nouveau tous nos sockets
                /// car le select efface tout à la fin
                FD_ZERO (&rdfs); // on clean tout
                FD_SET (sock_server, &rdfs); // on ajoute notre sock principal = du serveur

                //FD_SET(STDIN_FILENO, &rdfs); /// IMPORTANT : ON AJOUTE LE STDIN DANS LE RDFS CAR ON CONSIDERE QUE C AU GM DE GERER LE STEPBYSTEP

                for (i = 0 ; i < actual ; i++)
                {
                    // On ajoute les descripteurs de chaque clients player qu'on a
                    FD_SET (clients[i].sock, &rdfs);
                }


                // et du controler
                /// IF NON NECESSAIRE MAIS POUR LA BETA OU LE CONTROLER NE SE CONNECTE PAS, OUI (MAXCLIENT = 2)...
                if (strcmp (controler.name, "none") != 0)
                {
                    FD_SET (controler.sock, &rdfs);
                }

                //Maintenant avec la fonction select
                // ON check si des données (en lecture ici) sont arrivées
                if (select (max + 1, &rdfs, NULL, NULL, NULL) == -1)
                {
                    perror ("select()");
                    printf ("Merdeuh");
                    exit (EXIT_FAILURE);
                }

                printf ("\n\n");

                /// EN DESSOUS : 1) On check la réception de données +  2) déconnexions soudaines

                // Réception côté controler A TOUT MOMENT
                if (FD_ISSET (controler.sock, &rdfs) )
                {
                    int c = lire (controler.sock, buffer); // on récupère le return pour connaître l'état du client

                    // Controler déconnecté
                    if ( (c == 0) || (c < 0) )
                    {
                        printf ("WARNING : Le controler s'est deconnecte\n");
                        // On ferme son socket
                        closesocket (controler.sock);
                        // On le réinitialise
                        controler.sock = 0;
                        strcpy (controler.name, "none");
                        // il faut ensuite recalculer le max pour s'assurer la bonne utilisation du select
                        max = recherche_max (max, actual, clients, controler);
                        nbr_connexions--;
                        printf ("PROCEDURE D'URGENCE : #attente des 3 clients#\n");
                        printf ("Appuyez sur une touche ...\n\n");

                        maj_var_affi (actual, nbr_connexions, &affichage);
                        etat_connexion (affichage, clients, controler);

                    }
                    else //ya pas eu de soucis quoi
                    {
                        /// CONTROLER PEUT GERER A TOUT MOMENT :
                        /// 1) Le mode de jeu : continu ou à tour de rôle
                        /// 2) Relancer le jeu
                        /// 3) Modifier la taille du tableau
                        /// --> toutes ces infos dans un seul type de message
                        /// 4) Quitter = 2e type de message
                        /// NB : nécessité de créer un 3e type de message pour le tour de r

                        ///MESSAGE A TRAITER !
                        // Si message CONTROLER pas bien codé/envoyé
                        // ou incochérence = modifier dimensions plateau en pleine partie
                        if (buffer[0] != 0x55 || buffer[2] != 0x08 || check_CRC (buffer) != 1 || ( (( buffer[3]!=0)||(buffer[4]!=0 )) && (buffer[6]==0x00) ) ) /// IL FAUT AUSSI METTRE EN PLACE UN TIMEOUT + éventuelle closesocket() ?
                        {
                            unsigned char nok[5];
                            int e, temp;
                            temp = 0;
                            nok[0] = 0x55;
                            nok[1] = 1;
                            nok[2] = 0x02;
                            nok[3] = 0x00;
                            for (e = 2; e < 4; e++)
                                temp += nok[e];
                            nok[4] = temp & 0xff;
                            ecrire(controler.sock,buffer,5);
                            printf("Je ne peux rien faire, le controler s'est plant%c ...\n",130);
                        }
                        else
                        {
                            unsigned char ok[5];
                            int e, temp;
                            temp = 0;
                            ok[0] = 0x55;
                            ok[1] = 1;
                            ok[2] = 0x02;
                            ok[3] = 0x01;
                            for (e = 2; e < 4; e++)
                                temp += ok[e];
                            ok[4] = temp & 0xff;

                            // S'il y a une modif de la taille du plateau, et on a blindé en haut : si ya modif, ya forcément eu restart
                            if (buffer[3]!=0 && buffer[3]==buffer[4])
                            {
                                taille=buffer[3];
                            }

                            // On restart
                            if (buffer[6]==0x01)
                            {
                                // On remet à blanc le plateau
                                init_plateau(plateau);
                            }

                            if ((int)buffer[5]==CONTINUOUS)
                            {
                                if (gameMode!=CONTINUOUS)
                                    gameMode=CONTINUOUS;
                            }
                            else if ((int)buffer[5]==STEP_BY_STEP)
                            {
                                if (gameMode!=STEP_BY_STEP)
                                {
                                    gameMode=STEP_BY_STEP;
                                }

                                // On était déjà en Stepb by step --> il a donné la main pour que le prochain joueur joue
                                else if (gameMode==STEP_BY_STEP)
                                {
                                    ///MESSAGE NEXTTURN
                                    //unsigned char* nextTurn = NULL;
                                    //nextTurn=malloc(sizeof(unsigned char)*(7+taille*taille/4+1));
                                    int j;
                                    /*int e,temp;
                                    int current=7;
                                    temp=0;
                                    nextTurn[0] = 0x55;
                                    nextTurn[1] = 4+taille*taille/4;
                                    nextTurn[2] = 0x05;
                                    nextTurn[3] = -1;
                                    nextTurn[4] = -1;
                                    nextTurn[5] = taille;
                                    nextTurn[6] = taille;
                                    for (e=0 ; e < (taille*taille/4) ; e++)
                                    {
                                        nextTurn[current] = etat_partiel_plateau(plateau,e);
                                        current++;
                                    }
                                    for (e=2; e<current ; e++)
                                    {
                                        temp+=nextTurn[e];
                                    }
                                    nextTurn[e] = temp & 0xff;*/
                                    //strcpy(nextTurn,creation_NextTurn(plateau,x,y));
                                    //nextTurn[strlen(nextTurn)]='\0';
                                    creation_NextTurn(nextTurn,plateau,x,y);
                                    printf("\nEnvoi NEXTURN A %s : ",clients[a_qui_de_jouer]);
                                    for (j=0; j<(7+taille*taille/4+1); j++)
                                    {
                                        printf("%x ",nextTurn[j]);
                                    }
                                    printf("\n");

                                    // On maj le joueur qui doit jouer
                                    a_qui_de_jouer=maj_a_qui_de_jouer(a_qui_de_jouer);


                                    // C le BLACK_PLAYER qui vient de jouer
                                    if (a_qui_de_jouer==BLACK_PLAYER)
                                        ecrire(clients[0].sock,nextTurn,7+taille*taille/4+1);
                                    // WHITE_PLAYER qui a joué
                                    else if (a_qui_de_jouer==WHITE_PLAYER)
                                        ecrire(clients[1].sock,nextTurn,7+taille*taille/4+1);
                                    printf("1");
                                    //free(nextTurn);
                                    printf("2");
                                }
                            }


                            ecrire (controler.sock, ok,5);

                        }
                        // En fonction du type de message (normalement, un des 3 types ci-dessus
                        // Faire ce qu'il faut

                        ///NB : pour la 1), la fonction "change_GameMode(int game_mode) a déjà été codée ! Plus q'à coder ;) Bon chance
                        /// NB2 : la 2) et la 2) sont non gérées pour le moment, à faire une fois tous les autres "MESSAGES" aboutis
                    }
                    strcpy (buffer, "");
                }

                // Si jamais ya plusieurs envois en même temps --> s'assurer que tout le monde est là avant de récolter une autre donnée
                if (nbr_connexions == MAX_CLIENT)
                {

                    // On vérifie si un client a envoyé une donnée
                    for (i = 0; i < actual; i++)
                    {
                        // On va checker si l'un des clients a envoyé une donnée
                        /// NB: ici, on travaille non pas avec sock_serveur  mais client[i].sock
                        /// Car si le descripteur d'un des clients DEJA EXISTANTS est apparu dans le rdfs
                        /// 1) c'est qu'il a envoyé une donnée / 2) son descripteur correspond à client[i].sock
                        if (FD_ISSET (clients[i].sock, &rdfs) )
                        {
                            Cliente client_sender = clients[i];
                            clients[i].time_reflexion+=(clock()-temps)/CLOCKS_PER_SEC;
                            int c = lire (clients[i].sock, buffer); // on récupère le return pour connaître l'état du client
                            printf("Reception donnee : NEW_MOVE de %s : %x %d %x %d %d %d",clients[i].name,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
                            printf("\n\n");

                            printf("Je coupe la communication, tu as reussi a recuperer un message, attendre ton nextturn, et renvoyer\n");
                            printf("J'affiche le message que j'envoie comme le message que je recois\n");
                            printf("PENSE A AFFICHER CE QUE T ENVOIES ET CE QUE TU RECOIS DE TON COTE POUR COMPARER!!\n");
                            printf("\nIls peuvent etre legerement differents car j'affiche tantot en hexa, tantot en decimal\n");
                            printf("DONC SI TU VOIS UNE GRANDE DIFFERENCE OU QUE MEME EN CONVERTISSANT DE L UN A LAUTRE CA COLLLE PAS\n");
                            printf("REPARS TRAVAILLER, SALE BABOUIN ! :)");
                            printf("\nSinon bien joue, tu es vers la voie de la reussite et du podium, cher ami hehe");
                            return 0;

                            // Client déconnecté
                            if ( (c == 0) || (c < 0) )
                            {
                                printf ("%s vient de se deconnecter\n", clients[i].name);
                                closesocket (clients[i].sock);
                                suppression_client (clients, i, &actual);
                                // il faut ensuite recalculer le max pour s'assurer la bonne utilisation du select
                                max = recherche_max (max, actual, clients, controler);
                                nbr_connexions--;
                                printf ("PROCEDURE D'URGENCE : #attente des 3 clients#\n");

                                maj_var_affi (actual, nbr_connexions, &affichage);
                                etat_connexion (affichage, clients, controler);
                                break; // faut sortir de notre for
                            }

                            //sinon ya pas eu de soucis quoi = il a envoyé consciemment une donnée

                            // C pas à lui de jouer
                            else if ( ( (i==0) && (a_qui_de_jouer==WHITE_PLAYER) ) || ( (i==1)&& (a_qui_de_jouer==BLACK_PLAYER) ) )
                            {
                                printf("%s a joue a la place de l'autre joueur\n\n",clients[i].name);
                                printf("\n\n");
                                /// MESSAGE NOK mais pas obligé vu qu'il a perdu
                                unsigned char nok[5];
                                unsigned char end[4];
                                //unsigned char* status2 = NULL;
                                //status2=malloc(sizeof(unsigned char)*(12+strlen(clients[0].name)+strlen(clients[1].name)));
                                int e, temp;
                                int j;
                                temp = 0;
                                nok[0] = 0x55;
                                nok[1] = 1;
                                nok[2] = 0x02;
                                nok[3] = 0x00;
                                for (e = 2; e < 4; e++)
                                    temp += nok[e];
                                nok[4] = temp & 0xff;
                                ecrire(clients[i].sock, nok,5);

                                temp=0;

                                clients[i].score=0; // Il a perdu
                                score_final(clients,plateau);
                                ///MESSAGE STATUS2 au controler pour qu'il affiche correctement le joueur gagnant
                                //strcpy(status2,creation_Status2(plateau,clients));
                                //status2[strlen(status2)]='\0';
                                creation_Status2(status2,plateau,clients);
                                printf("\nStatus2 : ");
                                for (j=0; j<(12+strlen(clients[0].name)+strlen(clients[1].name)); j++)
                                {
                                    printf("%x ",status2[j]);
                                }
                                printf("\n");
                                ecrire(controler.sock,status2,12+strlen(clients[0].name)+strlen(clients[1].name));



                                /// MESSAGE POUR LE END !! = fin de partie
                                end[0] = 0x55;
                                end[1] = 0;
                                end[2] = 0x04;
                                end[3] = end[2] & 0xff;
                                ecrire(clients[0].sock, end,4);
                                ecrire(clients[1].sock, end,4);
                                ecrire(controler.sock, end,4);
                                init_plateau(plateau);

                                for (j = 0; j < actual; j++)
                                {
                                    clients[j].score=0;
                                    clients[j].time_reflexion=0;
                                }

                                printf("3");
                                //free(status2);
                                printf("4");
                            }

                            else
                            {
                                // Si message NEWMOVE pas bien codé/envoyé
                                if (buffer[0] != 0x55 || buffer[2]!=0x03 || check_CRC (buffer) != 1) /// IL FAUT AUSSI METTRE EN PLACE UN TIMEOUT + éventuelle closesocket() ?
                                {
                                    printf("%s a mal envoye son message\n\n",clients[i].name);
                                    /// MESSAGE NOK mais pas obligé vu qu'il a perdu
                                    unsigned char nok[5];
                                    unsigned char end[4];
                                    //unsigned char* status2 = NULL;
                                    //status2=malloc(sizeof(unsigned char)*(12+strlen(clients[0].name)+strlen(clients[1].name)));
                                    int e, temp;
                                    int j;
                                    temp = 0;
                                    nok[0] = 0x55;
                                    nok[1] = 1;
                                    nok[2] = 0x02;
                                    nok[3] = 0x00;
                                    for (e = 2; e < 4; e++)
                                        temp += nok[e];
                                    nok[4] = temp & 0xff;
                                    ecrire(clients[i].sock, nok,5);

                                    clients[i].score=0; // Il a perdu
                                    score_final(clients,plateau);
                                    ///MESSAGE STATUS2 au controler pour qu'il affiche correctement le joueur gagnant
                                    //strcpy(status2,creation_Status2(plateau,clients));
                                    //status2[strlen(status2)]='\0';
                                    creation_Status2(status2,plateau,clients);
                                    printf("\nStatus2 : ");
                                    for (j=0; j<(12+strlen(clients[0].name)+strlen(clients[1].name)); j++)
                                    {
                                        printf("%x ",status2[j]);
                                    }
                                    printf("\n");
                                    ecrire(controler.sock,status2,12+strlen(clients[0].name)+strlen(clients[1].name));

                                    /// MESSAGE POUR LE END !! = fin de partie
                                    end[0] = 0x55;
                                    end[1] = 0;
                                    end[2] = 0x04;
                                    end[3] = end[2] & 0xff;
                                    ecrire(clients[0].sock, end,4);
                                    ecrire(clients[1].sock, end,4);
                                    ecrire(controler.sock, end,4);
                                    init_plateau(plateau);

                                    for (j = 0; j < actual; j++)
                                    {
                                        clients[j].score=0;
                                        clients[j].time_reflexion=0;
                                    }

                                    printf("5");
                                    //free(status2);
                                    printf("6");
                                }
                                // Si message propre, on traite
                                else
                                {

                                    //On récupère les coordonnées
                                    x = (int)buffer[3];
                                    y = (int)buffer[4];


                                    printf ("x=%d et y=%d",x,y);
                                    printf("\nplateau : %d",plateau[x][y]);
                                    /// checker si le mouvement est valide : il transforme + il joue sur une case vide

                                    //Transformation check si on joue sur une case vide + si on emprisonne des pions adverses
                                    // = les conditions pour un coup valable
                                    if (capturable(plateau,x,y,a_qui_de_jouer) == 1)
                                    {
                                        printf("%s a bien joue!\n",clients[a_qui_de_jouer].name);
                                        /*unsigned char* status1 = NULL;
                                        unsigned char* status2 = NULL;
                                        status1=malloc(sizeof(unsigned char)*(7+taille*taille/4+1)); // 0x55 + length + Type + LMX + LMY + BSizeX + BSizeY + Board State 0 à N + CRC
                                        status2=malloc(sizeof(unsigned char)*(12+strlen(clients[0].name)+strlen(clients[1].name)));*/
                                        int j;
                                        //int current=7;
                                        ///MESSAGE OK

                                        unsigned char ok[5];
                                        unsigned char end[4];

                                        int e, temp;

                                        temp = 0;
                                        ok[0] = 0x55;
                                        ok[1] = 1;
                                        ok[2] = 0x02;
                                        ok[3] = 0x01;
                                        for (e = 2; e < 4; e++)
                                            temp += ok[e];
                                        ok[4] = temp & 0xff;
                                        ecrire (clients[i].sock, ok,5);

                                        // On met à jour notre plateau
                                        maj_plateau(plateau,x,y,a_qui_de_jouer);
                                        score_plateau(clients,plateau);

                                        // On met à jour les points de chaque player;
                                        /// A FAIRE

                                        /// MESSAGE
                                        // Envoi au controler maj tableau = Statut1 / 2
                                        // Status1
                                        /*temp=0;
                                        status1[0] = 0x55;
                                        status1[1] = 4+taille*taille/4;
                                        status1[2] = 0x06;
                                        status1[3] = x;
                                        status1[4] = y;
                                        status1[5] = taille;
                                        status1[6] = taille;
                                        for (e=0 ; e < (taille*taille/4) ; e++)
                                        {
                                            status1[current] = etat_partiel_plateau(plateau,e);
                                            current++;
                                        }
                                        for (e=2; e<current ; e++)
                                        {
                                            temp+=status1[e];
                                        }
                                        status1[e] = temp & 0xff;*/

                                        //strcpy(status1,creation_Status1(plateau,x,y));
                                        //status1[strlen(status1)]='\0';
                                        creation_Status1(status1,plateau,x,y);
                                        printf("\nStatus1 : ");
                                        for (j=0; j<(7+taille*taille/4+1); j++)
                                        {
                                            printf("%x ",status1[j]);
                                        }
                                        printf("\n");

                                        //ecrire(controler.sock,status1,7+taille*taille/4+1);

                                        /// MESSAGE pour le Status2 !
                                        //strcpy(status2,creation_Status2(plateau,clients));
                                        //status2[strlen(status2)]='\0';
                                        creation_Status2(status2,plateau,clients);
                                        printf("\nStatus2 : ");
                                        for (j=0; j<(12+strlen(clients[0].name)+strlen(clients[1].name)) ; j++)
                                        {
                                            printf("%x ",status2[j]);
                                        }
                                        printf("\n");
                                        //ecrire(controler.sock,status2,12+strlen(clients[0].name)+strlen(clients[1].name));

                                        coups_joues++;

                                        // Si plus de place sur le plateau ou un joueur a perdu avant (= plus de pions)
                                        if ( (coups_joues == (taille*taille-4)) || (clients[0].score==0) || (clients[1].score==0) )
                                        {
                                            printf("Partie termine\n");
                                            /// MESSAGE POUR LE END !! = fin de partie
                                            end[0] = 0x55;
                                            end[1] = 0;
                                            end[2] = 0x04;
                                            end[3] = end[2] & 0xff;
                                            ecrire(clients[0].sock, end,4);
                                            ecrire(clients[1].sock, end,4);
                                            ecrire(controler.sock, end,4);
                                        }

                                        /// MESSAGE du NEXTTURN a l'autre player seulement si c'est en mode continu
                                        if (gameMode == CONTINUOUS)
                                        {
                                            printf("%s vient de jouer, j'envoie donc le NEXTTURN au prochain joueur\n\n",clients[i].name);
                                            //unsigned char* nextTurn = NULL;
                                            //nextTurn=malloc(sizeof(unsigned char)*(7+taille*taille/4+1));
                                            int j;
                                            //strcpy(nextTurn,creation_NextTurn(plateau,x,y));
                                            //nextTurn[strlen(nextTurn)]='\0';
                                            creation_NextTurn(nextTurn,plateau,x,y);
                                            printf("\nNextTurn : ");
                                            for (j=0; j<(7+taille*taille/4+1); j++)
                                            {
                                                printf("%x ",nextTurn[j]);
                                            }
                                            printf("\n");

                                            // C le BLACK_PLAYER qui vient de jouer
                                            if (i==0)
                                                ecrire(clients[1].sock,nextTurn,7+taille*taille/4+1);
                                            // WHITE_PLAYER qui a joué
                                            else if (i==1)
                                                ecrire(clients[0].sock,nextTurn,7+taille*taille/4+1);



                                            temps=clock(); // On lance le timer pour le joueur qui est en train de réfléchir

                                            printf("7");
                                            //free(nextTurn);
                                            printf("8");
                                        }


                                        // On met à jour c à qui de jouer (si c'était le BP, maintenant c le WP et vice versa)
                                        a_qui_de_jouer = maj_a_qui_de_jouer (a_qui_de_jouer);

                                        printf("9");
                                        //free(status1);
                                        printf("10");

                                        printf("11");
                                        //free(status2);
                                        printf("12");

                                    }

                                    // Format bon mais mouvement invalide
                                    else
                                    {
                                        printf("%s a effectue un mouvement invalide\n\n",clients[i].name);
                                        unsigned char nok[5];
                                        unsigned char end[4];
                                        //unsigned char* status2 = NULL;
                                        //status2=malloc(sizeof(unsigned char)*(12+strlen(clients[0].name)+strlen(clients[1].name)));
                                        int e, temp;
                                        int j;
                                        temp = 0;
                                        nok[0] = 0x55;
                                        nok[1] = 1;
                                        nok[2] = 0x02;
                                        nok[3] = 0x00;
                                        for (e = 2; e < 4; e++)
                                            temp += nok[e];
                                        nok[4] = temp & 0xff;
                                        ecrire(clients[i].sock, nok,5);

                                        clients[i].score=0; // Il a perdu
                                        score_final(clients,plateau);
                                        ///MESSAGE STATUS2 au controler pour qu'il affiche correctement le joueur gagnant
                                        //strcpy(status2,creation_Status2(plateau,clients));
                                        //status2[strlen(status2)]='\0';
                                        creation_Status2(status2,plateau,clients);
                                        printf("\nStatus2 : ");
                                        for (j=0; j<(12+strlen(clients[0].name)+strlen(clients[1].name)); j++)
                                        {
                                            printf("%x ",status2[j]);
                                        }
                                        printf("\n");

                                        //ecrire(controler.sock,status2,12+strlen(clients[0].name)+strlen(clients[1].name));

                                        ///MESSAGE END = fin de partie
                                        end[0] = 0x55;
                                        end[1] = 0;
                                        end[2] = 0x04;
                                        end[3] = end[2] & 0xff;
                                        ecrire(clients[0].sock, end,4);
                                        ecrire(clients[1].sock, end,4);
                                        //ecrire(controler.sock, end,4);
                                        init_plateau(plateau);

                                        for (j = 0; j < actual; j++)
                                        {
                                            clients[j].score=0;
                                            clients[j].time_reflexion=0;
                                        }

                                        printf("13");
                                        //free(status2);
                                        printf("14");
                                    }
                                }
                            }
                            strcpy (buffer, "");
                            break;
                        }
                    }
                    printf("JE SORS DE LA BOUCLE\n");
                }

            }
        }
    }
}
