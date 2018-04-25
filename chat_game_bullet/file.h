#ifndef FILE_H
#define FILE_H

#include <fstream> // ifstream, ofstream

using namespace std;

// --------------------------------------------------

void Insert_INT(char* buffer, int beginIndex, int number, int bits)
{
    string number_STR = to_string(number);
    int i, j;

    for (i = beginIndex; i < ( beginIndex + (bits - number_STR.size()) ); i++)
        buffer[i] = '0';

    for(j = 0; j < number_STR.size(); j++, i++)
        buffer[i] = number_STR[j];
}

int Read_INT(char* buffer, int beginIndex, int bits)
{
    string number_STR = "";
    for (int i = beginIndex; i < (beginIndex + bits); i++)
        number_STR += buffer[i];

    return atoi(number_STR.c_str());
}

// --------------------------------------------------

void Insert_STR(char* buffer, int beginIndex, string text)
{
    for (int i = beginIndex; i < ( beginIndex + text.size() ); i++)
        buffer[i] = text[i - beginIndex];
}

string Read_STR(char* buffer, int beginIndex, int bits)
{
    string text = "";
    for (int i = beginIndex; i < bits; i++)
        text += buffer[i];

    return text;
}

// --------------------------------------------------

void Insert_BIN(char* buffer, int beginIndex, char* fileBinary, int fileSize)
{
    int j = 0;

    for (int i = beginIndex; i < (beginIndex + fileSize); i++, j++)
        buffer[i] = fileBinary[j];
}

void Read_BIN(char* buffer, int beginIndex, string fileName, int bits)
{
    ofstream file(fileName, ofstream::binary);

    char* fileBinary = new char[bits];

    for (int i = 0; i < bits; i++)
        fileBinary[i] = buffer[beginIndex + i];

    file.write(fileBinary, bits);

    delete[] fileBinary;
    file.close();
}

// --------------------------------------------------

void ActionFD_PACKAGE(char* buffer, string fileName, string nickname, char action, int& packageSize) // action = 'F' o 'D'
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
        Insert_INT(buffer, beginIndex, fileName.size(), 4); // 4B: FILE NAME SIZE

        beginIndex += 4;
        buffer[beginIndex] = action; // 1B: ACTION

        beginIndex += 1;
        Insert_INT(buffer, beginIndex, nickname.size(), 2); // 2B: NICKNAME SIZE

        beginIndex += 2;
        Insert_STR(buffer, beginIndex, nickname); // nB: NICKNAME

        beginIndex += nickname.size();
        Insert_STR(buffer, beginIndex, fileName); // nB: FILE NAME

        beginIndex += fileName.size();
        Insert_INT(buffer, beginIndex, fileSize, 4); // 4B: FILE SIZE

        beginIndex += 4;
        Insert_BIN(buffer, beginIndex, fileBinary, fileSize); // nB: FILE

    // LIBERANDO MEMORIA Y CERRANDO EL ARCHIVO --------------------

    delete[] fileBinary;
    file.close();

    packageSize = beginIndex + fileSize - 1;
}

#endif
