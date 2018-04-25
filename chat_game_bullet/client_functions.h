#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <extra.h>

#endif // CLIENT_FUNCTIONS_H

//Globales
vector<WIN> monstruos;
vector<string> mapa;
int vida=5;

struct bala_info
{
    int _begin_x;
    int _begin_y;
    int _direccion;
    string _jugador;

    bala_info(int begin_x, int begin_y, int direccion, string jugador)
    {
        _begin_x = begin_x;
        _begin_y = begin_y;
        _direccion = direccion;
        _jugador = jugador;
    }
};

vector<WIN> balas;
vector<bala_info> balas_info;

void LogIn(string nickname, int socket)
{
    char* buffer = new char[512];
    string message;
    int n;

    message=ActionL_PACKAGE(nickname);
    buffer = new char[message.size() + 1];
    strcpy(buffer, message.c_str()); // PASANDO EL MENSAJE AL BUFFER
    cout << endl << "[Sending: '" << buffer << "']" << endl;
    n = write(socket, buffer, message.size());

    //inicializar mapa 60x16
    for(int i=0;i<15;i++)
    {
        mapa.push_back("0000000000000000000000000000000000000000000000000000000000");
    }
}
//actualizar amigos
void UpdateFriends(vector<pair<string, pair<int,int> >>& amigos, string usuario)
{
    string message="";
    int n;

    //agregarlo
    CLIENT_MUTEX.lock();
    amigos.push_back(make_pair(usuario, make_pair(1,1)));
    CLIENT_MUTEX.unlock();

    WIN monster;
    init_win_params(&monster);
    print_win_params(&monster);
    monster.startx = monster.startx+0;
    create_box(&monster, TRUE);

    CLIENT_MUTEX.lock();
    monstruos.push_back(monster);
    CLIENT_MUTEX.unlock();
    //inicializarlo
}

void Pos_Thread(vector<pair<string, pair<int,int> >>& amigos)
{
    while(true)
    {
        // REVISANDO LAS BALAS --------------------------------------------

        for(int i=0; i<balas.size();i++)
        {
             create_bullet(&balas[i], FALSE);
        }
        for(int i=0; i<balas.size();i++)
        {
            balas[i].startx = balas_info[i]._begin_x;
            balas[i].starty = balas_info[i]._begin_y;
            //if(mapa[balas_info[i]._begin_y][balas_info[i]._begin_x-1]=='P')
                //vida--;
            //mapa[balas_info[i]._begin_y][balas_info[i]._begin_x-1]='O';
            if (balas_info[i]._direccion == BALA_ABAJO)
                balas_info[i]._begin_y++;
            else if (balas_info[i]._direccion == BALA_ARRIBA)
                balas_info[i]._begin_y--;
            else if (balas_info[i]._direccion == BALA_IZQUIERDA)
                balas_info[i]._begin_x--;
            else if (balas_info[i]._direccion == BALA_DERECHA)
                balas_info[i]._begin_x++;
            //mapa[balas_info[i]._begin_y][balas_info[i]._begin_x-1]='B';
        }
        for(int i=0; i<balas.size();i++)
        {
             if (balas[i].startx < getmaxx(stdscr) - 10 and
                 balas[i].starty < getmaxy(stdscr) - 4 and
                 balas[i].startx >= 0 and
                 balas[i].starty >= 0)
             {
                 create_bullet(&balas[i], TRUE);

                 for(int i=0; i<monstruos.size();i++)
                 {
                      create_box(&monstruos[i], TRUE);
                 }
             }
        }

        sleep(1);
    }
}



