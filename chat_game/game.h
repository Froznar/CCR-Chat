#ifndef GAME_H
#define GAME_H

#include <file.h>

using namespace std;

const int JUGADOR_1 = 1;
const int JUGADOR_2 = 2;

// --------------------------------------------------

void ActionG_PACKAGE(char* buffer, int jugador, int x, int y)
{
    // ARMANDO LA ESTRUCTURA DEL PAQUETE --------------------

        int beginIndex = 0;
        Insert_INT(buffer, beginIndex, 7, 4); // 4B: TAMANO DEL PAQUETE

        beginIndex += 4;
        buffer[beginIndex] = 'G'; // 1B: ACTION

        beginIndex += 1;
        Insert_INT(buffer, beginIndex, jugador, 1); // 1B: JUGADOR

        beginIndex += 1;
        Insert_INT(buffer, beginIndex, x, 3); // 3B: X

        beginIndex += 3;
        Insert_INT(buffer, beginIndex, y, 3); // 3B: Y
}

#endif
