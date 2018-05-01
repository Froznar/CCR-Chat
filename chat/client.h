#ifndef CLIENT_H
#define CLIENT_H

#include <client_functions.h>

using namespace std;

void Client()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port;
    int Res;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("[ Client ] Error, can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << endl << "==================================================" << endl << endl;
    cout << "[ Client ] Port: "; cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr); // IP LOCAL: 127.0.0.1

    if (0 > Res)
    {
        perror("[ Client ] Error, first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    else if (0 == Res)
    {
        perror("[ Client ] Error, char string, second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("[ Client ] Error, connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    cout << green << "[ Client ] OK" << reset << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    if (0 > SocketFD)
    {
        perror("[ Client ] Error, accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    bool end_chat = false;

    // CREANDO Y INICIANDO LAS THREADS --------------------------------------------------

    vector< pair< string, pair<int, int> > > friends; // USERNAME < X, Y >

    thread t_send(ClientSEND_THREAD, SocketFD, ref(friends));
    thread t_read(ClientREAD_THREAD, SocketFD, ref(friends));

    t_send.detach();
    t_read.detach();

    do
    {
        // WAITING CHAT TO END
    }
    while (end_chat == false);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
