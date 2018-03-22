#ifndef SERVER_H
#define SERVER_H

#include <extra.h>

void HostService()
{
    // DATOS DEL SOCKET --------------------------------------------------

    struct sockaddr_in stSockAddr;
    int port;
    string host_name;
    char *buffer;
    int n;

    // CREANDO EL SOCKET --------------------------------------------------

    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD)
    {
        perror("ERROR: Can not create socket");
        exit(EXIT_FAILURE);
    }

    // INGRESANDO DATOS PARA LA CONEXION --------------------------------------------------

    cout << "Username: ";
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

    cout << "----- You're listening now -----" << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    for(;;)
    {
        string message = "";
        buffer = ToChar(message);
        string end_chat;

        int ConnectFD;
        ConnectFD = accept(SocketFD, NULL, NULL);

        do
        {
            if (0 > ConnectFD)
            {
                perror("ERROR: Accept failed");
                close(SocketFD);
                exit(EXIT_FAILURE);
            }

            // LEYENDO UN MENSAJE --------------------------------------------------

            n = read(ConnectFD, buffer, 255);
            if (n < 0) perror("ERROR: Reading from socket");

            end_chat = ToString(buffer);

            cout << endl << buffer << endl;

            // ESCRIBIENDO UN MENSAJE --------------------------------------------------

            cout << host_name << ": ";
            std::getline(std::cin, message);
            message = host_name + ": " + message;
            buffer = ToChar(message);

            n = write(ConnectFD, buffer, message.size());
            if (n < 0) perror("ERROR: Writing to socket");
        }
        while (end_chat[end_chat.size()-1] != '#');

        shutdown(ConnectFD, SHUT_RDWR);
        close(ConnectFD);
    }

    close(SocketFD);
}

#endif
