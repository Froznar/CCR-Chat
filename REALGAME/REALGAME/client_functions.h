#ifndef CLIENT_FUNCTIONS_H
#define CLIENT_FUNCTIONS_H

#include <extra.h>
#include <game_protocols.h>

//VARIABLES GLOBALES
vector<string> mapa;
string client_name;

int vida = 5;
int misbalas=0;

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
    for(int i=0;i<20;i++)
    {
        mapa.push_back("00000000000000000000");
    }
}

//actualizar amigos
void UpdateFriends(vector<pair<string, pair<int,int> >>& amigos, string usuario)
{
    //agregarlo

    CLIENT_MUTEX.lock();

    amigos.push_back(make_pair(usuario, make_pair(0,0)));
    balas_info.emplace_back(-1,-1, BALA_DERECHA, usuario);

    CLIENT_MUTEX.unlock();
    //inicializarlo
}



void Game_Thread(int client_socket, vector<pair<string, pair<int,int> >>& amigos)
{
    ALLEGRO_DISPLAY *display;

    float FPS=30.0;

    if(!al_init())
        {
        fprintf(stderr, "failed to initialize allegro!\n");
        return;
        }

    display = al_create_display(600,620);//tile=30*30
    al_set_window_title(display,"CCR GAME");

    if(!display)
        {
          fprintf(stderr, "failed to create display!\n");
          return;
        }
    al_set_window_position(display,100,100);


  //--------------------------------INSTANCIAS--------------------------------------
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_FONT *fuente=al_load_font("Alice_in_Wonderland_3.ttf",45,NULL);

  //---------------------------------IMAGENES---------------------------------------

    ALLEGRO_BITMAP *brainburn = al_load_bitmap("Brainburn.png");
    ALLEGRO_BITMAP *fondo = al_load_bitmap("fondo.jpg");
    ALLEGRO_BITMAP *logo = al_load_bitmap("Logo.png");

    ALLEGRO_BITMAP* players[6];
    ALLEGRO_BITMAP *p0 = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *p1 = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *p2 = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *p3 = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *p4 = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *p5 = al_load_bitmap("1.png");

    players[0]= p0;
    players[1]= p1;
    players[2]= p2;
    players[3]= p3;
    players[4]= p4;
    players[5]= p5;
    ALLEGRO_BITMAP* balas[7];
    ALLEGRO_BITMAP *b0 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b1 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b2 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b3 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b4 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b5 = al_load_bitmap("bala.png");
    ALLEGRO_BITMAP *b6 = al_load_bitmap("bala.png");

    balas[0]= b0;
    balas[1]= b1;
    balas[2]= b2;
    balas[3]= b3;
    balas[4]= b4;
    balas[5]= b5;
    balas[6]= b6;
    //-------------------------------CREANDO COLA DE EVENTOS--------------------------

      ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);


      ALLEGRO_EVENT_QUEUE *cola_de_eventos = al_create_event_queue();
      ALLEGRO_KEYBOARD_STATE key_state;

      //teclado

      al_register_event_source(cola_de_eventos,al_get_keyboard_event_source());
      al_register_event_source(cola_de_eventos,al_get_timer_event_source(timer));
      al_register_event_source(cola_de_eventos,al_get_display_event_source(display));

    //---------------------------------MUSICA--------------------------------
      al_reserve_samples(2);

      ALLEGRO_SAMPLE *background = al_load_sample("background.flac");
      ALLEGRO_SAMPLE_INSTANCE *song_instance=al_create_sample_instance(background);
      al_set_sample_instance_position(song_instance,ALLEGRO_PLAYMODE_LOOP);
      al_attach_sample_instance_to_mixer(song_instance,al_get_default_mixer());

      al_play_sample_instance(song_instance);

    //-------------------------------BUCLE DEL JUEGO----------------------------------
    bool done=false;

    al_start_timer(timer);


    //iniciar variables del jugador
    char* buffer = new char[512];
    int n;


    int x=0;
    int y=0;
    int amigossize=0;

    mapa[y][x]='P';
    while(!done)
    {

        amigossize=amigos.size();
        ALLEGRO_EVENT events;
        al_wait_for_event(cola_de_eventos,&events);
        al_get_keyboard_state(&key_state);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done=true;
        }

        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            if(al_key_down(&key_state,ALLEGRO_KEY_LEFT) )
            {
                mapa[y][x]='O';
                x--;
                if(x<0) x++;
                mapa[y][x]='P';

                ActionG_PACKAGE(buffer, client_name, x, y);
                n = write(client_socket, buffer, 5 + client_name.size() + 6);
                if (n < 0) perror("ERROR: Writing to socket");
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_RIGHT) )
            {
                mapa[y][x]='O';
                x++;
                if(x>=20) x--;
                mapa[y][x]='P';

                ActionG_PACKAGE(buffer, client_name, x, y);
                n = write(client_socket, buffer, 5 + client_name.size() + 6);
                if (n < 0) perror("ERROR: Writing to socket");
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_UP) )
            {
                mapa[y][x]='O';
                y--;
                if(y<0) y++;
                mapa[y][x]='P';

                ActionG_PACKAGE(buffer, client_name, x, y);
                n = write(client_socket, buffer, 5 + client_name.size() + 6);
                if (n < 0) perror("ERROR: Writing to socket");
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_DOWN) )
            {
                mapa[y][x]='O';
                y++;
                if(y>=20) y--;
                mapa[y][x]='P';

                ActionG_PACKAGE(buffer, client_name, x, y);
                n = write(client_socket, buffer, 5 + client_name.size() + 6);
                if (n < 0) perror("ERROR: Writing to socket");
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_A) )
            {
                misbalas++;
                if(misbalas<2)
                {
                    ActionB_PACKAGE(buffer, x-1, y, BALA_IZQUIERDA, client_name);
                    n = write(client_socket, buffer, 5 + client_name.size() + 7);
                    if (n < 0) perror("ERROR: Writing to socket");
                }
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_D) )
            {
                misbalas++;
                if(misbalas<2)
                {
                    ActionB_PACKAGE(buffer, x+1, y, BALA_DERECHA, client_name);
                    n = write(client_socket, buffer, 5 + client_name.size() + 7);
                    if (n < 0) perror("ERROR: Writing to socket");
                }
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_W) )
            {
                misbalas++;
                if(misbalas<2)
                {
                    ActionB_PACKAGE(buffer, x, y-1, BALA_ARRIBA, client_name);
                    n = write(client_socket, buffer, 5 + client_name.size() + 7);
                    if (n < 0) perror("ERROR: Writing to socket");
                }
            }
            if(al_key_down(&key_state,ALLEGRO_KEY_S) )
            {
                misbalas++;
                if(misbalas<2)
                {
                    ActionB_PACKAGE(buffer, x, y+1, BALA_ABAJO, client_name);
                    n = write(client_socket, buffer, 5 + client_name.size() + 7);
                    if (n < 0) perror("ERROR: Writing to socket");
                }
            }

            al_draw_bitmap(fondo,0,0,NULL);
            //actualizar data
            for(int i=0; i< amigos.size();i++)
            {
                if(players[i]!=nullptr)
                    al_draw_bitmap(players[i], amigos[i].second.first*30, amigos[i].second.second*30,NULL);

            }
            for(int i=0; i<balas_info.size();i++)
            {
                if(balas_info[i]._begin_x >= 0 and
                        balas_info[i]._begin_x <= 19 and
                        balas_info[i]._begin_y >= 0 and
                        balas_info[i]._begin_y <= 19)
                {
                    cout<<"BALA x: "<<balas_info[i]._begin_x<<" y: "<<balas_info[i]._begin_y<<endl;
                    cout<<"posmapa: "<<mapa[balas_info[i]._begin_y][balas_info[i]._begin_x]<<endl;
                    if( mapa[balas_info[i]._begin_y][balas_info[i]._begin_x]=='P')
                    {
                        vida--;
                    }
                    if(balas[i]!=nullptr)
                        al_draw_bitmap(balas[i], balas_info[i]._begin_x*30, balas_info[i]._begin_y*30,NULL);

                    if (balas_info[i]._direccion == BALA_ABAJO)
                        balas_info[i]._begin_y++;
                    else if (balas_info[i]._direccion == BALA_ARRIBA)
                        balas_info[i]._begin_y--;
                    else if (balas_info[i]._direccion == BALA_IZQUIERDA)
                        balas_info[i]._begin_x--;
                    else if (balas_info[i]._direccion == BALA_DERECHA)
                        balas_info[i]._begin_x++;

                }
                else if(balas_info[i]._jugador == client_name)
                {
                    misbalas--;
                }
            }
            char numvids= vida+48;
            al_draw_text(fuente,al_map_rgb(15,21,136),0,0,NULL,&numvids);
            //refrescar
            al_flip_display();
        }
    }
    for(int i=0;i<6;i++){al_destroy_bitmap(players[i]);}
    for(int i=0;i<7;i++){al_destroy_bitmap(balas[i]);}
    al_destroy_display(display);
    al_destroy_bitmap(brainburn);
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(logo);
    al_destroy_font(fuente);
    al_destroy_timer(timer);
    al_destroy_event_queue(cola_de_eventos);
    al_destroy_sample(background);
    al_destroy_sample_instance(song_instance);

}

