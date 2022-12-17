#ifndef YA_ORDER_SERVER_H
#define YA_ORDER_SERVER_H
#include <string>
#include <vector>
#include "Item.h"

using namespace std;
class Server{
public:
    int id;
    int port;

public:
    Server(int id, int port);
    ~Server();
    int start();  // 开始监听
    // 接下来的方法，用于处理客户端的操作。
    vector<Item> searchAll();
    void update(int itemID,Item* item);
    void remove(int itemID);
    void add(Item* item);
};

#endif //YA_ORDER_SERVER_H
