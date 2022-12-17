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

    if(-1 == nListenSocket)
    {
        perror("socket error");
        return 0;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(this->port);

    // 绑定 ip 和 port
    if(::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
    {
        perror("bind error");
        ::close(nListenSocket);
        return 0;
    }

    // 开启监听
    if(::listen(nListenSocket, 23) == -1)
    {
        perror("listen error");
        ::close(nListenSocket);
        return 0;
    }


    sockaddr_in ClientAddress;
    socklen_t LengthOfClientAddress = sizeof(sockaddr_in);

    cout << "启动 server 成功, 开始接受连接..." << endl;
    while(true)
    {
        // 接受连接
        int nConnectedSocket = ::accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
        if(-1 == nConnectedSocket)
        {
            perror("accept error");
            ::close(nListenSocket);
            //break;
        }
        cout <<"开始发送数据"<< endl;
        // 收发数据，用函数send()和recv()，或者read()和write();
        ::write(nConnectedSocket, "This is message from server A\n", 50);
    }

    return 0;
}

Server::~Server() = default;
/**
 * 首先存数量
 * @return
 */
vector<Item> Server::searchAll() {
    vector<Item> result;
    string dbName = "db_" + to_string(this->id);

    int fd = open(dbName.c_str(), O_RDWR, 0664);


    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd,&count,4);

    cout << "当前共有 " + to_string(count) + " 个物品"  << endl;

    for(int i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4+i*4*3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        int i_price;
        int i_amount;
        read(fd,&i_id,4);
        read(fd,&i_price,4);
        read(fd,&i_amount,4);
        Item item = Item(i_id, i_price, i_amount);
        result.push_back(item);
    }

    if (-1 == close(fd)) {
        perror("server close");
    }
    return result;
}

void Server::add(Item *item) {
    string dbName = "db_" + to_string(this->id);

    if (-1 == open(dbName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0664)) {
        // 表文件已经存在，说明表已经被创建过
        if (17 == errno) {
            string msg = "「" + dbName + "」already exists, create fail.";
            cout << msg << endl;
        }
    }else{
        int fd = open(dbName.c_str(), O_WRONLY | O_CREAT, 0664);
        if (-1 == fd) {
            perror("server db error open");
        }
        int count = 0;
        write(fd,&count,4);
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
    read(fd,&count,4);

    cout << "当前共有 " + to_string(count) + " 个物品"  << endl;

    if (-1 == lseek(fd, 4 + 3*4*count, SEEK_SET)) {
        perror("server lseek");
    }

    write(fd,&item->id,4);
    write(fd,&item->price,4);
    write(fd,&item->amount,4);

    cout << "add 新物品 成功" << endl;

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    count ++;
    write(fd,&count,4);


    if (-1 == close(fd)) {
        perror("server close");
    }

}

void Server::update(int itemID, Item *item) {

    string dbName = "db_" + to_string(this->id);

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd,&count,4);

    cout << "当前共有 " + to_string(count) + " 个物品"  << endl;
    for(int i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4+i*4*3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        read(fd,&i_id,4);
        if(i_id == itemID){
            if (-1 == lseek(fd, 4+i*4*3, SEEK_SET)) {
                perror("server lseek");
            }
            write(fd,&item->id,4);
            write(fd,&item->price,4);
            write(fd,&item->amount,4);
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
void Server::remove(int itemID) {
    string dbName = "db_" + to_string(this->id);

    int fd = open(dbName.c_str(), O_RDWR, 0664);

    if (-1 == fd) {
        perror("server db error open");
    }

    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    int count;
    read(fd,&count,4);

    cout << "当前共有 " + to_string(count) + " 个物品"  << endl;
    int i;
    for(i = 0; i < count; i++) {
        // 定位到下一个商品
        if (-1 == lseek(fd, 4+i*4*3, SEEK_SET)) {
            perror("server lseek");
        }
        int i_id;
        read(fd,&i_id,4);
        if(i_id == itemID){
            break;
        }
    }
    for(int j = i ; j < count; j++){
        if (-1 == lseek(fd, 4+(j+1)*4*3, SEEK_SET)) {
            perror("server lseek");
        }

        int j_id;
        int j_price;
        int j_amount;
        read(fd,&j_id,4);
        read(fd,&j_price,4);
        read(fd,&j_amount,4);

        if (-1 == lseek(fd, 4+j*4*3, SEEK_SET)) {
            perror("server lseek");
        }
        write(fd,&j_id,4);
        write(fd,&j_price,4);
        write(fd,&j_amount,4);
    }


    if (-1 == lseek(fd, 0, SEEK_SET)) {
        perror("server lseek");
    }
    count --;
    write(fd,&count,4);

    if (-1 == close(fd)) {
        perror("server close");
    }
}