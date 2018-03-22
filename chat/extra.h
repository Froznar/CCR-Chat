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
#include <unistd.h>

using namespace std;

string ToString(char* input)
{
    string output = string(input);
    return output;
}

char* ToChar(string input)
{
    char *output = new char[input.length() + 1];

    for(int i = 0; i < input.length(); i++)
        output[i] = input[i];

    return output;
}

#endif
