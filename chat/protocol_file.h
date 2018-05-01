#ifndef PROTOCOL_FILE_H
#define PROTOCOL_FILE_H

#include <fstream> // ifstream, ofstream
#include <protocol_functions.h>

using namespace std;

// ACCION F / D --------------------------------------------------

void ActionFD_PACKAGE(char* buffer, string fileName, string username, char action, int& packageSize) // action = 'F' or 'D'
{
    ifstream file(fileName, ifstream::binary);

    // OBTENIENDO EL TAMANO DEL ARCHIVO --------------------

    file.seekg(0, file.end);
    long fileSize = file.tellg();
    file.seekg (0);

    // TRANSFORMANDO EL ARCHIVO A BINARIO --------------------

    char* fileBinary = new char[fileSize];
    file.read(fileBinary, fileSize);

    // ARMANDO LA ESTRUCTURA DEL PAQUETE --------------------

        int beginIndex = 0;
        Insert_INT(buffer, beginIndex, fileName.size(), 4); // 4B: TAMANO DEL NOMBRE DEL ARCHIVO

        beginIndex += 4;
        buffer[beginIndex] = action; // 1B: ACCCION

        beginIndex += 1;
        Insert_INT(buffer, beginIndex, username.size(), 2); // 2B: TAMANO DEL USERNAME

        beginIndex += 2;
        Insert_STR(buffer, beginIndex, username); // nB: USERNAME

        beginIndex += username.size();
        Insert_STR(buffer, beginIndex, fileName); // nB: NOMBRE DEL ARCHIVO

        beginIndex += fileName.size();
        Insert_INT(buffer, beginIndex, fileSize, 4); // 4B: TAMANO DEL ARCHIVO

        beginIndex += 4;
        Insert_BIN(buffer, beginIndex, fileBinary, fileSize); // nB: ARCHIVO EN BINARIO

    // LIBERANDO MEMORIA Y CERRANDO EL ARCHIVO --------------------

    delete[] fileBinary;
    file.close();

    packageSize = beginIndex + fileSize;
}

#endif
