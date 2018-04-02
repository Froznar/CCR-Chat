#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <string>

using namespace std;

string IntToStr(int number, int sizeStr)
{
    string output = "";
    string number_str = to_string(number);

    if (number_str.size() > sizeStr)
    {
        perror("ERROR in 'IntToStr', number_str.size() is greater than sizeStr");
        return output;
    }

    for (int i = 0; i < ( sizeStr - number_str.size() ); i++)
        output += "0";

    output += number_str;

    return output;
}

// Action P: Print list of user on the chat -------------------------

string ActionP_PACKAGE()
{
    cout << endl << "[Action P] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(1, 4); // 4B: Size of the msg
    output += "P"; // 1B: Action = P

    cout << "[Action P] PACKAGE: FINISHED" << endl << endl;

    return output;
}

// Action L: Login to the chat -------------------------

string ActionL_PACKAGE(string nickname)
{
    cout << endl << "[Action L] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(nickname.size(), 4); // 4B: Size of nickname
    output += "L"; // 1B: Action = L
    output += nickname; // nB: Nickname

    cout << "[Action L] PACKAGE: FINISHED" << endl << endl;

    return output;
}

void ActionL_UNPACK(char* buffer)
{
    cout << endl << "[Action L] UNPACK: STARTING" << endl;

    if (buffer[4] != 'L')
    {
        perror("[Action L] ERROR in UNPACK, action != L");
        return;
    }

    string bufferSubset = "";
    int endSubset = 4;

    for (int i = 0; i < endSubset; i++)
        bufferSubset += buffer[i];

    int nicknameSize = atoi( bufferSubset.c_str() );
    cout << "[Action L] Nickname size: " << nicknameSize << endl;

    bufferSubset = "";
    endSubset = 5 + nicknameSize;

    for (int i = 5; i < endSubset; i++)
        bufferSubset += buffer[i];

    string nickname = bufferSubset;
    cout << "[Action L] Nickname: " << nickname << endl;

    cout << "[Action L] UNPACK: FINISHED" << endl << endl;
}

// Action C: Send a msg to an user on the chat -------------------------

string ActionC_PACKAGE(string nickname, string msg)
{
    cout << endl << "[Action C] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(msg.size(), 4); // 4B: Size of msg
    output += "C"; // 1B: Action = C
    output += IntToStr(nickname.size(), 2); // 2B: Nickname size
    output += nickname; // nB: Nickname
    output += msg; // nB: Msg

    cout << "[Action C] PACKAGE: FINISHED" << endl << endl;

    return output;
}

void ActionC_UNPACK(char* buffer)
{
    cout << endl << "[Action C] UNPACK: STARTING" << endl;

    if (buffer[4] != 'C')
    {
        perror("[Action C] ERROR in UNPACK, action != C");
        return;
    }

    string bufferSubset = "";
    int endSubset = 4;

    for (int i = 0; i < endSubset; i++)
        bufferSubset += buffer[i];

    int msgSize = atoi( bufferSubset.c_str() );
    cout << "[Action C] Msg size: " << msgSize << endl;

    bufferSubset = "";
    endSubset = 5 + 2;

    for (int i = 5; i < endSubset; i++)
        bufferSubset += buffer[i];

    int nicknameSize = atoi( bufferSubset.c_str() );
    cout << "[Action C] Nickname size: " << nicknameSize << endl;

    bufferSubset = "";
    endSubset = 7 + nicknameSize;

    for (int i = 7; i < endSubset; i++)
        bufferSubset += buffer[i];

    string nickname = bufferSubset;
    cout << "[Action C] Nickname: " << nickname << endl;

    bufferSubset = "";
    int beginSubset = endSubset;
    endSubset = beginSubset + msgSize;

    for (int i = beginSubset; i < endSubset; i++)
        bufferSubset += buffer[i];

    string msg = bufferSubset;
    cout << "[Action C] Msg: " << msg << endl;

    cout << "[Action C] UNPACK: FINISHED" << endl << endl;
}

// Action R: Send a msg to a client -------------------------

string ActionR_PACKAGE(string msg)
{
    cout << endl << "[Action R] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(msg.size(), 4); // 4B: Size of msg
    output += "R"; // 1B: Action = R
    output += msg; // nB: 'msg' or 'nickname: msg'

    cout << "[Action R] PACKAGE: FINISHED" << endl << endl;

    return output;
}

void ActionR_UNPACK(char* buffer)
{
    cout << endl << "[Action R] UNPACK: STARTING" << endl;

    if (buffer[4] != 'R')
    {
        perror("[Action R] ERROR in UNPACK, action != R");
        return;
    }

    string bufferSubset = "";
    int endSubset = 4;

    for (int i = 0; i < endSubset; i++)
        bufferSubset += buffer[i];

    int msgSize = atoi( bufferSubset.c_str() );
    cout << "[Action R] Msg size: " << msgSize << endl;

    bufferSubset = "";
    endSubset = 5 + msgSize;

    for (int i = 5; i < endSubset; i++)
        bufferSubset += buffer[i];

    string msg = bufferSubset;
    cout << "[Action R] Msg: " << msg << endl;

    cout << "[Action R] UNPACK: FINISHED" << endl << endl;
}

// Action E: End chat or logout from chat -------------------------

string ActionE_PACKAGE()
{
    cout << endl << "[Action E] PACKAGE: STARTING" << endl;

    string output = "";
    output += IntToStr(1, 4); // 4B: Size of the msg
    output += "E"; // 1B: Action = E

    cout << "[Action E] PACKAGE: FINISHED" << endl << endl;

    return output;
}

#endif
