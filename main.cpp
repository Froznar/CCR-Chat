#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

string ToString(char* sab)
{
    string sabe(sab);
    return sabe;
}

char* ToChar(string str)
{
    char *y = new char[str.length() + 1];
    strcpy(y, str.c_str());
    return y;
}

void HostService()
{
    //Datos
    int puerto;
    string host_name;

    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char *buffer;
    int n;

    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }

    //Menu conexion
    cout<<"Antes de iniciar tu chat..."<<endl;
    cout<<"Select your nickname:  ";
    cin>>host_name;
    cout<<"Que puerto usaras? :  ";
    cin>>puerto;

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(puerto);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    cout<<"You're in a conversation now!"<<endl;

    //Bucle Principal
    string mensag="";
    buffer = ToChar(mensag);
    int ConnectFD;
    ConnectFD = accept(SocketFD, NULL, NULL);
    for(;;)
    {

      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }
     /* perform read write operations ... */
     //Read
     n = read(ConnectFD,buffer,255);
     if (n < 0) perror("ERROR reading from socket");
     //Print
     cout<<buffer<<endl;
     //Write
     cout<<"Escribe: ";
     cin>>mensag;
     mensag = host_name+"-->"+mensag;
     buffer = ToChar(mensag);
     n = write(ConnectFD,buffer,mensag.size());
     if (n < 0) perror("ERROR writing to socket");
     //Close
    buffer = ToChar("");
    }
    shutdown(ConnectFD, SHUT_RDWR);
    close(ConnectFD);
    close(SocketFD);
}

void ClientService()
{
    //Datos
    int puerto;
    string client_name;

    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    //Menu conexion
    cout<<"Antes de iniciar tu chat..."<<endl;
    cout<<"Select your nickname:  ";
    cin>>client_name;
    cout<<"Que puerto usaras? :  ";
    cin>>puerto;

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(puerto);
    Res = inet_pton(AF_INET, "192.168.160.193", &stSockAddr.sin_addr);//192.168.110.219 //.110.217 //

    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }


    /* perform read write operations ... */


    char* buffer;
    string mi_mensaje;
    for(;;)
    {
       if(0 > SocketFD)
       {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
       }
        cout<<" --> ";
        cin>>mi_mensaje;
        mi_mensaje = client_name+"-->"+mi_mensaje;
        buffer = ToChar(mi_mensaje);
        n = write(SocketFD, buffer, mi_mensaje.size());


        n = read(SocketFD,buffer,255);
        if (n < 0) perror("ERROR reading from socket");
        cout<< buffer <<endl;
        buffer = ToChar("");

    }



     /* perform read write operations ... */



    shutdown(SocketFD, SHUT_RDWR);

    close(SocketFD);
}

int main()
{
    cout << "Cool Chat!" << endl;
    int selector;
    cout << "Create[1] or Join[2] a conversation?";
    cin >> selector;
    if(selector==1)
    {
        HostService();
    }
    if(selector==2)
    {
        ClientService();
    }




    return 0;
}
