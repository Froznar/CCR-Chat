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
const int OPTION_PRINT_FRIENDS = 6;

void ClientSEND_THREAD(int socketFD, vector< pair< string, pair<int, int> > >& friends, bool& online)
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
             << "[ Write ] 5: End chat" << endl
             << "[ Write ] 6: Print friends" << endl << endl;

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

        // ACCION F: ENVIAR UN ARCHIVO --------------------------------------------------

        else if (option == OPTION_SEND_FILE)
        {
            cout << endl << "==================================================" << endl << endl;

            string fileName = "";
            cout << "[ Write ][ Send file ] File name: "; cin >> fileName;

            string username = "";
            cout << "[ Write ][ Send file ] Username: "; cin >> username;

            int packageSize = 0;
            bool fine = ActionFD_PACKAGE(buffer, fileName, username, 'F', packageSize);

            if (fine)
            {
                n = write(socketFD, buffer, packageSize);
                if (n < 0) perror("[ Write ][ Send file ] Error, write in socket");

                cout << "[ Write ][ Send file ] OK" << endl;
            }

            else
                cout << "[ Write ][ Send file ] Error" << endl;
        }

        // ACCION G: EMPEZAR EL JUEGO --------------------------------------------------

        else if (option == OPTION_START_GAME)
        {
            // -
        }

        // ACCION E: SALIR DEL CHAT --------------------------------------------------

        else if (option == OPTION_END_CHAT)
        {
            cout << endl << "==================================================" << endl << endl;

            int packageSize = 0;
            ActionE_PACKAGE(buffer, packageSize);

            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Write ][ End chat ] Error, write in socket");

            cout << "[ Write ][ End chat ] OK" << endl;

            online = false;
        }

        // IMPRIMIR AMIGOS --------------------------------------------------

        else if (option == OPTION_PRINT_FRIENDS)
        {
            cout << endl << "==================================================" << endl << endl;

            cout << "[ Friends ]";

            for (int i = 0; i < friends.size(); i++)
                cout << " <" << friends[i].first
                     << " < " << friends[i].second.first << " " << friends[i].second.second << " >> |";

            cout << endl;
        }

        else
        {
            cout << "[ Write ] Error, invalid option" << endl;
        }
    }
}

void ClientREAD_THREAD(int socketFD, vector< pair< string, pair<int, int> > >& friends, bool& online)
{
    char* buffer = new char[buffer_size];
    int n;

    while (online)
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
            cout << endl << yellow << "[ Read ][ Action R ] " << message << reset << endl;
        }

        // ACCION N: ACTUALIZANDO LISTA DE AMIGOS --------------------------------------------------

        if (action == 'N')
        {
            // LEYENDO EL MENSAJE --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action N ] Error, reading from socket");

            string username = Read_STR(buffer, 0, header);
            cout << endl << yellow << "[ Read ][ Action N ] There is a new user in the chat: " << username << reset << endl;

            CLIENT_MUTEX.lock();

            friends.push_back( make_pair(username, make_pair(0, 0)) );

            CLIENT_MUTEX.unlock();
        }

        // ACCION D: LEYENDO ARCHIVO --------------------------------------------------

        else if (action == 'D')
        {
            // LEYENDO EL TAMANO DEL RECEPTOR --------------------------------------------------

            n = read(socketFD, buffer, 2);
            if (n < 0) perror("[ Read ][ Action D ] Error, reading from socket");

            int sender_size = Read_INT(buffer, 0, 2);
            cout << endl << yellow << "[ Read ][ Action D ] Sender size: " << sender_size << reset << endl;

            // LEYENDO EL USERNAME --------------------------------------------------

            n = read(socketFD, buffer, sender_size);
            if (n < 0) perror("[ Read ][ Action D ] Error, reading from socket");

            string sender = Read_STR(buffer, 0, sender_size);
            cout << yellow << "[ Read ][ Action D ] Sender: " << sender << reset << endl;

            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action D ] Error, reading from socket");

            string fileName = Read_STR(buffer, 0, header);
            cout << yellow << "[ Read ][ Action D ] File name: " << fileName << reset << endl;

            string newFileName = sender;
            newFileName += fileName;
            fileName = newFileName;

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Read ][ Action D ] Error, reading from socket");

            int file_size = Read_INT(buffer, 0, 4);
            cout << yellow << "[ Read ][ Action D ] File size: " << file_size << reset << endl;

            // LEYENDO EL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, file_size);
            if (n < 0) perror("[ Read ][ Action D ] Error, reading from socket");

            Read_BIN(buffer, 0, fileName, file_size);
            cout << yellow << "[ Read ][ Action D ] File: OK" << reset << endl;

            cout << endl << yellow << "[ Read ] The file has been saved with the name: " << fileName << reset << endl;
        }

        // ACCION INVALIDA --------------------------------------------------

        else
        {
            cout << endl << yellow << "[ Read ] Error, invalid action" << reset << endl;
            online = false;
        }

        cout << endl << "==================================================" << endl << endl;
    }
}

#endif
