#ifndef EXTRA_H
#define EXTRA_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <file.h>
#include <vector>
#include <mutex>
#include <protocols.h>

// INICIA DEPENDENCIAS DEL JUEGO ==================================================

#include <game.h>
#include <ncurses.h> // LIBS += -lncurses
#include <chrono> // std::chrono::seconds

// TERMINA DEPENDENCIAS DEL JUEGO ==================================================

using namespace std;
//GLOBAL VARIABLES
mutex MAP_MUTEX;
mutex CLIENT_MUTEX;


#define CLEAR cout << "\033[2J\033[1;1H" // Limpia la consola

const std::string reset("\033[0m");
const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string blue("\033[0;34m");
const std::string yellow("\033[0;33m");
const std::string magenta("\033[0;35m");

string CreateHeader(int message_size_int)
{
    string header = "";
    string message_size_str = to_string(message_size_int);
    for (int i = 0; i < ( 4 - message_size_str.size() ); i++)
        header += "0";

    return header + message_size_str;
}

string ToString(char* input)
{
    string output(input);

    return output;
}

// INICIA CODIGO DEL JUEGO ==================================================

const int GAME_TIPO_CLIENTE = 0;
const int GAME_TIPO_SERVIDOR = 1;

// VARIABLES PARA ACTUALIZAR LA POSICION DEL OTRO JUGADOR
int JUGADOR_1_X = 0;
int JUGADOR_1_Y = 0;
int JUGADOR_2_X = 0;
int JUGADOR_2_Y = 0;
string JUGADOR_ACTUAL = "1";

typedef struct _win_border_struct
{
    chtype ls, rs, ts, bs, tl, tr, bl, br;
}

WIN_BORDER;

typedef struct _WIN_struct
{
    int startx, starty;
    int height, width;
    WIN_BORDER border;
}

WIN;
void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);

void bullet(WIN *p_win){}

void init_bullet_params(WIN *p_win)
{
    p_win->height = 1;
    p_win->width = 1;
    p_win->starty = (LINES - p_win->height)/2;
    p_win->startx = (COLS - p_win->width)/2;
    p_win->border.ls = '|';
    p_win->border.rs = '|';
    p_win->border.ts = '-';
    p_win->border.bs = '-';
    p_win->border.tl = '+';
    p_win->border.tr = '+';
    p_win->border.bl = '+';
    p_win->border.br = '+';
}

void init_win_params(WIN *p_win)
{
    p_win->height = 3;
    p_win->width = 5;
    p_win->starty = (LINES - p_win->height)/2;
    p_win->startx = (COLS - p_win->width)/2;
    p_win->border.ls = '|';
    p_win->border.rs = '|';
    p_win->border.ts = '-';
    p_win->border.bs = '-';
    p_win->border.tl = '+';
    p_win->border.tr = '+';
    p_win->border.bl = '+';
    p_win->border.br = '+';
}

void print_win_params(WIN *p_win){}

void create_bullet(WIN *p_win, bool flag)
{
    int i, j;
    int x, y, w, h;
    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if(flag == TRUE)
    {
        move( y+0,x ); addstr("o");
    }
    else
        for(j = y; j <= y + h; ++j)
            for(i = x; i <= x + w; ++i)
                mvaddch(j, i, ' ');

    refresh();
}

void create_box(WIN *p_win, bool flag)
{
    int i, j;
    int x, y, w, h;
    x = p_win->startx;
    y = p_win->starty;
    w = p_win->width;
    h = p_win->height;

    if(flag == TRUE)
    {
        move( y+0,x ); addstr("/|_/|");
        move( y+1,x ); addstr(">^,^<");
        move( y+2,x ); addstr(" / | ");
        move( y+3,x ); addstr("(___)");
    }   
    else
        for(j = y; j <= y + h; ++j)
            for(i = x; i <= x + w; ++i)
            {
                mvaddch(j, i, ' ');

            }


    refresh();
}

// TERMINA CODIGO DEL JUEGO ==================================================

void ChatRecive(int chat_socket, bool& end_chat)
{
    char* buffer = new char[8192];
    int n;

    cout << red << "- Recive started -" << reset << endl;

    do{
        // LEYENDO LA CABEZARA --------------------------------------------------

        n = read(chat_socket, buffer, 4);
        if (n < 0) perror("ERROR: Reading from socket");

        int header = Read_INT(buffer, 0, 4);
        cout << "Header: " << header << endl;

        // LEYENDO LA ACCION --------------------------------------------------

        n = read(chat_socket, buffer, 1);
        if (n < 0) perror("ERROR: Reading from socket");

        char action = buffer[0];
        cout << "Action: " << action << endl;

        // EJECUTANDO LA ACCION --------------------------------------------------

        if (action == 'G')
        {
            cout << "----- RECIBIENDO ---------------------------------" << endl;

            // LEYENDO EL JUGADOR --------------------------------------------------

            n = read(chat_socket, buffer, 1);
            if (n < 0) perror("ERROR: Reading from socket");

            int jugador = Read_INT(buffer, 0, 1);
            cout << "Jugador: " << jugador << endl;

            // LEYENDO X --------------------------------------------------

            n = read(chat_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int x = Read_INT(buffer, 0, 3);
            cout << "X: " << x << endl;

            // LEYENDO Y --------------------------------------------------

            n = read(chat_socket, buffer, 3);
            if (n < 0) perror("ERROR: Reading from socket");

            int y = Read_INT(buffer, 0, 3);
            cout << "Y: " << y << endl;

            cout << "--------------------------------------------------" << endl;
        }


        //int wait; cin >> wait;

    }while (end_chat == false);

}

#endif
