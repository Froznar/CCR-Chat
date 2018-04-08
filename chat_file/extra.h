#ifndef EXTRA_H
#define EXTRA_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <file.h>

using namespace std;

#define CLEAR cout << "\033[2J\033[1;1H" // Limpia la consola

const string reset("\033[0m");
const string red("\033[0;31m");
const string green("\033[0;32m");
const string magenta("\033[0;35m");

string CreateHeader(int message_size_int)
{
    string header = "";
    string message_size_str = to_string(message_size_int);
    for (int i = 0; i < ( 4 - message_size_str.size() ); i++)
        header += "0";

    return header + message_size_str;
}

string ToString(char* input)
{
    string output(input);

    return output;
}

void ChatSend(int chat_socket, string user_name, bool& end_chat)
{
    char* buffer = new char[8192];
    string fileName;
    int packageSize = 0;
    int n;

    cout << red << "- Send started -" << reset << endl;

    do{

        cout << "File name: "; cin >> fileName;

        ActionFD_PACKAGE(buffer, fileName, user_name, 'F', packageSize);

        cout << endl << "[Sending: '" << buffer << "']" << endl;

        n = write(chat_socket, buffer, packageSize);
        if (n < 0) perror("ERROR: Writing to socket");

    }while (end_chat == false);
}

void ChatRecive(int chat_socket, bool& end_chat)
{
    char* buffer = new char[8192];
    int n;

    cout << red << "- Recive started -" << reset << endl;

    do{
        // LEYENDO LA CABEZARA --------------------------------------------------

        n = read(chat_socket, buffer, 4);
        if (n < 0) perror("ERROR: Reading from socket");

        int header = Read_INT(buffer, 0, 4);
        cout << "Header: " << header << endl;

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(chat_socket, buffer, 1);
        if (n < 0) perror("ERROR: Reading from socket");

        char action = buffer[0];
        cout << "Action: " << action << endl;

        // EJECUTANDO LA ACCION --------------------------------------------------

        if (action == 'F')
        {
            // LEYENDO EL TAMANO DEL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, 2);
            if (n < 0) perror("ERROR: Reading from socket");

            int nicknameSize = Read_INT(buffer, 0, 2);
            cout << "Nickname size: " << nicknameSize << endl;

            // LEYENDO EL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, nicknameSize);
            if (n < 0) perror("ERROR: Reading from socket");

            string nickname = Read_STR(buffer, 0, nicknameSize);
            cout << "Nickname: " << nickname << endl;

            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string fileName = Read_STR(buffer, 0, header);
            cout << "File name: " << fileName << endl;

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int fileSize = Read_INT(buffer, 0, 4);
            cout << "File size: " << fileSize << endl;

            // LEYENDO EL ARCHIVO BINARIO --------------------------------------------------

            n = read(chat_socket, buffer, fileSize);
            if (n < 0) perror("ERROR: Reading from socket");

            Read_BIN(buffer, 0, nickname + "_" + fileName, fileSize);
            cout << "The file has been saved as: " << nickname + "_" + fileName << endl;
        }

        int wait; cin >> wait;

    }while (end_chat == false);

}

#endif
