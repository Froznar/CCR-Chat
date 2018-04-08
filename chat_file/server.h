#ifndef SERVER_H
#define SERVER_H

#include <extra.h>

void HostService()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port;
    string host_name;
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
    cin >> host_name;
    cout << "Port: ";
    cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("ERROR: Bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(SocketFD, 10))
    {
        perror("ERROR: Listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    // INICIANDO EL CHAT --------------------------------------------------

    for(;;)
    {
        cout << green << "----- You're listening now -----" << reset << endl;
        int ConnectFD;
        ConnectFD = accept(SocketFD, NULL, NULL);
        if (0 > ConnectFD)
        {
            perror("ERROR: Accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        bool end_chat = false;

        // CREANDO Y INICIANDO LAS THREADS --------------------------------------------------

        thread t_send(ChatSend, ConnectFD, host_name, ref(end_chat));
        thread t_recive(ChatRecive, ConnectFD, ref(end_chat));

        t_send.detach();
        t_recive.detach();

        do
        {
           // WAITING CHAT TO END
        }
        while (end_chat == false);

        shutdown(ConnectFD, SHUT_RDWR);
        close(ConnectFD);
    }

    close(SocketFD);
}

#endif
