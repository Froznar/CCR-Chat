#ifndef SERVER_H
#define SERVER_H

#include <server_functions.h>

using namespace std;

void Server()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("[ Server ] Error, can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << endl << "==================================================" << endl << endl;
    cout << "[ Server ] Port: "; cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("[ Server ] Error, bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(SocketFD, 10))
    {
        perror("[ Server ] Error, listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    // INICIANDO EL SERVIDOR --------------------------------------------------

    vector< pair< pair<string, int>, pair<int, int> > > clients; // < USERNAME, SOCKET DESCRIPTOR > < X, Y >

    cout << green << "[ Server ] OK" << reset << endl;

    for(;;)
    {
        thread t_server(Server_THREAD, SocketFD, ref(clients));
        t_server.detach();

        do
        {
            // WAITING CHAT TO END
        }
        while (true);
    }

    close(SocketFD);
}

#endif
