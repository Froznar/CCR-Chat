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

    cout << endl << "----- You're connected now -----" << endl << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    char* buffer = new char[512];
    string message;
    int message_size;

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
        cin >> message;
        message = client_name + ": " + message;

        message = CreateHeader(message.size()) + message; // PROTOCOLO: AGREGANDO EL TAMANO DEL MENSAJE AL INICIO

        buffer = new char[message.size() + 1];
        strcpy(buffer, message.c_str()); // PASANDO EL MENSAJE AL BUFFER

        cout << endl << "[Sending: '" << buffer << "']" << endl;

        n = write(SocketFD, buffer, message.size());

        // LEYENDO UN MENSAJE --------------------------------------------------

        n = read(SocketFD, buffer, 4); // PROTOCOLO: LEYENDO LA CABECERA
        if (n < 0) perror("ERROR: Reading from socket");

        cout << endl << "[Receiving: '";
        for (int i = 0; i < 4; i++) cout << buffer[i];
        cout << "']" << endl;

        message_size = atoi(buffer); // PROTOCOLO: OBTENIENDO EL TAMANO DEL MENSAJE

        n = read(SocketFD, buffer, message_size);
        if (n < 0) perror("ERROR: Reading from socket");

        cout << endl;
        for (int i = 0; i < message_size; i++) cout << buffer[i];
        cout << endl;
    }
    while (true);

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
}

#endif
