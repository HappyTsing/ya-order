#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <vector>
#include <fcntl.h>
#include "../include/Server.h"


Server::Server(int id, int port) {
    this->id = id;
    this->port = port;
}

int Server::start() {
    int nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);

//    std::cout <<"nListenSocket= " <<nListenSocket << std::endl;

    if (-1 == nListenSocket) {
        perror("socket error");
        return 0;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(this->port);

    // 绑定 ip 和 port
    if (::bind(nListenSocket, (sockaddr *) &ServerAddress, sizeof(sockaddr_in)) == -1) {
        perror("bind error");
        ::close(nListenSocket);
        return 0;
    }

    // 开启监听
    if (::listen(nListenSocket, 23) == -1) {
        perror("listen error");
        ::close(nListenSocket);
        return 0;
    }


    sockaddr_in ClientAddress;
    socklen_t LengthOfClientAddress = sizeof(sockaddr_in);

    cout << "启动 server 成功, 开始接受连接..." << endl;
    while (true) {
        // 接受连接
        int nConnectedSocket = ::accept(nListenSocket, (sockaddr *) &ClientAddress, &LengthOfClientAddress);
        if (-1 == nConnectedSocket) {
            perror("accept error");
            ::close(nListenSocket);
            //break;
        }
        int buf[256];
        int round = 0;
        while (true) {
            memset(buf, 0, sizeof(buf));
            recv(nConnectedSocket, buf, sizeof(buf), 0);
            if (buf[0] == 1) {
                cout << "服务端动作：搜索" << endl;
                // searchAll
                int schoolID = buf[1];
                vector<Item> items = searchAll(schoolID);
                int result[256] = {0};
                int j = 0;
                result[j++] = items.size();
                for (int i = 0; i < items.size(); i++) {
                    Item item = items[i];
                    result[j++] = item.id;
                    result[j++] = item.price;
                    result[j++] = item.amount;
                }
                send(nConnectedSocket, result, sizeof(result), 0);
                // 传输回去

            } else if (buf[0] == 2) {
                // buy
                cout << "服务端动作：购买" << endl;
                int schoolID = buf[1];
                int itemID = buf[2];
                purchase(itemID, schoolID);
                char result[256] = "buy success";
                send(nConnectedSocket, result, sizeof(result), 0);
            } else if (buf[0] == 3) {
                // add
                cout << "服务端动作：新增" << endl;
                int schoolID = buf[1];
                Item *item = new Item(buf[2], buf[3], buf[4]);
                add(item, schoolID);
                char result[256] = "add success";
                send(nConnectedSocket, result, sizeof(result), 0);
            } else if (buf[0] == 4) {
                // update
                cout << "服务端动作：更新" << endl;
                int schoolID = buf[1];
                int itemID = buf[2];
                Item *item = new Item(buf[3], buf[4], buf[5]);
                update(itemID, item, schoolID);
                char result[256] = "update success";
                send(nConnectedSocket, result, sizeof(result), 0);
            } else if (buf[0] == 5) {
                // remove
                cout << "服务端动作：删除" << endl;
                int schoolID = buf[1];
                int itemID = buf[2];
                remove(itemID, schoolID);
                char result[256] = "remove success";
                send(nConnectedSocket, result, sizeof(result), 0);
            }
            round++;
            if (round == 3) {
                close(nConnectedSocket);
            }

        }
        close(nConnectedSocket);
    }

    return 0;
}

Server::~Server() = default;

/**
 * 首先存数量
 * @return
 */
vector<Item> Server::searchAll(int schoolID) {
    vector<Item> result;
    string dbName = "db_" + to_string(schoolID);

    int fd = open(dbName.c_str(), O_RDWR, 0664);


    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd, &count, 4);

    cout << "当前共有 " + to_string(count) + " 个物品" << endl;

    for (int i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        int i_price;
        int i_amount;
        read(fd, &i_id, 4);
        read(fd, &i_price, 4);
        read(fd, &i_amount, 4);
        Item item = Item(i_id, i_price, i_amount);
        result.push_back(item);
    }

    if (-1 == close(fd)) {
        perror("server close");
    }
    return result;
}

void Server::add(Item *item, int schoolID) {
    string dbName = "db_" + to_string(schoolID);

    if (-1 == open(dbName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0664)) {
        // 表文件已经存在，说明表已经被创建过
        if (17 == errno) {
            string msg = "「" + dbName + "」already exists, create fail.";
            cout << msg << endl;
        }
    } else {
        int fd = open(dbName.c_str(), O_WRONLY | O_CREAT, 0664);
        if (-1 == fd) {
            perror("server db error open");
        }
        int count = 0;
        write(fd, &count, 4);
        close(fd);
    }

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd, &count, 4);

    cout << "当前共有 " + to_string(count) + " 个物品" << endl;

    if (-1 == lseek(fd, 4 + 3 * 4 * count, SEEK_SET)) {
        perror("server lseek");
    }

    write(fd, &item->id, 4);
    write(fd, &item->price, 4);
    write(fd, &item->amount, 4);

    cout << "add 新物品 成功" << endl;

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    count++;
    write(fd, &count, 4);


    if (-1 == close(fd)) {
        perror("server close");
    }

}

void Server::update(int itemID, Item *item, int schoolID) {

    string dbName = "db_" + to_string(schoolID);

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd, &count, 4);

    cout << "当前共有 " + to_string(count) + " 个物品" << endl;
    for (int i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        read(fd, &i_id, 4);
        if (i_id == itemID) {
            if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
                perror("server lseek");
            }
            write(fd, &item->id, 4);
            write(fd, &item->price, 4);
            write(fd, &item->amount, 4);
            cout << "发现更新目标，更新完成" << endl;
        }
    }

    if (-1 == close(fd)) {
        perror("server close");
    }
}

/**
 * 采取全部前移的方式
 * @param itemID
 */
void Server::remove(int itemID, int schoolID) {
    string dbName = "db_" + to_string(schoolID);

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd, &count, 4);

    cout << "当前共有 " + to_string(count) + " 个物品" << endl;
    int i;
    for (i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        read(fd, &i_id, 4);
        if (i_id == itemID) {
            // 定位成功
            cout << "成功定位到需要删除的物品" << endl;
            break;
        }
    }
    for (int j = i; j < count; j++) {
        if (-1 == lseek(fd, 4 + (j + 1) * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }

        int j_id;
        int j_price;
        int j_amount;
        read(fd, &j_id, 4);
        read(fd, &j_price, 4);
        read(fd, &j_amount, 4);

        if (-1 == lseek(fd, 4 + j * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }
        write(fd, &j_id, 4);
        write(fd, &j_price, 4);
        write(fd, &j_amount, 4);
    }


    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    count--;
    write(fd, &count, 4);

    if (-1 == close(fd)) {
        perror("server close");
    }
}


void Server::purchase(int itemID, int schoolID) {

    string dbName = "db_" + to_string(schoolID);

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd, &count, 4);

    cout << "当前共有 " + to_string(count) + " 个物品" << endl;
    for (int i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        int i_price;
        int i_amount;
        read(fd, &i_id, 4);
        read(fd, &i_price, 4);
        read(fd, &i_amount, 4);
        if (i_id == itemID) {
            if (-1 == lseek(fd, 4 + i * 4 * 3, SEEK_SET)) {
                perror("server lseek");
            }
            write(fd, &i_id, 4);
            write(fd, &i_price, 4);
            int afterBuyAmount = i_amount - 1;
            write(fd, &afterBuyAmount, 4);
        }
    }

    if (-1 == close(fd)) {
        perror("server close");
    }
}
