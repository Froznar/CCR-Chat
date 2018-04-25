#ifndef SERVER_H
#define SERVER_H

#include <extra.h>
#include <server_functions.h>

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

    // INICIANDO EL SERVIDOR --------------------------------------------------

    vector< pair< pair<string, int>, pair<int,int> > > m_clients; //nickname - socket descriptor , x - y

    cout << green << "----- SERVER ONLINE -----" << reset << endl;

    for(;;)
    {
        //Start Server Functionality: CHAT
        thread t_ServerThread(ServerThread, SocketFD, ref(m_clients));
        t_ServerThread.detach();
        do
        {
           // WAITING CHAT TO END
        }
        while (true);

    }

    close(SocketFD);
}

#endif
