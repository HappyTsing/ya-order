#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../include/Server.h"

Server::Server(int id, int port, string ip) {
    this->id = id;
    this->port = port;
    this->ip = ip;
}

int Server::start() {
    int nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    std::cout <<"nListenSocket= " <<nListenSocket << std::endl;

    if(-1 == nListenSocket)
    {
        std::cout << "socket error" << std::endl;
        return 0;
    }

    sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(5000);

    if(::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1)
    {
        std::cout << "bind error" << std::endl;
        ::close(nListenSocket);
        return 0;
    }

    if(::listen(nListenSocket, 23) == -1)
    {
        std::cout << "listen error" << std::endl;
        ::close(nListenSocket);
        return 0;
    }


    sockaddr_in ClientAddress;
    socklen_t LengthOfClientAddress = sizeof(sockaddr_in);

    std::cout<<"before conjuction"<<std::endl;
    //设置超时时间片6s
    // struct timeval timeout = {6,0};
    // int state = 0;
    // int temp = 0;

    while(true)
    {
        //超时设置，对IO 套接字进行超时设置，如果超过时间片
        // if (setsockopt(nListenSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval)) != 0)
        // {
        //     printf("set accept timeout failed");
        // }
        //如果accept超时，则accept会返回-1
        int nConnectedSocket = ::accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
        if(-1 == nConnectedSocket)
        {
            std::cout << "accept error" << std::endl;
            ::close(nListenSocket);
            //break;
        }
        ::write(nConnectedSocket, "Server A is running\n", 20);

    }
    // std::cout<<"Process A has been killed"<<std::endl;
    // for(int i =0 ;i <1000; i++)
    // {
    //     std::cout<<"Process B is running   "<< std::endl;
    //     sleep(3);
    // }

    // int nConnectedSocket = ::accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
    //     if(-1 == nConnectedSocket)
    //     {
    //         std::cout << "accept error" << std::endl;
    //         ::close(nListenSocket);
    //         return 0;
    //     }
    //     ::write(nConnectedSocket, "Process A is running\n", 20);


    // ::close(nConnectedSocket);
    // ::close(nListenSocket);

    return 0;
}
