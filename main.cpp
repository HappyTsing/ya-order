#include "include/Server.h"

int main() {
    Server* server = new Server(1,5000,"127.0.0.1");
    server->start();
    return 0;
}
