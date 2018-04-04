#include <iostream>
#include <protocols.h>
#include <string.h>

using namespace std;

int main()
{
    // EL EJEMPLO DEL INSERT HEADER =======================================================

    char* miau = new char[512];
    miau[0] = 'c';
    miau[1] = 'a';
    miau[2] = 't';

    cout << "A: " << miau << endl; // IMPRIME: cat

    InsertHeader("1111R", miau);

    cout << "header + A: " << miau << endl; // IMPRIME: 1111Rcat

    // ======================================================================================

    string package_clientA = "";

    char* buffer_server = new char[512];
    string package_server = "";

    while(true)
    {

        // ============================== INTERFAZ DEL CLIENTE ==============================

        cout << "--------------------------------------------------" << endl;
        cout << "Actions: " << endl
             << "    [P] Print list of user on the chat" << endl
             << "    [L] Login to the chat" << endl
             << "    [C] Send a msg to an user on the chat" << endl
             << "    [E] End chat or logout from chat" << endl
             << endl;

        string selectedOption = "";
        cout << "Option: "; cin >> selectedOption;

        if(selectedOption == "P")
        {
            cout << endl << "========== Action P ==========" << endl << endl;

            cout << "----- In Client A -----" << endl;
            package_clientA = ActionP_PACKAGE();
            cout << endl << "SENDING: " << package_clientA << endl << endl;

            strcpy(buffer_server, package_clientA.c_str()); // "ENVIANDO" PAQUETE AL SERVIDOR
        }

        if(selectedOption == "L")
        {
            cout << endl << "========== Action L ==========" << endl << endl;

            cout << "----- In Client A -----" << endl;

            string nickname = "";
            cout << "Nickname: "; cin >> nickname;
            package_clientA = ActionL_PACKAGE(nickname);
            cout << endl << "SENDING: " << package_clientA << endl << endl;

            strcpy(buffer_server, package_clientA.c_str()); // "ENVIANDO" PAQUETE AL SERVIDOR
        }

        if(selectedOption == "C")
        {
            cout << endl << "========== Action C ==========" << endl << endl;

            cout << "----- In Client A -----" << endl;

            string targetUser = "";
            string msg = "";

            cout << "Send a message to: "; cin >> targetUser;
            cout << "Msg: "; cin >> msg;

            package_clientA = ActionC_PACKAGE(targetUser, msg);
            cout << endl << "SENDING: " << package_clientA << endl << endl;

            strcpy(buffer_server, package_clientA.c_str()); // "ENVIANDO" PAQUETE AL SERVIDOR
        }

        if(selectedOption == "E")
        {
            cout << endl << "========== Action E ==========" << endl << endl;

            cout << "----- In Client A -----" << endl;

            package_clientA = ActionE_PACKAGE();
            cout << endl << "SENDING: " << package_clientA << endl << endl;

            strcpy(buffer_server, package_clientA.c_str()); // "ENVIANDO" PAQUETE AL SERVIDOR
        }

        // ============================== INTERFAZ DEL SERVIDOR ==============================

        cout << "----- In Server -----" << endl;

        if (buffer_server[4] == 'P')
        {
            string usersOnline = "User1,User2,User3";
            cout << "Users list: " << usersOnline << endl;

            package_server = ActionR_PACKAGE(usersOnline);
            cout << endl << "SENDING: " << package_server << endl << endl;
        }

        if (buffer_server[4] == 'L')
        {
            ActionL_UNPACK(buffer_server);
        }

        if (buffer_server[4] == 'C')
        {
            ActionC_UNPACK(buffer_server);
        }

        if (buffer_server[4] == 'E')
        {
            cout << "- LOGOUT -" << endl;
        }

    }

    return 0;
}
