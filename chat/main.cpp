#include <iostream>
#include <client.h>
#include <server.h>

using namespace std;

int main()
{
    int option;

    cout << endl << "[ Chat ]" << endl << endl;

    cout << "[ Menu ] 1: Create a conversation" << endl
         << "[ Menu ] 2: Join a conversation" << endl << endl;

    cout << "[ Menu ] Option: "; cin >> option;

    if (option == 1) Server();
    if (option == 2) Client();

    return 0;
}
