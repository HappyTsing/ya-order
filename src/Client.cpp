#include "../include/Client.h"
#include "../include/Utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace std;

Client::Client(int schoolID,int type) {
    this->schoolID = schoolID;
    this->type = type;
}

int Client::start() {
    const char *ip;
    int port;

    if (this->schoolID == 1) {
        ip = "127.0.0.1";
        port = 5000;

    } else if (this->schoolID == 2) {
        ip = "127.0.0.1";
        port = 6000;
    } else {
        cout << "error schoolID: " << this->schoolID << endl;
    }


    int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == nClientSocket) {
        perror("socket error");
        return 0;
    }
    //init
    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    if (::inet_pton(AF_INET, ip, &ServerAddress.sin_addr) != 1) {
        perror("inet_pton error");
        return 0;
    }

    ServerAddress.sin_port = htons(port);
    //connent

    sleep(1);
    //这里只考虑两台服务器
    if (::connect(nClientSocket, (sockaddr *) &ServerAddress, sizeof(ServerAddress)) == -1) {
        perror("connect error");
        std::cout << "connect server" << this->schoolID << " error" << std::endl;
        std::cout << "Server " << this->schoolID << " crushed " << std::endl;
        //这里connect连接失败则认为A服务器崩溃，连接跳转到服务器B
        if (this->schoolID == 1) {
            port = 6000;

        } else if (this->schoolID == 2) {
            port = 5000;
        }

        ServerAddress.sin_port = htons(port);
        sleep(4);
        if (::connect(nClientSocket, (sockaddr *) &ServerAddress, sizeof(ServerAddress)) == -1) {
            return 0;
        }
    }

    this->clientSocket = nClientSocket;
    cout << "启动 client 成功" << std::endl;

    if(type == STUDENT){
        printStudentClientHelpMessage();
    } else if(type == STORE){
        printStoreClientHelpMessage();
    }
    char command[256];
    while (true) {
        printLineHeader();
        cin.getline(command, 256);
        if (strncasecmp(command, "help", 4) == 0) {
            if(type == STUDENT){
                printStudentClientHelpMessage();
            } else if(type == STORE){
                printStoreClientHelpMessage();
            }
            continue;
        } else if (strncasecmp(command, "exit", 4) == 0) {
            printExitMessage();
            break;
        } else if (strncasecmp(command, "show", 4) == 0) {
            searchAll();
            continue;
        }

        if(type == STUDENT){
            if (strncasecmp(command, "buy", 3) == 0) {
                vector<string> words = lexer(command);
                int itemId = stoi(words[1]);
                purchase(itemId);
            } else {
                printErrorCommandMessage(command);
            }
        } else if(type == STORE){
            if (strncasecmp(command, "add", 3) == 0) {
                vector<string> words = lexer(command);
                int itemId = stoi(words[1]);
                int itemPrice = stoi(words[2]);
                int itemAmount = stoi(words[3]);
                Item *item = new Item(itemId, itemPrice,itemAmount);
                add(item);
            } else if (strncasecmp(command, "update", 6) == 0) {
                vector<string> words = lexer(command);
                int itemId = stoi(words[1]);
                int itemIdNew = stoi(words[2]);
                int itemPriceNew = stoi(words[3]);
                int itemAmountNew = stoi(words[4]);
                Item *item = new Item(itemIdNew, itemPriceNew,itemAmountNew);
                update(itemId,item);
            } else if (strncasecmp(command, "remove", 6) == 0) {
                vector<string> words = lexer(command);
                int itemId = stoi(words[1]);
                remove(itemId);
            }  else {
                printErrorCommandMessage(command);
            }
        }
    }
    return 0;
}

bool Client::close() {
    ::close(this->clientSocket);
}

void Client::searchAll() {
    int data[256] = {1, this->schoolID};
    int result[256];
    memset(result, 0, sizeof(result));
    send(this->clientSocket, data, sizeof(data), 0);
    recv(this->clientSocket, result, sizeof(result), 0);
    vector<Item> items;
    int itemsCount = result[0];
    int j = 1;
    for (int i = 0; i < itemsCount; i++) {
        int i_id = result[j++];
        int i_price = result[j++];
        int i_amount = result[j++];
        Item item = Item(i_id, i_price, i_amount);
        items.push_back(item);
    }
    printSplit();
    printTableHeader();
    printSplit();
    for (int i = 0; i < itemsCount; i++) {
        printItem(items[i]);
    }
    printSplit();
}

void Client::purchase(int itemID) {
    int data[256] = {2, this->schoolID};
    data[2] = itemID;
    char result[256];
    send(this->clientSocket, data, sizeof(data), 0);
    recv(this->clientSocket, result, sizeof(result), 0);
    std::cout << "从服务端获取的数据： " << result << std::endl;
}

void Client::add(Item *item) {
    int data[256] = {3, this->schoolID};
    int j = 2;
    data[j++] = item->id;
    data[j++] = item->price;
    data[j++] = item->amount;
    char result[256];
    send(this->clientSocket, data, sizeof(data), 0);
    recv(this->clientSocket, result, sizeof(result), 0);
    std::cout << "从服务端获取的数据： " << result << std::endl;
}

void Client::update(int itemID, Item *item) {
    int data[256] = {4, this->schoolID};
    int j = 2;
    data[j++] = itemID;
    data[j++] = item->id;
    data[j++] = item->price;
    data[j++] = item->amount;
    char result[256];
    send(this->clientSocket, data, sizeof(data), 0);
    recv(this->clientSocket, result, sizeof(result), 0);
    std::cout << "从服务端获取的数据： " << result << std::endl;
}


void Client::remove(int itemID) {
    int data[256] = {5, this->schoolID};
    data[2] = itemID;
    char result[256];
    send(this->clientSocket, data, sizeof(data), 0);
    recv(this->clientSocket, result, sizeof(result), 0);
    std::cout << "从服务端获取的数据： " << result << std::endl;
}

Client::~Client() {
    this->close();
}