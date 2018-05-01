#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <extra.h>
#include <game_protocols.h>

void PrintUsers(vector< pair< pair<string, int>, pair<int,int> > >& clients)
{
    for (int i=0; i<clients.size(); i++)
    {
        MAP_MUTEX.lock();
        cout << yellow << clients[i].first.first << reset <<endl;
        MAP_MUTEX.unlock();
    }
}

void UpdateUsers(vector< pair< pair<string, int>, pair<int,int> > > clients)
{
    cout<<green<<"-------UPDATING USERS-------"<< reset << endl;
    char* buffer = new char[256];
    string message="";
    int n;
    int usuario = clients.size()-1;
    //actualizando al nuevo usuario en los demas
    for (int i=0; i<clients.size(); i++)
    {
        MAP_MUTEX.lock();

        message = ActionR_PACKAGE(clients[i].first.first);
        buffer = new char[message.size() + 1];
        strcpy(buffer, message.c_str());
        cout<<"nombre: "<< clients[usuario].first.second<< endl;
        n = write(clients[usuario].first.second, buffer, message.size());
        if (n < 0) perror("ERROR: Writing to socket");

        MAP_MUTEX.unlock();
    }
    message = clients[usuario].first.first;
    //actualizando a los demas
    message = ActionR_PACKAGE(message);
    buffer = new char[message.size() + 1];
    strcpy(buffer, message.c_str());
    //AL nuevo todo
    for(int i=0; i<clients.size(); i++)
    {

        if(i!=usuario)
        {
            MAP_MUTEX.lock();

            n = write(clients[i].first.second, buffer, message.size());
            if (n < 0) perror("ERROR: Writing to socket");

            MAP_MUTEX.unlock();
        }
    }
    //Todos tiene al nuevo usuario
    // y el usuario tiene a todos
}

void Server_Listener(int client_socket, vector< pair< pair<string, int>, pair<int,int> > >& clients)
{
    char* buffer = new char[256];
    int n;
    bool end_thread = false;
    do{
        // LEYENDO LA CABEZARA --------------------------------------------------
        n = read(client_socket, buffer, 4);        
        if (n==0) end_thread = true;
        int header = Read_INT(buffer, 0, 4);

        cout << "Header: " << header << endl;

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(client_socket, buffer, 1);
        if (n < 0) perror("ERROR: Reading from socket");

        char action = buffer[0];

        // EJECUTANDO LA ACCION --------------------------------------------------
        // Action L: Login to game -------------------------
        if (action == 'L')
        {

            string bufferSubset = "";
            int nicknameSize = header;
            bufferSubset = "";

            n = read(client_socket, buffer, nicknameSize);
            if (n < 0) perror("ERROR: Reading from socket");
            for (int i = 0; i < nicknameSize; i++)
                bufferSubset += buffer[i];

            string nickname = bufferSubset;
            //New User
            MAP_MUTEX.lock();
            clients.push_back(make_pair( make_pair(nickname,client_socket),make_pair(1,1)));
            MAP_MUTEX.unlock();

            //Aqui se actualiza la lista de usuarios en los clientes
            UpdateUsers(clients);
            PrintUsers(clients);

        }
        if (action == 'B')
        {
            cout << "----- BALA ---------------------------------" << endl;

            // LEYENDO EL JUGADOR --------------------------------------------------

            n = read(client_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string jugador = Read_STR(buffer, 0, header);
            cout << "Jugador: " << jugador << endl;

            // LEYENDO X --------------------------------------------------

            n = read(client_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int x = Read_INT(buffer, 0, 3);
            cout << "X: " << x << endl;

            // LEYENDO Y --------------------------------------------------

            n = read(client_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int y = Read_INT(buffer, 0, 3);
            cout << "Y: " << y << endl;

            // LEYENDO DIRECCION --------------------------------------------------

            n = read(client_socket, buffer, 1);
            if (n < 0) perror("ERROR: Reading from socket");

            int dir = Read_INT(buffer, 0, 1);
            cout << "DIR: " << dir << endl;

            // ENVIANDO LA BALA A LOS OTROS USUARIOS --------------------------------------------------

            cout << "--------------- ACTUALIZANDO BALA ---------------------" << endl;

            int usuario = 0;
            for (int i=0; i<clients.size(); i++)
            {
                MAP_MUTEX.lock();
                if(clients[i].first.second == client_socket)
                {
                    usuario=i;
                }
                MAP_MUTEX.unlock();
            }

            cout << "usuario: " << usuario<< endl;
            string nombre = clients[usuario].first.first;

            ActionB_PACKAGE(buffer, x, y, dir, jugador);

            for (int i=0; i<clients.size(); i++)
            {
                n = write(clients[i].first.second, buffer, 5 + nombre.size() + 7);
                if (n < 0) perror("ERROR: Writing to socket");
            }

            cout << "--------------- BALA ENVIADA ---------------------" << endl;
        }
        if (action == 'G')
        {
            // LEYENDO EL JUGADOR --------------------------------------------------

            n = read(client_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string jugador = Read_STR(buffer, 0, header);
            cout << "Jugador: " << jugador << endl;

            // LEYENDO X --------------------------------------------------

            n = read(client_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int x = Read_INT(buffer, 0, 3);
            cout << "X: " << x << endl;

            // LEYENDO Y --------------------------------------------------

            n = read(client_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int y = Read_INT(buffer, 0, 3);
            cout << "Y: " << y << endl;

            //Update other users whit this informatión
            cout << "----------------ACTUALIZANDO---------------------" << endl;
            //buscamos a nuestro usuario
            int usuario = 0;
            for (int i=0; i<clients.size(); i++)
            {
                MAP_MUTEX.lock();
                if(clients[i].first.second == client_socket)
                {
                    usuario=i;
                }
                MAP_MUTEX.unlock();
            }
            cout << "usuario: " << usuario<< endl;
            //Actualicemos sus cordenadas a los demas
            string nombre = clients[usuario].first.first;
            //actualizando a los demas
            char* buffer = new char[256];

            clients[usuario].second.first=x;
            clients[usuario].second.second=y;
            cout << "x: " << clients[usuario].second.first << "y: " << clients[usuario].second.second <<endl;
            ActionG_PACKAGE(buffer, nombre, clients[usuario].second.first, clients[usuario].second.second);
            cout <<red<< buffer<<reset<< endl;
            for (int i=0; i<clients.size(); i++)
            {
                n = write(clients[i].first.second, buffer, 5 + nombre.size() + 6);
                if (n < 0) perror("ERROR: Writing to socket");
            }

        }

    }while(end_thread==false);

}

void ServerThread(int server_socket, vector< pair< pair<string, int>, pair<int,int> > >& clients)
{
    do{
        int ConnectFD;
        ConnectFD = accept(server_socket, NULL, NULL);
        thread esp(Server_Listener, ConnectFD, ref(clients));
        esp.detach();
    }while(true);
}
#endif // SERVER_FUNCTIONS_H
