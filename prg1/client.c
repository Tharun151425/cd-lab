#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
    int fd, n;
    struct sockaddr_in saddr;
    char fname[256], buf[1024];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_port = htons(15001);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));

    printf("Enter file name :");
    fgets(fname, sizeof(fname), stdin);
    fname[strcspn(fname, "\n")] = '\0';
    send(fd, fname, strlen(fname), 0);

    printf("\nFile content------------------\n");
    memset(buf, 0, sizeof(buf));
    while ((n = recv(fd, buf, sizeof(buf), 0)) > 0){
        fputs(buf, stdout);
        memset(buf, 0, sizeof(buf));
    }
    close(fd);
    exit(0);
}