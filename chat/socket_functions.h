#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

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
#include <vector>
#include <mutex>

using namespace std;

#define CLEAR cout << "\033[2J\033[1;1H" // LIMPIA EL COLOR DE LA CONSOLA

const string reset("\033[0m");
const string red("\033[0;31m");
const string green("\033[0;32m");
const string blue("\033[0;34m");
const string yellow("\033[0;33m");
const string magenta("\033[0;35m");

const int buffer_size = 4096;

#endif
