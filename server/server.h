#ifndef SERVER_H
#define SERVER_H

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>
#include <locale.h>

#define BUF_SIZE 1024
#define MAX_FILES 100

typedef struct user {
    char ID[10]; 
    char PassWord[20];
    char **FileName;
    int file_index;
    struct user* next; 
} User;

extern SOCKET cliSocks[10];
extern HANDLE mtxMutex;
extern int clicount;
extern User* head;

#endif
