#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <socket_functions.h>
#include <protocol_chat.h>
#include <protocol_file.h>
#include <protocol_game.h>

using namespace std;

mutex SERVER_MUTEX;

void PrintClients(vector< pair< pair<string, int>, pair<int, int> > >& clients)
{
    cout << endl << "[ Read ] Clients: ";

    for (int i = 0; i < clients.size(); i++)
    {
        SERVER_MUTEX.lock();

        cout << yellow << clients[i].first.first << reset << " ";

        SERVER_MUTEX.unlock();
    }

    cout << endl;
}

void ServerREAD_THREAD(int socketFD, vector< pair< pair<string, int>, pair<int, int> > >& clients)
{
    char* buffer = new char[buffer_size];
    int n;
    string my_name = "";

    while (true)
    {
        // LEYENDO LA CABEZARA --------------------------------------------------

        n = read(socketFD, buffer, 4);

        cout << endl << "==================================================" << endl << endl;

        if (n < 0) perror("[ Read ] Error, reading from socket");

        int header = Read_INT(buffer, 0, 4);
        cout << "[ Read ] Header: " << header << endl;

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(socketFD, buffer, 1);
        if (n < 0) perror("[ Read ] Error, reading from socket");

        char action = buffer[0];
        cout << "[ Read ] Action: " << action << endl;

        // ACCION L: LOGIN --------------------------------------------------

        if (action == 'L')
        {
            // LEYENDO EL USERNAME --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action L ] Error, reading from socket");

            string username = Read_STR(buffer, 0, header);
            cout << endl << "[ Read ][ Action L ] Username: " << username << endl;

            // ANADIENDO USUARIO A LA LISTA DE USUARIOS --------------------------------------------------

            SERVER_MUTEX.lock();

            clients.push_back( make_pair( make_pair(username, socketFD), make_pair(100, 100) ) );
            my_name = username;

            SERVER_MUTEX.unlock();

            PrintClients(clients);
        }

        // ACCION P: LISTA DE USUARIOS --------------------------------------------------

        else if (action == 'P')
        {
            // JUNTANDO LISTA DE USUARIOS --------------------------------------------------

            string list_users = "";

            for (int i = 0; i < clients.size(); i++)
                list_users += clients[i].first.first + " ";

            // RESPONDIENDO --------------------------------------------------

            int packageSize = 0;
            ActionR_PACKAGE(buffer, list_users, packageSize);

            n = write(socketFD, buffer, packageSize);
            if (n < 0) perror("[ Write ][ Action P ] Error, write in socket");

            cout << endl << "[ Read ][ Action P ] OK" << endl;
        }

        // ACCION C: ENVIAR MENSAJE A OTRO USUARIO --------------------------------------------------

        else if (action == 'C')
        {
            // LEYENDO EL TAMANO DEL RECEPTOR --------------------------------------------------

            n = read(socketFD, buffer, 2);
            if (n < 0) perror("[ Read ][ Action C ] Error, reading from socket");

            int receiver_size = Read_INT(buffer, 0, 2);
            cout << endl << "[ Read ][ Action C ] Receiver size: " << receiver_size << endl;

            // LEYENDO EL USERNAME --------------------------------------------------

            n = read(socketFD, buffer, receiver_size);
            if (n < 0) perror("[ Read ][ Action C ] Error, reading from socket");

            string receiver = Read_STR(buffer, 0, receiver_size);
            cout << "[ Read ][ Action C ] Receiver: " << receiver << endl;

            // LEYENDO EL MENSAJE --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action C ] Error, reading from socket");

            string message = Read_STR(buffer, 0, header);
            cout << "[ Read ][ Action C ] Message: " << message << endl;

            // BUSCANDO AL RECEPTOR DEL MENSAJE --------------------------------------------------

            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].first.first == receiver)
                {
                    int socketFD_receiver = clients[i].first.second;

                    // RESPONDIENDO --------------------------------------------------

                    int packageSize = 0;
                    ActionR_PACKAGE(buffer, my_name + ": " + message, packageSize);

                    n = write(socketFD_receiver, buffer, packageSize);
                    if (n < 0) perror("[ Write ][ Action C ] Error, write in socket");

                    cout << "[ Read ][ Action C ] OK" << endl;

                    break;
                }
            }
        }

        // ACCION INVALIDA --------------------------------------------------

        else
        {
            cout << endl << "[ Read ] Error, invalid action" << endl;
        }
    }
}

void Server_THREAD(int socketFD, vector< pair< pair<string, int>, pair<int, int> > >& clients)
{
    while (true)
    {
        int ConnectFD;
        ConnectFD = accept(socketFD, NULL, NULL);

        thread t_read(ServerREAD_THREAD, ConnectFD, ref(clients));
        t_read.detach();
    }
}

#endif
