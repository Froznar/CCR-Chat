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

    cout << "Username: ";
    cin >> client_name;
    cout << "Port: ";
    cin >> port;

    // CONFIGURANDO Y INICIANDO EL SOCKET --------------------------------------------------

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(port);
    Res = inet_pton(AF_INET, "192.168.1.37", &stSockAddr.sin_addr);

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

    cout << "----- You're connected now -----" << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    char* buffer;
    string message;
    string end_chat;

    do
    {
        if (0 > SocketFD)
        {
            perror("ERROR: Accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        // ESCRIBIENDO UN MENSAJE --------------------------------------------------

        cout << client_name << ": ";
        std::getline (std::cin, message);
        message = client_name + ": " + message;
        buffer = ToChar(message);

        n = write(SocketFD, buffer, message.size());

        // LEYENDO UN MENSAJE --------------------------------------------------

        n = read(SocketFD,buffer, 255);
        if (n < 0) perror("ERROR: Reading from socket");

        cout << endl << buffer << endl;

        end_chat = ToString(buffer);
    }
    while (end_chat[end_chat.size()-1] != '#');

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