void Client_Send_Thread(int chat_socket, vector<pair<string, pair<int,int> >>& amigos)
{
    char* buffer = new char[256];
    string message;
    int n;
    int option;


    cout<<red <<"send started"<<reset<<endl;
    do{
        // MENU --------------------------------------------------
        cout << magenta << "----- MENU -----" << endl;
        cout << "[1] -> Log in" << endl;
        cout << "[2] -> Play" << reset << endl;
        
        cin>>option;

        string client_name;

        if(option == 1)
        {
            // INICIANDO EL LOG IN --------------------------------------------------
            cout << yellow << "----- Logging In -----" << reset << endl;
            

            cout << endl << "Username: ";
            cin >> client_name;
            LogIn(client_name, chat_socket);
            CLIENT_MUTEX.lock();
            amigos.push_back(make_pair(client_name, make_pair(1,1)));
            CLIENT_MUTEX.unlock();

            WIN monster;
            init_win_params(&monster);
            print_win_params(&monster);
            monster.startx = monster.startx+0;
            create_box(&monster, TRUE);

            CLIENT_MUTEX.lock();
            monstruos.push_back(monster);
            CLIENT_MUTEX.unlock();

            //ya estas en el juego
            //colocando jugador en la matriz
            mapa[0,0] = 'P';
        }
        else if(option == 2)
        {
            cout << yellow << "----- List Of Users -----" << reset << endl;

            char* buffer = new char[256];
            string fileName;
            int packageSize = 0;
            int n;

            cout << red << "- Send started -" << reset << endl;

            //thread t_bala(&Bala_Thread, &bala, &bala_x, &bala_y, &bala_direccion, &bala_jugador);

            do{
                // INICIA CODIGO DEL JUEGO ==================================================

                //el jugador siempre sera el elemneto 0


                int ch;
                initscr(); // Start curses mode
                start_color(); // Start the color functionality
                cbreak(); // Line buffering disabled, Pass on * everty thing to me
                keypad(stdscr, TRUE); // I need that nifty F1
                noecho();
                init_pair(1, COLOR_CYAN, COLOR_BLACK);

                // Initialize the window parameters


                // Initialize the window parameters

                thread t_pos(&Pos_Thread, ref(amigos)); // REVISANDO POSICIONES DE LAS BALAS
                t_pos.detach();

                while((ch = getch()) != KEY_F(1))
                {
                    // REVISANDO LOS MOVIMIENTOS DEL OTRO JUGADOR --------------------
                    for(int i=0; i<monstruos.size();i++)
                    {
                         create_box(&monstruos[i], FALSE);
                    }
                    for(int i=0; i<monstruos.size();i++)
                    {
                        monstruos[i].startx = amigos[i].second.first;
                        monstruos[i].starty = amigos[i].second.second;
                    }
                    for(int i=0; i<monstruos.size();i++)
                    {
                         create_box(&monstruos[i], TRUE);
                    }
                    //Aqui Colocamos la vida
                    mvaddch(0, 0, vida+48);

                    // --------Hacer en loop con amigos---------------------------------------------

                    switch(ch)
                    {

                        case KEY_LEFT:
                        create_box(&monstruos[0], FALSE);
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='O';
                        --amigos[0].second.first;
                        if(amigos[0].second.first <=0)
                        {
                            amigos[0].second.first++;
                        }
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='P';

                        ActionG_PACKAGE(buffer, amigos[0].first, amigos[0].second.first, amigos[0].second.second);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&monstruos[0], TRUE);


                        break;

                        // --------------------------------------------------

                        case KEY_RIGHT:
                        create_box(&monstruos[0], FALSE);
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='O';
                        ++amigos[0].second.first;
                        if(amigos[0].second.first >=60)
                        {
                            amigos[0].second.first--;
                        }
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='P';

                        ActionG_PACKAGE(buffer, amigos[0].first, amigos[0].second.first, amigos[0].second.second);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&monstruos[0], TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_UP:
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='O';
                        create_box(&monstruos[0], FALSE);
                        --amigos[0].second.second;
                        if(amigos[0].second.second <=0)
                        {
                            amigos[0].second.second++;
                        }
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='P';

                        ActionG_PACKAGE(buffer, amigos[0].first, amigos[0].second.first, amigos[0].second.second);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&monstruos[0], TRUE);


                        break;

                        // --------------------------------------------------

                        case KEY_DOWN:
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='O';
                        create_box(&monstruos[0], FALSE);
                        ++amigos[0].second.second;
                        if(amigos[0].second.second >=16)
                        {
                            amigos[0].second.second--;
                        }
                        mapa[amigos[0].second.second-1][amigos[0].second.first-1]='P';

                        ActionG_PACKAGE(buffer, amigos[0].first, amigos[0].second.first, amigos[0].second.second);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&monstruos[0], TRUE);

                        break;

                        case 'd':

                        WIN bala;
                        init_win_params(&bala);
                        bala.startx = monstruos[0].startx;
                        bala.starty = monstruos[0].starty;
                        create_bullet(&bala, FALSE);
                        create_box(&monstruos[0], TRUE);

                        CLIENT_MUTEX.lock();
                        balas.push_back(bala);
                        balas_info.emplace_back(monstruos[0].startx + 8, monstruos[0].starty, BALA_DERECHA, amigos[0].first);
                        CLIENT_MUTEX.unlock();

                        ActionB_PACKAGE(buffer, monstruos[0].startx + 8, monstruos[0].starty, BALA_DERECHA, amigos[0].first);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 7);
                        if (n < 0) perror("ERROR: Writing to socket");

                        break;

                        case 'a':

                        //WIN bala;
                        init_win_params(&bala);
                        bala.startx = monstruos[0].startx;
                        bala.starty = monstruos[0].starty;
                        create_bullet(&bala, FALSE);
                        create_box(&monstruos[0], TRUE);

                        CLIENT_MUTEX.lock();
                        balas.push_back(bala);
                        balas_info.emplace_back(monstruos[0].startx - 8, monstruos[0].starty, BALA_IZQUIERDA, amigos[0].first);
                        CLIENT_MUTEX.unlock();

                        ActionB_PACKAGE(buffer, monstruos[0].startx - 8, monstruos[0].starty, BALA_IZQUIERDA, amigos[0].first);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 7);
                        if (n < 0) perror("ERROR: Writing to socket");

                        break;

                        case 'w':

                        //WIN bala;
                        init_win_params(&bala);
                        bala.startx = monstruos[0].startx;
                        bala.starty = monstruos[0].starty;
                        create_bullet(&bala, FALSE);
                        create_box(&monstruos[0], TRUE);

                        CLIENT_MUTEX.lock();
                        balas.push_back(bala);
                        balas_info.emplace_back(monstruos[0].startx, monstruos[0].starty - 4, BALA_ARRIBA, amigos[0].first);
                        CLIENT_MUTEX.unlock();

                        ActionB_PACKAGE(buffer, monstruos[0].startx, monstruos[0].starty - 4, BALA_ARRIBA, amigos[0].first);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 7);
                        if (n < 0) perror("ERROR: Writing to socket");

                        break;

                        case 's':

                        //WIN bala;
                        init_win_params(&bala);
                        bala.startx = monstruos[0].startx;
                        bala.starty = monstruos[0].starty;
                        create_bullet(&bala, FALSE);
                        create_box(&monstruos[0], TRUE);

                        CLIENT_MUTEX.lock();
                        balas.push_back(bala);
                        balas_info.emplace_back(monstruos[0].startx, monstruos[0].starty + 4, BALA_ABAJO, amigos[0].first);
                        CLIENT_MUTEX.unlock();

                        ActionB_PACKAGE(buffer, monstruos[0].startx, monstruos[0].starty + 4, BALA_ABAJO, amigos[0].first);
                        n = write(chat_socket, buffer, 5 + amigos[0].first.size() + 7);
                        if (n < 0) perror("ERROR: Writing to socket");

                        break;

                    }
                }

                endwin();


                // TERMINA CODIGO DEL JUEGO ==================================================

            }while (true);
        }
        else 
        {
            //cout << red << "Incorrect Option, enter a valid imput" << reset << endl;
        }
        

    }while(option !=3);
}

