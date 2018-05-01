#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <stdio.h>
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
    int n = 1;
    string my_name = "";
    bool online = true;

    while (online)
    {
        // LEYENDO LA CABEZARA --------------------------------------------------

        n = read(socketFD, buffer, 4);

        cout << endl << "==================================================" << endl;

        cout << "[ Socket " << socketFD << " ]" << endl << endl;

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

            clients.push_back( make_pair( make_pair(username, socketFD), make_pair(0, 0) ) );
            my_name = username;

            SERVER_MUTEX.unlock();

            PrintClients(clients);

            cout << endl;

            // ENVIANDO LA LISTA DE CLIENTES AL NUEVO CLIENTE --------------------------------------------------

            int packageSize = 0;
            string username_i = "";

            for (int i = 0; i < clients.size() - 1; i++)
            {
                username_i = clients[i].first.first;
                ActionN_PACKAGE(buffer, username_i, packageSize);

                n = write(socketFD, buffer, packageSize);
                if (n < 0) perror("[ Write ][ Action N ] Error, write in socket");

                cout << "[ Write ][ Action N ] To socket " << socketFD << ": OK" << endl;
            }

            // AVISANDO A TODOS LOS CLIENTES DEL NUEVO CLIENTE --------------------------------------------------

            int socketFD_receiver = 0;

            for (int i = 0; i < clients.size(); i++)
            {
                socketFD_receiver = clients[i].first.second;

                ActionN_PACKAGE(buffer, my_name, packageSize);

                n = write(socketFD_receiver, buffer, packageSize);
                if (n < 0) perror("[ Write ][ Action N ] Error, write in socket");

                cout << "[ Write ][ Action N ] To socket " << socketFD_receiver << ": OK" << endl;
            }
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

                    cout << endl << "[ Write ][ Action C ] OK" << endl;

                    break;
                }
            }
        }

        // ACCION F: ENVIAR ARCHIVO A OTRO USUARIO --------------------------------------------------

        else if (action == 'F')
        {
            // LEYENDO EL TAMANO DEL RECEPTOR --------------------------------------------------

            n = read(socketFD, buffer, 2);
            if (n < 0) perror("[ Read ][ Action F ] Error, reading from socket");

            int receiver_size = Read_INT(buffer, 0, 2);
            cout << endl << "[ Read ][ Action F ] Receiver size: " << receiver_size << endl;

            // LEYENDO EL USERNAME --------------------------------------------------

            n = read(socketFD, buffer, receiver_size);
            if (n < 0) perror("[ Read ][ Action F ] Error, reading from socket");

            string receiver = Read_STR(buffer, 0, receiver_size);
            cout << "[ Read ][ Action F ] Receiver: " << receiver << endl;

            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, header);
            if (n < 0) perror("[ Read ][ Action F ] Error, reading from socket");

            string fileName = Read_STR(buffer, 0, header);
            cout << "[ Read ][ Action F ] File name: " << fileName << endl;

            string newFileName = "_";
            newFileName += fileName;
            fileName = newFileName;

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, 4);
            if (n < 0) perror("[ Read ][ Action F ] Error, reading from socket");

            int file_size = Read_INT(buffer, 0, 4);
            cout << "[ Read ][ Action F ] File size: " << file_size << endl;

            // LEYENDO EL ARCHIVO --------------------------------------------------

            n = read(socketFD, buffer, file_size);
            if (n < 0) perror("[ Read ][ Action F ] Error, reading from socket");

            Read_BIN(buffer, 0, fileName, file_size);
            cout << "[ Read ][ Action F ] File: OK" << endl;

            // BUSCANDO AL RECEPTOR DEL MENSAJE --------------------------------------------------

            for (int i = 0; i < clients.size(); i++)
            {
                if (clients[i].first.first == receiver)
                {
                    int socketFD_receiver = clients[i].first.second;

                    // RESPONDIENDO --------------------------------------------------

                    int packageSize = 0;
                    ActionFD_PACKAGE(buffer, fileName, my_name, 'D', packageSize);
                    remove(fileName.c_str()); // ELIMINANDO EL ARCHIVO DEL SERVIDOR

                    n = write(socketFD_receiver, buffer, packageSize);
                    if (n < 0) perror("[ Write ][ Action F ] Error, write in socket");

                    cout << endl << "[ Write ][ Action F ] OK" << endl;

                    break;
                }
            }
        }

        // ACCION E: SALIR DEL CHAT --------------------------------------------------

        else if (action == 'E')
        {
            // AVISANDO A TODOS LOS CLIENTES --------------------------------------------------

            string message = " has disconnected from the chat";
            int socketFD_receiver = 0;

            cout << endl;

            for (int i = 0; i < clients.size(); i++)
            {
                socketFD_receiver = clients[i].first.second;

                // RESPONDIENDO --------------------------------------------------

                int packageSize = 0;
                ActionR_PACKAGE(buffer, my_name + message, packageSize);

                n = write(socketFD_receiver, buffer, packageSize);
                if (n < 0) perror("[ Write ][ Action E ] Error, write in socket");

                cout << "[ Write ][ Action N ] To socket " << socketFD_receiver << ": OK" << endl;
            }

            online = false;
        }

        // ACCION INVALIDA --------------------------------------------------

        else
        {
            cout << endl << "[ Read ] Error, invalid action" << endl;
            online = false;
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
