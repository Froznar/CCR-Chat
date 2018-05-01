#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <socket_functions.h>
#include <protocol_chat.h>
#include <protocol_file.h>
#include <protocol_game.h>

using namespace std;

mutex CLIENT_MUTEX;

const int OPTION_LOGIN = 0;
const int OPTION_PRINT_USERS = 1;
const int OPTION_SEND_MESSAGE = 2;
const int OPTION_SEND_FILE = 3;
const int OPTION_START_GAME = 4;
const int OPTION_END_CHAT = 5;

void ClientSEND_THREAD(int socketFD, vector< pair< string, pair<int, int> > >& friends)
{
    char* buffer = new char[buffer_size];
    int n;
    int option = -1;

    while(option != OPTION_END_CHAT)
    {
        cout << endl << "==================================================" << endl << endl;

        cout << "[ Write ] 0: Login" << endl
             << "[ Write ] 1: Print list of users" << endl
             << "[ Write ] 2: Send message" << endl
             << "[ Write ] 3: Send file" << endl
             << "[ Write ] 4: Start game" << endl
             << "[ Write ] 5: End chat" << endl << endl;

        cout << "[ Write ] Option: "; cin >> option;

        // ACCION L: LOGIN --------------------------------------------------

        if (option == OPTION_LOGIN)
        {
            cout << endl << "==================================================" << endl << endl;

            string username = "";
            cout << "[ Write ][ Login ] Username: "; cin >> username;

            int packageSize = 0;
            ActionL_PACKAGE(buffer, username, packageSize);

            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Write ][ Login ] Error, write in socket");

            // AGREGANDO A LA LISTA DE AMIGOS --------------------

            CLIENT_MUTEX.lock();

            friends.push_back( make_pair(username, make_pair(1, 1)) );

            CLIENT_MUTEX.unlock();

            cout << "[ Write ][ Login ] OK" << endl;
        }

        // ACCION P: IMPRIMIR LISTA DE USUARIOS EN EL CHAT --------------------------------------------------

        else if (option == OPTION_PRINT_USERS)
        {
            cout << endl << "==================================================" << endl << endl;

            int packageSize = 0;
            ActionP_PACKAGE(buffer, packageSize);

            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Write ][ List users ] Error, write in socket");

            cout << "[ Write ][ List users ] OK" << endl;
        }

        // ACCION C: ENVIAR MENSAJE A OTRO USUARIO --------------------------------------------------

        else if (option == OPTION_SEND_MESSAGE)
        {
            cout << endl << "==================================================" << endl << endl;

            string username = "";
            cout << "[ Write ][ Send message ] To: "; cin >> username;

            string message = "";
            cout << "[ Write ][ Send message ] Message: "; cin >> message;

            int packageSize = 0;
            ActionC_PACKAGE(buffer, username, message, packageSize);

            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Write ][ Send message ] Error, write in socket");

            cout << "[ Write ][ Send message ] OK" << endl;
        }

        // ACCION F: ENVIAR UN ARCHIVOS --------------------------------------------------

        else if (option == OPTION_SEND_FILE)
        {
            // -
        }

        // ACCION G: EMPEZAR EL JUEGO --------------------------------------------------

        else if (option == OPTION_START_GAME)
        {
            // -
        }

        // ACCION E: SALIR DEL CHAT --------------------------------------------------

        else if (option == OPTION_END_CHAT)
        {
            // -
        }

        else
        {
            cout << "[ Write ] Error, invalid option" << endl;
        }
    }
}

void ClientREAD_THREAD(int socketFD, vector< pair< string, pair<int, int> > >& friends)
{
    char* buffer = new char[buffer_size];
    int n;

    while (true)
    {
        // LEYENDO LA CABEZARA --------------------------------------------------

        n = read(socketFD, buffer, 4);

        cout << endl << endl << "==================================================" << endl << endl;

        if (n < 0) perror("[ Read ] Error, reading from socket");

        int header = Read_INT(buffer, 0, 4);
        cout << yellow << "[ Read ] Header: " << header << reset << endl;

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(socketFD, buffer, 1);
        if (n < 0) perror("[ Read ] Error, reading from socket");

        char action = buffer[0];
        cout << yellow << "[ Read ] Action: " << action << reset << endl;

        // ACCION R: LEYENDO MENSAJE --------------------------------------------------

        if (action == 'R')
        {
            // LEYENDO EL MENSAJE --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action R ] Error, reading from socket");

            string message = Read_STR(buffer, 0, header);
            cout << endl << yellow << "[ Read ][ Action R ] Message: " << message << reset << endl;
        }

        // ACCION INVALIDA --------------------------------------------------

        else
        {
            cout << endl << yellow << "[ Read ] Error, invalid action" << reset << endl;
        }

        cout << endl << "==================================================" << endl << endl;
    }
}

#endif
