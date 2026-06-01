#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int listenfd, connfd, fp, n;
    struct sockaddr_in saddr, caddr;
    socklen_t clen;
    char fname[256], buf[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_port = htons(15001);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
    listen(listenfd, 5);

    clen = sizeof(caddr);
    connfd = accept(listenfd, (struct sockaddr *)&caddr, &clen);

    memset(fname,0,sizeof(fname));
    recv(connfd, fname, sizeof(fname), 0);
    printf("File name : %s", fname);

    fp = open(fname, O_RDONLY);
    memset(buf, 0, sizeof(buf));
    while ((n = read(fp, buf, sizeof(buf))) > 0){
        send(connfd, buf, n, 0);
        memset(buf, 0, sizeof(buf));
    }

    close(fp);
    close(listenfd);
    close(connfd);
    exit(0);
}