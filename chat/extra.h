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

/*      DEFINES
 * Aqui colocares código que se repita mucho
 * SIEMPRE coloca un comentario con lo que hace
 * De una linea sin ";" final
*/

//Limpia la consola
#define CLEAR cout << "\033[2J\033[1;1H"

/*      COLORES
 * Forma de uso(en este caso verde->"green")
   cout<<green<<"TU STRING"<<reset;
   reset regresa la consola a sus colores normales
   los colores deben estar en ingles
*/
const std::string reset("\033[0m");
const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string magenta("\033[0;35m");

using namespace std;

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
    char* buffer = new char[512];
    string message;
    int n;
    cout<<red <<"send started"<<reset<<endl;
    do{
        // ESCRIBIENDO UN MENSAJE --------------------------------------------------

        cout << user_name << ": ";
        cin >> message;
        message = user_name + ": " + message;

        message = CreateHeader(message.size()) + message; // PROTOCOLO: AGREGANDO EL TAMANO DEL MENSAJE AL INICIO

        buffer = new char[message.size() + 1];
        strcpy(buffer, message.c_str()); // PASANDO EL MENSAJE AL BUFFER

        cout << endl << "[Sending: '" << buffer << "']" << endl;
        for (int i = 0; i < message.size(); i++) if(buffer[i]=='#') end_chat = true;
        n = write(chat_socket, buffer, message.size());
        if (n < 0) perror("ERROR: Writing to socket");


    }while(end_chat == false);
}

void ChatRecive(int chat_socket, bool& end_chat)
{
    char* buffer = new char[512];
    int message_size;
    int n;
    cout<<red <<"recive started"<<reset<<endl;
    do{
        // LEYENDO UN MENSAJE --------------------------------------------------

        n = read(chat_socket, buffer, 4); // PROTOCOLO: LEYENDO LA CABECERA
        if (n < 0) perror("ERROR: Reading from socket");

        cout << endl << "[Receiving: '";
        for (int i = 0; i < 4; i++) cout << buffer[i];
        cout << "']" << endl;

        message_size = atoi(buffer); // PROTOCOLO: OBTENIENDO EL TAMANO DEL MENSAJE

        n = read(chat_socket, buffer, message_size);
        if (n < 0) perror("ERROR: Reading from socket");

        cout << endl;
        for (int i = 0; i < message_size; i++)
        {
            if(buffer[i]=='#') end_chat = true;
            else cout << buffer[i];
        }
        cout << endl;

    }while(end_chat == false);

}



#endif