void CLIENT_UNPACK(int client_socket, vector<pair<string, pair<int,int> >>& amigos)
{
    char* buffer = new char[256];
    int n;
    // LEYENDO LA CABEZARA --------------------------------------------------

    n = read(client_socket, buffer, 4);
    if (n < 0) perror("ERROR: Reading from socket");

    int header = Read_INT(buffer, 0, 4);
    //cout << "Header: " << header << endl;

    // LEYENDO LA ACCION --------------------------------------------------

    n = read(client_socket, buffer, 1);
    if (n < 0) perror("ERROR: Reading from socket");

    char action = buffer[0];
   // cout << "Action: " << action << endl;

    // AGREGANDO AMIGO --------------------------------------------------
    if(action == 'R')
    {
        //cout << endl << "[Action R] UNPACK: STARTING" << endl;

        string bufferSubset = "";

        //cout << "[Action R] Msg size: " << header << endl;

        n = read(client_socket, buffer, header);
        if (n < 0) perror("ERROR: Reading from socket");
        for (int i = 0; i < header; i++)
            bufferSubset += buffer[i];

        string msg = bufferSubset;
        //cout << green << "You Have a new Friend " << reset <<endl;
        //cout << msg << endl;
        //agregar al usuario
        UpdateFriends(ref(amigos),msg);

        //cout << "[Action R] UNPACK: FINISHED" << endl << endl;
    }

    else if (action == 'B')
    {
        //cout << "----- RECIBIENDO BALA ---------------------------------" << endl;

        // LEYENDO EL JUGADOR --------------------------------------------------

        n = read(client_socket, buffer, header);
        if (n < 0) perror("ERROR: Reading from socket");

        string jugador = Read_STR(buffer, 0, header);
        //cout << "Jugador: " << jugador << endl;

        // LEYENDO X --------------------------------------------------

        n = read(client_socket, buffer, 3);
        if (n < 0) perror("ERROR: Reading from socket");

        int x = Read_INT(buffer, 0, 3);
        //cout << "X: " << x << endl;

        // LEYENDO Y --------------------------------------------------

        n = read(client_socket, buffer, 3);
        if (n < 0) perror("ERROR: Reading from socket");

        int y = Read_INT(buffer, 0, 3);
        //cout << "Y: " << y << endl;

        // LEYENDO DIRECCION --------------------------------------------------

        n = read(client_socket, buffer, 1);
        if (n < 0) perror("ERROR: Reading from socket");

        int dir = Read_INT(buffer, 0, 1);
        //cout << "DIR: " << dir << endl;

        WIN bala;
        init_win_params(&bala);
        bala.startx = x;
        bala.starty = y;
        create_bullet(&bala, FALSE);
        create_box(&monstruos[0], TRUE);

        CLIENT_MUTEX.lock();
        balas.push_back(bala);
        balas_info.emplace_back(x, y, dir, jugador);
        CLIENT_MUTEX.unlock();
    }

    else if (action == 'G')
    {
        //cout << "----- RECIBIENDO ---------------------------------" << endl;

        // LEYENDO EL JUGADOR --------------------------------------------------

        n = read(client_socket, buffer, header);
        if (n < 0) perror("ERROR: Reading from socket");

        string jugador = Read_STR(buffer, 0, header);
        //cout << "Jugador: " << jugador << endl;

        // LEYENDO X --------------------------------------------------

        n = read(client_socket, buffer, 3);
        if (n < 0) perror("ERROR: Reading from socket");

        int x = Read_INT(buffer, 0, 3);
        //cout << "X: " << x << endl;

        // LEYENDO Y --------------------------------------------------

        n = read(client_socket, buffer, 3);
        if (n < 0) perror("ERROR: Reading from socket");

        int y = Read_INT(buffer, 0, 3);
        //cout << "Y: " << y << endl;

        for(int i=0; i< amigos.size(); i++)
        {
            if(amigos[i].first == jugador)
            {                
                amigos[i].second.first = x;
                amigos[i].second.second = y;
            }
        }

        //cout << "--------------------------------------------------" << endl;
    }
    else
    {
        //cout << red << "MASIVE ERROR NO PROTOCOL" << reset << endl;
    }

}

void Client_Listener_Thread(int client_socket, vector<pair<string, pair<int,int> >>& amigos)
{
    do{
        CLIENT_UNPACK(client_socket, ref(amigos));
    }while(true);
}
