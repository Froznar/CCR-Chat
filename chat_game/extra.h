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

// INICIA DEPENDENCIAS DEL JUEGO ==================================================

#include <game.h>
#include <ncurses.h> // LIBS += -lncurses
#include <chrono> // std::chrono::seconds

// TERMINA DEPENDENCIAS DEL JUEGO ==================================================

using namespace std;

#define CLEAR cout << "\033[2J\033[1;1H" // Limpia la consola

const string reset("\033[0m");
const string red("\033[0;31m");
const string green("\033[0;32m");
const string magenta("\033[0;35m");

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

void init_win_params(WIN *p_win)
{
    p_win->height = 8;
    p_win->width = 16;
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
        move( y+0,x ); addstr("  ##        ##  ");
        move( y+1,x ); addstr("    #      #    ");
        move( y+2,x ); addstr("  ############  ");
        move( y+3,x ); addstr(" ###  ####  ### ");
        move( y+4,x ); addstr("################");
        move( y+5,x ); addstr("# ############ #");
        move( y+6,x ); addstr("# #          # #");
        move( y+7,x ); addstr("   ##      ##   ");
    }
    else
        for(j = y; j <= y + h; ++j)
            for(i = x; i <= x + w; ++i)
                mvaddch(j, i, ' ');

    refresh();
}

// TERMINA CODIGO DEL JUEGO ==================================================