void Client_Send_Thread(int chat_socket, vector<pair<string, pair<int,int> >>& amigos)
{
    int option;
    do{
        // MENU --------------------------------------------------
        cout << magenta << "----- MENU -----" << endl;
        cout << "[1] -> Log in" << endl;
        //otras acciones
        cout << "[2] -> Play" << reset << endl;

        cin>>option;
        if(option == 1)
        {
            // INICIANDO EL LOG IN --------------------------------------------------
            cout << yellow << "----- Logging In -----" << reset << endl;

            cout << endl << "Username: ";
            cin >> client_name;

            LogIn(client_name, chat_socket);
            bool confirmation = false;
            while (confirmation == false)
            {
                for(int i=0;i<amigos.size();i++)
                {
                    if(amigos[i].first == client_name)
                    {
                        confirmation=true;
                        break;
                    }
                }
            }
            cout<< green << "Successful login" <<reset <<endl;
            //ya estas en el juego

        }
        else if(option == 2)
        {
            thread mygame(Game_Thread, chat_socket, ref(amigos));
            mygame.detach();
        }
        else
        {
            cout << red << "opcion invalida" << reset << endl;
        }

    }while(true);
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
    cout << "Action: " << action << endl;

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
        cout << green << "You Have a new Friend " << reset <<endl;
        cout << msg << endl;
        //agregar al usuario
        UpdateFriends(ref(amigos),msg);

        //cout << "[Action R] UNPACK: FINISHED" << endl << endl;
    }
    else if (action == 'B') // RECIBIENDO LAS BALAS
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
        for(int i=0; i< amigos.size(); i++)
        {
            if(amigos[i].first == jugador)
            {
                balas_info[i]._begin_x = x;
                balas_info[i]._begin_y = y;
                balas_info[i]._direccion= dir;
            }
        }
    }
    //cout << "--------------- RECIBIENDO INFORMACION -----------------" << endl;
    else if (action == 'G')
    {
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



#endif // CLIENT_FUNCTIONS_H
