#include "include/Server.h"
#include "include/Client.h"
#include <thread>
using namespace std;
int main() {
    Server* server1 = new Server(1,5000);
    Server* server2 = new Server(2,6000);

    Client* stuClient1 = new Client(1,STUDENT);
    Client* stuClient2 = new Client(2,STUDENT);
    Client* storeClient1 = new Client(1,STORE);
    Client* storeClient2 = new Client(1,STORE);




//    thread serverThread(&Server::start,server);
//    serverThread.join(); // 等待子线程结束
    return 0;
}
