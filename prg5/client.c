#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int fd;
    struct sockaddr_in saddr;
    char buf[1024];

    fd = socket(AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(5000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(fd, (struct sockaddr *) &saddr, sizeof(saddr));

    while (1){
        printf("Client :");
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        send(fd,buf,strlen(buf),0);
        
        memset(buf, 0, sizeof(buf));
        recv(fd,buf,sizeof(buf),0);
        printf("Server : %s",buf); 
    }
    close(fd);
    return 0;
}