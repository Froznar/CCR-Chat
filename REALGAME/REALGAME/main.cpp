#include <server.h>
#include <client.h>


using namespace std;

int main()
{
    int option;

    cout << "----- Cool Chat -----" << endl
         << "Options:" << endl
         << "    [1] Create a conversation" << endl
         << "    [2] Join a conversation" << endl
         << "Your choice: ";

    cin >> option;

    if (option == 1) HostService();
    if (option == 2) ClientService();

    return 0;

    }
