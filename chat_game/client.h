#ifndef CLIENT_H
#define CLIENT_H

#include <extra.h>

void ClientService()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port;
    string client_name;
    int Res;
    int n;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("ERROR: Can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << endl << "Username: ";
    cin >> client_name;
    cout << "Port: ";
    cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("ERROR: First parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("ERROR: Char string, second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("ERROR: Connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    cout << green << "----- You're connected now -----" << reset << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    if (0 > SocketFD)
    {
        perror("ERROR: Accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    bool end_chat = false;

    // CREANDO Y INICIANDO LAS THREADS --------------------------------------------------

    int GAME_TIPO = GAME_TIPO_CLIENTE;

    thread t_send(ChatSend, SocketFD, client_name, ref(end_chat), GAME_TIPO);
    thread t_recive(ChatRecive, SocketFD, ref(end_chat));

    t_send.detach();
    t_recive.detach();

    do
    {
       // WAITING CHAT TO END
    }
    while (end_chat == false);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