void ChatSend(int chat_socket, string user_name, bool& end_chat, int GAME_TIPO) // GAME TIPO SE USA PARA QUE EL SERVIDOR NO EJECUTE EL JUEGO
{
    char* buffer = new char[8192];
    string fileName;
    int packageSize = 0;
    int n;

    cout << red << "- Send started -" << reset << endl;

    do{
        // INICIA CODIGO DEL JUEGO ==================================================

        if ( GAME_TIPO == GAME_TIPO_CLIENTE )
        {
            int JUGADOR_ACTUAL = JUGADOR_1;

            if (user_name[0] == '1') // SI EL NOMBRE DEL USUARIO EMPIEZA CON 1 SE LE ASIGNA EL JUGADOR 1
                JUGADOR_ACTUAL = JUGADOR_1;

            if (user_name[0] == '2') // SI EL NOMBRE DEL USUARIO EMPIEZA CON 2 SE LE ASIGNA EL JUGADOR 2
                JUGADOR_ACTUAL = JUGADOR_2;

            // VARIABLES PARA ACTUALIZAR LA POSICION DEL OTRO JUGADOR
            int JUGADOR_1_X_ANTERIOR = JUGADOR_1_X;
            int JUGADOR_1_Y_ANTERIOR = JUGADOR_1_Y;
            int JUGADOR_2_X_ANTERIOR = JUGADOR_2_X;
            int JUGADOR_2_Y_ANTERIOR = JUGADOR_2_Y;

            WIN win;
            WIN win2;
            int ch;
            initscr(); // Start curses mode
            start_color(); // Start the color functionality
            cbreak(); // Line buffering disabled, Pass on * everty thing to me
            keypad(stdscr, TRUE); // I need that nifty F1
            noecho();
            init_pair(1, COLOR_CYAN, COLOR_BLACK);

            // Initialize the window parameters

            init_win_params(&win);
            print_win_params(&win);

            // Initialize the window parameters

            init_win_params(&win2);
            print_win_params(&win2);

            attron(COLOR_PAIR(1));
            printw("Press F1 to exit");
            refresh();

            attroff(COLOR_PAIR(1));
            win.startx = win.startx - 20;
            create_box(&win, TRUE);
            win2.startx = win2.startx + 20;
            create_box(&win2, TRUE);

            while((ch = getch()) != KEY_F(1))
            {
                // REVISANDO LOS MOVIMIENTOS DEL OTRO JUGADOR --------------------

                if (JUGADOR_1_X != JUGADOR_1_X_ANTERIOR)
                {
                    win.startx = JUGADOR_1_X;
                    JUGADOR_1_X_ANTERIOR = JUGADOR_1_X;
                }

                if (JUGADOR_1_Y != JUGADOR_1_Y_ANTERIOR)
                {
                    win.startx = JUGADOR_1_Y;
                    JUGADOR_1_Y_ANTERIOR = JUGADOR_1_Y;
                }

                if (JUGADOR_2_X != JUGADOR_2_X_ANTERIOR)
                {
                    win2.startx = JUGADOR_2_X;
                    JUGADOR_2_X_ANTERIOR = JUGADOR_2_X;
                }

                if (JUGADOR_2_Y != JUGADOR_2_Y_ANTERIOR)
                {
                    win2.startx = JUGADOR_2_Y;
                    JUGADOR_2_Y_ANTERIOR = JUGADOR_2_Y;
                }

                // ---------------------------------------------------------------

                create_box(&win, TRUE);
                create_box(&win2, TRUE);

                if (JUGADOR_ACTUAL == JUGADOR_1)
                {
                    switch(ch)
                    {
                        case KEY_LEFT:
                        create_box(&win, FALSE);
                        --win.startx;

                        ActionG_PACKAGE(buffer, user_name, win.startx, win.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_RIGHT:
                        create_box(&win, FALSE);
                        ++win.startx;

                        ActionG_PACKAGE(buffer, user_name, win.startx, win.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_UP:
                        create_box(&win, FALSE);
                        --win.starty;

                        ActionG_PACKAGE(buffer, user_name, win.startx, win.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_DOWN:
                        create_box(&win, FALSE);
                        ++win.starty;

                        ActionG_PACKAGE(buffer, user_name, win.startx, win.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win, TRUE);
                        break;
                    }
                }

                if (JUGADOR_ACTUAL == JUGADOR_2)
                {
                    switch(ch)
                    {
                        case KEY_LEFT:
                        create_box(&win2, FALSE);
                        --win2.startx;

                        ActionG_PACKAGE(buffer, user_name, win2.startx, win2.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win2, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_RIGHT:
                        create_box(&win2, FALSE);
                        ++win2.startx;

                        ActionG_PACKAGE(buffer, user_name, win2.startx, win2.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win2, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_UP:
                        create_box(&win2, FALSE);
                        --win2.starty;

                        ActionG_PACKAGE(buffer, user_name, win2.startx, win2.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win2, TRUE);
                        break;

                        // --------------------------------------------------

                        case KEY_DOWN:
                        create_box(&win2, FALSE);
                        ++win2.starty;

                        ActionG_PACKAGE(buffer, user_name, win2.startx, win2.starty);
                        n = write(chat_socket, buffer, 5 + user_name.size() + 6);
                        if (n < 0) perror("ERROR: Writing to socket");

                        create_box(&win2, TRUE);
                        break;
                    }
                }
            }

            endwin();
        }

        // TERMINA CODIGO DEL JUEGO ==================================================

        else
        {
            cout << "File name: "; cin >> fileName;

            ActionFD_PACKAGE(buffer, fileName, user_name, 'F', packageSize);

            cout << endl << "[Sending: '" << buffer << "']" << endl;

            n = write(chat_socket, buffer, packageSize);
            if (n < 0) perror("ERROR: Writing to socket");
        }

    }while (end_chat == false);
}

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

            n = read(chat_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string jugador = Read_STR(buffer, 0, header);
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

        if (action == 'F')
        {
            // LEYENDO EL TAMANO DEL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, 2);
            if (n < 0) perror("ERROR: Reading from socket");

            int nicknameSize = Read_INT(buffer, 0, 2);
            cout << "Nickname size: " << nicknameSize << endl;

            // LEYENDO EL NICKNAME --------------------------------------------------

            n = read(chat_socket, buffer, nicknameSize);
            if (n < 0) perror("ERROR: Reading from socket");

            string nickname = Read_STR(buffer, 0, nicknameSize);
            cout << "Nickname: " << nickname << endl;

            // LEYENDO EL NOMBRE DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, header);
            if (n < 0) perror("ERROR: Reading from socket");

            string fileName = Read_STR(buffer, 0, header);
            cout << "File name: " << fileName << endl;

            // LEYENDO EL TAMANO DEL ARCHIVO --------------------------------------------------

            n = read(chat_socket, buffer, 4);
            if (n < 0) perror("ERROR: Reading from socket");

            int fileSize = Read_INT(buffer, 0, 4);
            cout << "File size: " << fileSize << endl;

            // LEYENDO EL ARCHIVO BINARIO --------------------------------------------------

            n = read(chat_socket, buffer, fileSize);
            if (n < 0) perror("ERROR: Reading from socket");

            Read_BIN(buffer, 0, nickname + "_" + fileName, fileSize);
            cout << "The file has been saved as: " << nickname + "_" + fileName << endl;
        }

        //int wait; cin >> wait;

    }while (end_chat == false);

}

#endif
