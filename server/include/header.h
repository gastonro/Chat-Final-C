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

static int client_id = 0;

typedef struct {
    struct sockaddr_in address;
    int fd;
    int client_id;
    int client_to_send_id;
    char username[20];
    char username_other_user[20];
    int state;

} client_struct;

client_struct *clients[MAX_CLIENTS];