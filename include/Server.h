#ifndef YA_ORDER_SERVER_H
#define YA_ORDER_SERVER_H
#include <string>

using namespace std;
class Server{
public:
    int id;
    int port;
    string ip;

public:
    Server(int id, int port,string ip);
    int start();  // 开始监听
    // 接下来的方法，用于处理客户端的操作。
};

#endif //YA_ORDER_SERVER_H
