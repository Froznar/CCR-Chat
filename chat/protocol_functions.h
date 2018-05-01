#ifndef PROTOCOL_FUNCTIONS_H
#define PROTOCOL_FUNCTIONS_H

#include <fstream> // ifstream, ofstream

using namespace std;

// INSERTAR ENTERO EN EL BUFFER --------------------------------------------------

void Insert_INT(char* buffer, int beginIndex, int number, int bits)
{
    string number_STR = to_string(number);
    int i, j;

    for (i = beginIndex; i < ( beginIndex + (bits - number_STR.size()) ); i++)
        buffer[i] = '0';

    for(j = 0; j < number_STR.size(); j++, i++)
        buffer[i] = number_STR[j];
}

// LEER ENTERO EN EL BUFFER --------------------------------------------------

int Read_INT(char* buffer, int beginIndex, int bits)
{
    string number_STR = "";
    for (int i = beginIndex; i < (beginIndex + bits); i++)
        number_STR += buffer[i];

    return atoi(number_STR.c_str());
}

// INSERTAR CADENA EN EL BUFFER --------------------------------------------------

void Insert_STR(char* buffer, int beginIndex, string text)
{
    for (int i = beginIndex; i < ( beginIndex + text.size() ); i++)
        buffer[i] = text[i - beginIndex];
}

// LEER CADENA EN EL BUFFER --------------------------------------------------

string Read_STR(char* buffer, int beginIndex, int bits)
{
    string text = "";
    for (int i = beginIndex; i < bits; i++)
        text += buffer[i];

    return text;
}

// INSERTAR ARCHIVO EN EL BUFFER --------------------------------------------------

void Insert_BIN(char* buffer, int beginIndex, char* fileBinary, int fileSize)
{
    int j = 0;

    for (int i = beginIndex; i < (beginIndex + fileSize); i++, j++)
        buffer[i] = fileBinary[j];
}

// LEER ARCHIVO EN EL BUFFER --------------------------------------------------

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

#endif
