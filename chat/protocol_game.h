#ifndef PROTOCOL_GAME_H
#define PROTOCOL_GAME_H

#include <protocol_functions.h>

using namespace std;

const int DIRECTION_UP = 0;
const int DIRECTION_DOWN = 1;
const int DIRECTION_LEFT = 2;
const int DIRECTION_RIGHT = 3;

// ACCION G --------------------------------------------------

void ActionG_PACKAGE(char* buffer, string username, int x, int y, int& packageSize) // ACCION JUEGO
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, username.size(), 4); // 4B: TAMANO DEL USERNAME

    beginIndex += 4;
    buffer[beginIndex] = 'G'; // 1B: ACCION

    beginIndex += 1;
    Insert_STR(buffer, beginIndex, username); // nB: USERNAME

    beginIndex += username.size();
    Insert_INT(buffer, beginIndex, x, 3); // 3B: X

    beginIndex += 3;
    Insert_INT(buffer, beginIndex, y, 3); // 3B: Y

    packageSize = beginIndex += 3;
}

// ACCION B --------------------------------------------------

void ActionB_PACKAGE(char* buffer, int x, int y, int direction, string username, int& packageSize) // ACCION BALA
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, username.size(), 4); // 4B: TAMANO DEL USERNAME

    beginIndex += 4;
    buffer[beginIndex] = 'B'; // 1B: ACTION

    beginIndex += 1;
    Insert_STR(buffer, beginIndex, username); // nB: USERNAME

    beginIndex += username.size();
    Insert_INT(buffer, beginIndex, x, 3); // 3B: POSICION X

    beginIndex += 3;
    Insert_INT(buffer, beginIndex, y, 3); // 3B: POSICION Y

    beginIndex += 3;
    Insert_INT(buffer, beginIndex, direction, 1); // 1B: DIRECCION DE LA BALA

    packageSize = beginIndex + 1;
}

// ACCION K --------------------------------------------------

void ActionK_PACKAGE(char* buffer, string username, int& packageSize) // ACCION MATAR
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, username.size(), 4); // 4B: TAMANO DEL USERNAME

    beginIndex += 4;
    buffer[beginIndex] = 'K'; // 1B: ACTION

    beginIndex += 1;
    Insert_STR(buffer, beginIndex, username); // nB: USERNAME

    packageSize = beginIndex + username.size();
}

#endif
