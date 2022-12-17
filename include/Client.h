#ifndef YA_ORDER_CLIENT_H
#define YA_ORDER_CLIENT_H

#include "Item.h"
#define STUDENT 1
#define STORE 2

/**
 * 在线选购和结算，结算无需实现，可以采用支付宝？因此只需要展示服务器上的菜品即可。本质是搜索，选购后服务器上数量 -1
 */
class Client {

public:
    int schoolID;
    int clientSocket;
    int type;

public:
    Client(int schoolID, int type);

    ~Client();

    int start();

    bool close();

    void searchAll();

    void purchase(int itemID);

    void update(int itemID, Item *item);

    void remove(int itemID);

    void add(Item *item);
};

#endif //YA_ORDER_CLIENT_H
