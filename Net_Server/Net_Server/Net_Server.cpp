// Net_Server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define Buf_Size 1024
void Error(const char* msg);
void Log(const char* msg);

char strServerIP[] = "172.30.1.79";
char strPort[] = "12345";

void Server_Code();

int main()
{
    std::cout << "Server Start!\n";
    Server_Code();
    return 0;
}

void Server_Code()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket; 
    char msg[Buf_Size];
    int strLength, i;

    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        Error("WSAStartup() Error");
    Log("WSAStartup() OK");

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
        Error("socket() Error");
    Log("socket() OK");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(strPort));

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        Error("bind() Error");
    Log("bind() OK");

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        Error("listen() Error");
    Log("listen() OK");

    clientAddrSize = sizeof(clientAddr);
    for (int i = 0; i < SOMAXCONN; i++)
    {
        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        Log("accept()...");
        if (clientSocket == SOCKET_ERROR)
            Error("accept() Error");
        else
            printf("Connected client %d\n", i + 1);

        memset(msg, 0, sizeof(msg));

        while ((strLength = recv(clientSocket, msg, Buf_Size, 0)) != 0)
        {
            printf("%d client : %s", i + 1, msg);
            send(clientSocket, msg, strLength, 0);
            memset(msg, 0, sizeof(msg));
        }

        closesocket(clientSocket);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    Log("Finish...");
}

void Error(const char* msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void Log(const char* msg)
{
    fputs(msg, stdout);
    fputc('\n', stdout);
}