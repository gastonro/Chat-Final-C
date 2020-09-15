#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include<sys/types.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define SERVER_PORT 8000
#define MAX_CLIENTS 10

// CLIENT STRUCTURE

typedef struct {
    int sockid;
    int chat_busy;
    char nickname_other_user[50];
    char nickname[50];
} client_data;
