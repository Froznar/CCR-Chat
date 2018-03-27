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

    cout << endl << "----- You're listening now -----" << endl;

    // INICIANDO EL CHAT --------------------------------------------------

    char* buffer = new char[512];
    string message;
    int message_size;

    for(;;)
    {
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

            n = read(ConnectFD, buffer, 4); // PROTOCOLO: LEYENDO LA CABECERA
            if (n < 0) perror("ERROR: Reading from socket");

            cout << endl << "[Receiving: '";
            for (int i = 0; i < 4; i++) cout << buffer[i];
            cout << "']" << endl;

            message_size = atoi(buffer); // PROTOCOLO: OBTENIENDO EL TAMANO DEL MENSAJE

            n = read(ConnectFD, buffer, message_size);
            if (n < 0) perror("ERROR: Reading from socket");

            cout << endl;
            for (int i = 0; i < message_size; i++) cout << buffer[i];
            cout << endl;

            // ESCRIBIENDO UN MENSAJE --------------------------------------------------

            cout << host_name << ": ";
            cin >> message;
            message = host_name + ": " + message;

            message = CreateHeader(message.size()) + message; // PROTOCOLO: AGREGANDO EL TAMANO DEL MENSAJE AL INICIO

            buffer = new char[message.size() + 1];
            strcpy(buffer, message.c_str()); // PASANDO EL MENSAJE AL BUFFER

            cout << endl << "[Sending: '" << buffer << "']" << endl;

            n = write(ConnectFD, buffer, message.size());
            if (n < 0) perror("ERROR: Writing to socket");
        }
        while (true);

        shutdown(ConnectFD, SHUT_RDWR);
        close(ConnectFD);
    }

    close(SocketFD);
}

#endif
