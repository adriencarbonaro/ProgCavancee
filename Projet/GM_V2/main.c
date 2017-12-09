/***********
    NE PAS ENLEVER CE HEADER !!

    PROJET REVERSI

    OWNERS : Paul SÉRAGÉ, Victor LAM, Alexis DE LAUNAY, & PAS TOI

    Copyright © NE PAS ENLEVER CE HEADER SOUS PEINE DE POURSUITES JUDICIAIRES!


*********/

#include "GameMaster.h"

/// REGLES OTHELLO

/// 1) Transformation lorsque pion adverse encadré (horizontal ou vertical ou diagonal)
    // NB : possibilité de retourner plusieurs segments en un seul coup (et horizontal et diagonal --> tout checker)
    // NB2 : n'est transformé que le(s) pion(s) adverse encadrés PAR RAPPORT au pion de référence = qui vient d'être joué
/// 1) Obligé de transformer un pion adverse lorsqu'on joue --> si impossible, on passe


int main()
{
    //initia();
    /*Client cli;
    cli.name='P';
    cli.port=8888;
    corps(cli);*/
    //fin();

    srand(time(NULL));
    initia();
    application();
    fin();
    return 0;
}
