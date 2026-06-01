#include<arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    // pid_t childpid;
    char buff[1024];
    socklen_t clen;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_port = htons(5000);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    printf("Server waiting for clients \n");

    while (1){
        clen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clen);
        
        if (fork() == 0){
            while (1){
                memset(buff, 0, sizeof(buff));
                recv(connfd, buff, sizeof(buff),0);
                printf("Client : %s",buff);
                
                memset(buff, 0, sizeof(buff));
                printf("Server :");
                fgets(buff, sizeof(buff), stdin);
                send(connfd, buff, strlen(buff),0);
            }
        }
    }
    close(listenfd);
    return 0;
}