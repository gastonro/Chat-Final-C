#include "header.h"

void add_client(client_struct *added_client) {
    for (int i=0; i< MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = added_client;
            return;
        }
    }

}

void who_i_am(char *s, int fd){
    char *command_to_client = "1";
    struct iovec message_sent_server[2];
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);

} 

void send_list_clients(int fd) {
    int i = 0;
    char s[64];
    char *command_to_client = "2";
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] && (clients[i]->state == 0)){
            sprintf(s + strlen(s), "| Name: %s | \n\r",clients[i] ->username);
            
        }

    }
    printf("%s",s);
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
}

int search_client_nickname(char *s) {
    int i = 0;
    int cont = 0;
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){

        if (clients[i]) {
            if (strcmp(clients[i]->username,s) == 0){
            return 1;
            }
        }
    }
    return 0;

}

int search_client_id(char *s) {
    int i = 0;
    int id_client;
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){

        if (clients[i]) {
            if (strcmp(clients[i]->username,s) == 0){
            id_client = clients[i]->client_id;
            return id_client;
            }
        }
    }
}
void send_connection(int fd){
    char *command_to_client = "3";
    char s[64];
    sprintf(s, "Aceptado");
    struct iovec message_sent_server[2];
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
}

void send_connection_refuse(int fd){
    char *command_to_client = "4";
    char s[64];
    sprintf(s, "Denegado");
    struct iovec message_sent_server[2];
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
}

void send_message_client_to_client(char *s,int id) {
    char *command_to_client = "5";
    int i;
    int fd;
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){
    if (clients[i] && (clients[i]->client_id == id)){
        fd = clients[i]->fd;
        }
    }
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
    
}

int change_state(int id) {
    int i = 0;
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){

        if (clients[i]) {
            if (clients[i]->client_id == id ){
            clients[i]->state++;
            printf("Cambiado estado del cliente a busy\n");
            }
        }
    }
}

int change_state_log_off(int id) {
    int i = 0;
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){

        if (clients[i]) {
            if (clients[i]->client_id == id ){
            clients[i]->state--;
            printf("Cambiado estado del cliente a idle\n");
            }
        }
    }
}


void store_data(const char *data) {
    FILE *fp = fopen("Server_log.txt", "ab");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}

void end_chat(int fd) {
    char *command_to_client = "6";
    char s[64];
    sprintf(s, "Bye");
    struct iovec message_sent_server[2];
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
}

void client_log_off(int fd) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            if(clients[i]->fd == fd){
                clients[i] = NULL;
            }
        }
    }
}

void *chat(void * arg ){


    struct iovec message_recieved_client[2];
    char buffo1[1];
    char buffo2[512];
    char *message_from_client = NULL;
    char *get_client_value1 = NULL;
    char *get_client_value2 = NULL;
    char command_from_client;
    char data[1024];
    message_recieved_client[0].iov_base = buffo1;
    message_recieved_client[0].iov_len = sizeof(buffo1);
    message_recieved_client[1].iov_base = buffo2;
    message_recieved_client[1].iov_len = sizeof(buffo2);


    // MESSAGE TIME
    time_t t;
    struct tm *tm;
    char chat_date[100];
    t=time(NULL);
    tm=localtime(&t);
    strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
    printf("%s ", chat_date);

    client_struct *client = (client_struct *) arg;
    printf("Cliente connectado ID %d \n ", client->client_id);


    while (readv(client->fd, message_recieved_client, 2) > 0 ){
        
        printf("\n");
        get_client_value1 = message_recieved_client[0].iov_base;
        get_client_value2 = message_recieved_client[1].iov_base;
        
        command_from_client = get_client_value1[0];
        message_from_client = message_recieved_client[1].iov_base;
        printf("Mensaje de cliente recibido\n");

        switch (command_from_client)
        {
            case '1':
                strcpy(client->username,message_from_client);
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s asignado nickname \n",chat_date,client->username);
                printf ("%s",data);
                store_data(data);
                who_i_am(message_from_client,client->fd);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            
            case '2':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s solicito lista \n",chat_date,client->username);
                printf("%s",data);
                store_data(data);
                send_list_clients(client->fd);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            
            case '3':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s solicito chatear con: %s \n",chat_date,client->username,message_from_client);
                printf("%s", data);
                store_data(data);
                if (search_client_nickname(message_from_client)) {
                    send_connection(client->fd);
                    client->client_to_send_id =  search_client_id(message_from_client);
                    strcpy(client->username_other_user,message_from_client);
                    sprintf(data, "%s Conexion establecida de %s con %s \n",chat_date,client->username, client->username_other_user);
                    printf("%s",data);
                    store_data(data);
                    change_state(client->client_to_send_id);

                } else {
                    printf("Cliente no encontrado");
                    send_connection_refuse(client->fd);
                }
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;

            case '4':

                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s envio mensaje a usuario: %s, mensaje: %s \n"
                                                ,chat_date,client->username,client->username_other_user, message_from_client);
                printf("%s", data);
                store_data(data);
                send_message_client_to_client(message_from_client,client->client_to_send_id);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            case '5':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s solicito desconectarse del chat: %s \n",chat_date,client->username,message_from_client);
                printf("%s", data);
                store_data(data);
                end_chat(client->fd);
                change_state_log_off(client->client_to_send_id);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            
            case '6':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                sprintf(data, "%s Usuario: %s solicito desconexion servidor \n",chat_date,client->username);
                printf("%s", data);
                store_data(data);
                client_log_off(client->fd);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            default:
                break;
            }
    printf("\n");
    }
    close (client->fd);

} 
int main(int argc, char** argv) {
 
    char *error;
 
    int sockid, conn_sock;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t thread;
 
    //Socket creation
    if((sockid=socket(AF_INET,SOCK_STREAM,0))<0){
        error="socket";
        goto err;
    }
 
 
    //Socket configuration
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET; //address family
    server_address.sin_port=htons(SERVER_PORT);  //atoi ascii to integer
    server_address.sin_addr.s_addr=htonl(INADDR_ANY); //0.0.0.0
 
 
    //Bind the address to the socket
    if ((bind(sockid, (struct sockaddr *)&server_address, sizeof(server_address)))<0 )    {
        error="bind";
        goto err;
    }
 
    // Listen to queue
    if (listen(sockid, MAX_CLIENTS) < 0)    {
        error="listen";
        goto err;

    }
    printf("SERVER ONLINE \n");
    printf("WAITING FOR USERS \n");

    while(1){
        socklen_t client_len = sizeof(client_address);
        
        conn_sock = accept(sockid, (struct sockaddr *)&client_address, &client_len);
        if (conn_sock < 0){
            error="accept";
            goto err;
        }

        //ADD CLIENT CONFIGURATION
        client_struct *client = (client_struct *) malloc(sizeof(client_struct));
        client->address = client_address;
        client->client_id = client_id++;
        client->fd = conn_sock;
        client->state = 0;
        client->client_to_send_id = 0; 

        // ADD CLIENT TO QUEUE AND CREATE THREAD
        add_client(client);
        pthread_create(&thread, NULL, &chat, (void *)client);
        sleep(1);

    }
  
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}


