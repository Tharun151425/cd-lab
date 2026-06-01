#include <arpa/inet.h> //1.33
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int listenfd, connfd;
    struct sockaddr_in saddr, caddr;
    socklen_t clen;
    char cmd[100],res[1000];
    FILE *fp;

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9000);
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenfd,(struct sockaddr *)&saddr, sizeof(saddr));
    listen(listenfd, 5);

    clen = sizeof(caddr);
    connfd = accept(listenfd, (struct sockaddr *) &caddr, &clen);

    memset(cmd, 0, sizeof(cmd)); // Clear garbage before receiving
    recv(connfd, cmd, sizeof(cmd), 0);
    printf("Command recieved : %s\n", cmd);

    fp = popen(cmd,"r");
    res[0] = '\0';
    while (fgets(cmd, sizeof(cmd), fp) != NULL) strcat(res, cmd);
    pclose(fp);

    send(connfd, res, sizeof(res), 0);
    close(connfd);
    close(listenfd);
}