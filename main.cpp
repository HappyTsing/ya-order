#include "include/Server.h"
#include "include/Client.h"
#include <thread>

int main() {
    char command[256];
    printMainHelpMessage();
    printLineHeader();
    cin.getline(command, 256);
    if (strncasecmp(command, "start server 1", 14) == 0) {
        Server *server1 = new Server(1, 5000);
        server1->start();
    } else if (strncasecmp(command, "start server 2", 14) == 0) {
        Server *server2 = new Server(2, 6000);
        server2->start();
    } else if (strncasecmp(command, "start store client 1", 20) == 0) {
        Client *storeClient1 = new Client(1, STORE);
        storeClient1->start();
    } else if (strncasecmp(command, "start store client 2", 20) == 0) {
        Client *storeClient2 = new Client(2, STORE);
        storeClient2->start();
    } else if (strncasecmp(command, "start student client 1", 22) == 0) {
        Client *stuClient1 = new Client(1, STUDENT);
        stuClient1->start();
    } else if (strncasecmp(command, "start student client 2", 22) == 0) {
        Client *stuClient2 = new Client(2, STUDENT);
        stuClient2->start();
    }

//    thread serverThread(&Server::start,server);
//    serverThread.join(); // 等待子线程结束
    return 0;
}
