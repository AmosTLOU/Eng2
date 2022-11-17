﻿#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "cMyNetworkServer.h"

using namespace std;


//void Networking::MyNetwork::sigchld_handler(int s)
//{
//    while (waitpid(-1, NULL, WNOHANG) > 0);
//}


// Get IPv4 or IPv6 sockaddr
void* Networking::MyNetworkServer::get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void Networking::MyNetworkServer::StartServer()
{
    std::thread serverThread = std::thread(Networking::MyNetworkServer::RunServer);
    serverThread.join();
}


int Networking::MyNetworkServer::RunServer()
{
    WSADATA wsaData; // if this doesn't work
    //WSAData wsaData; // then try this instead

    // MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    int sockfd, new_fd; // 在 sock_fd 進行 listen，new_fd 是新的連線
    struct addrinfo hints, * servinfo, * p;
    struct sockaddr_storage their_addr; // 連線者的位址資訊 
    socklen_t sin_size;
    //struct sigaction sa;
    int yes = 1;
    string lastClientIP = "";
    string curClientIP;
    string lastClientIndex = "";
    string curClientIndex;
    string lastMessage;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_flags = AI_PASSIVE; // 使用我的 IP

    if ((rv = getaddrinfo(IP, PORT, &hints, &servinfo)) != 0) {
        const char* errorMsg = reinterpret_cast<char*>(gai_strerror(rv));
        fprintf(stderr, "getaddrinfo: %s\n", errorMsg);
        return 1;
    }

    // 以迴圈找出全部的結果，並綁定（bind）到第一個能用的結果
    for (p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((sockfd = static_cast<int>(socket(p->ai_family, p->ai_socktype,
            p->ai_protocol))) == -1) 
        {
            perror("server: socket");
            continue;
        }
        const char* tmpVar = reinterpret_cast<char*>(&yes);
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, tmpVar,
            sizeof(int)) == -1) 
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, static_cast<int>(p->ai_addrlen)) == -1) 
        {
            closesocket(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL) 
    {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo); // 全部都用這個 structure

    if (listen(sockfd, BACKLOG) == -1) 
    {
        perror("listen");
        exit(1);
    }

    //sa.sa_handler = sigchld_handler; // 收拾全部死掉的 processes
    //sigemptyset(&sa.sa_mask);
    //sa.sa_flags = SA_RESTART;

    //if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    //    perror("sigaction");
    //    exit(1);
    //}

    printf("server: waiting for connections...\n");

    // 主要的 accept() 迴圈
    while (1)
    {
        sin_size = sizeof their_addr;
        new_fd = static_cast<int>(accept(sockfd, (struct sockaddr*)&their_addr, &sin_size));
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr*)&their_addr),
            s, sizeof s);
        curClientIP = s;
        printf("server: got connection from %s\n", s);

        //if (!fork()) { // 這個是 child process
        //    //close(sockfd); // child 不需要 listener
        //    if (send(new_fd, "Hello, world!", 13, 0) == -1)
        //        perror("send");
        //    //close(new_fd);
        //    exit(0);
        //}

        //STARTUPINFO si;
        //PROCESS_INFORMATION pi;
        //ZeroMemory(&si, sizeof(si));
        //si.cb = sizeof(si);
        //ZeroMemory(&pi, sizeof(pi));
        //// Start the child process. 
        //if (!CreateProcess(NULL,   // No module name (use command line)
        //    NULL,        // Command line
        //    NULL,           // Process handle not inheritable
        //    NULL,           // Thread handle not inheritable
        //    FALSE,          // Set handle inheritance to FALSE
        //    0,              // No creation flags
        //    NULL,           // Use parent's environment block
        //    NULL,           // Use parent's starting directory 
        //    &si,            // Pointer to STARTUPINFO structure
        //    &pi)           // Pointer to PROCESS_INFORMATION structure
        //    )
        //{
        //    printf("CreateProcess failed (%d)\n", GetLastError());
        //    if (send(new_fd, "Hello, world!", 13, 0) == -1)
        //        perror("send");
        //    return 2;
        //}
        //else
        //{
        //    printf("CreateProcess succeeded");
        //}
        //// Wait until child process exits.
        //WaitForSingleObject(pi.hProcess, INFINITE);
        //// Close process and thread handles. 
        //CloseHandle(pi.hProcess);
        //CloseHandle(pi.hThread);


        //closesocket(sockfd);

        // receive
        int numbytes;
        char buf[MAXDATASIZE];
        if ((numbytes = recv(new_fd, buf, MAXDATASIZE - 1, 0)) == -1) {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        curClientIndex = buf[0];
        printf("sever: received '%s'\n", buf);

        // send
        int mode = 0;
        string curClient, lastClient;
        if (mode == 0)
        {
            curClient = curClientIP;
            lastClient = lastClientIP;
        }
        else
        {
            curClient = curClientIndex;
            lastClient = lastClientIndex;
        }
        if (curClient == lastClient)
        {
            if (send(new_fd, "", numbytes, 0) == -1)
                perror("send");
        }
        else
        {
            if (send(new_fd, lastMessage.c_str(), static_cast<int>(lastMessage.length()), 0) == -1)
                perror("send");
        }
        lastClientIndex = curClientIndex;
        lastClientIP = curClientIP;
        lastMessage = buf;

        closesocket(new_fd);
    }

    return 0;
}


int Networking::MyNetworkServer::EndServer()
{
    WSACleanup();
    return 0;
}
