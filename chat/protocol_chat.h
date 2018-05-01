#ifndef PROTOCOL_CHAT_H
#define PROTOCOL_CHAT_H

#include <protocol_functions.h>

using namespace std;

// ACCION P --------------------------------------------------

void ActionP_PACKAGE(char* buffer, int& packageSize) // IMPRIMIR LISTA DE USUARIOS EN EL CHAT
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, 5, 4); // 4B: TAMANO DEL MENSAJE

    beginIndex += 4;
    buffer[beginIndex] = 'P'; // 1B: ACCION

    packageSize = beginIndex + 1;
}

// ACCION L --------------------------------------------------

void ActionL_PACKAGE(char* buffer, string username, int& packageSize) // LOGIN
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, username.size(), 4); // 4B: TAMANO DEL USERNAME

    beginIndex += 4;
    buffer[beginIndex] = 'L'; // 1B: ACCION

    beginIndex += 1;
    Insert_STR(buffer, beginIndex, username); // nB: USERNAME

    packageSize = beginIndex + username.size();
}

// ACCION C --------------------------------------------------

void ActionC_PACKAGE(char* buffer, string username, string message, int& packageSize) // ENVIAR MENSAJE A OTRO USUARIO
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, message.size(), 4); // 4B: TAMANO DEL MENSAJE

    beginIndex += 4;
    buffer[beginIndex] = 'C'; // 1B: ACCION

    beginIndex += 1;
    Insert_INT(buffer, beginIndex, username.size(), 2); // 2B: TAMANO DEL USERNAME

    beginIndex += 2;
    Insert_STR(buffer, beginIndex, username); // nB: USERNAME

    beginIndex += username.size();
    Insert_STR(buffer, beginIndex, message); // nB: MENSAJE

    packageSize = beginIndex + message.size();
}

// ACCION R --------------------------------------------------

void ActionR_PACKAGE(char* buffer, string message, int& packageSize) // ENVIAR MENSAJE A UN CLIENTE
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, message.size(), 4); // 4B: TAMANO DEL MENSAJE

    beginIndex += 4;
    buffer[beginIndex] = 'R'; // 1B: ACCION

    beginIndex += 1;
    Insert_STR(buffer, beginIndex, message); // nB: MENSAJE

    packageSize = beginIndex + message.size();
}

// ACCION E --------------------------------------------------

void ActionE_PACKAGE(char* buffer, int& packageSize) // SALIR DEL CHAT
{
    int beginIndex = 0;
    Insert_INT(buffer, beginIndex, 5, 4); // 4B: TAMANO DEL MENSAJE

    beginIndex += 4;
    buffer[beginIndex] = 'E'; // 1B: ACCION

    packageSize = beginIndex + 1;
}

#endif
