#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    int fd;
    struct sockaddr_in saddr;
    char cmd[100], res[10000];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));

    printf("Enter cmd :");
    fgets(cmd, sizeof(cmd), stdin);
    cmd[strcspn(cmd, "\n")] = '\0'; // Properly removes the newline character - cmd[strlen(cmd) - 1] = '\0'
    send(fd, cmd, strlen(cmd), 0);

    memset(res, 0, sizeof(res)); // Clear garbage before receiving
    recv(fd, res, sizeof(res), 0);
    printf("Result : \n%s\n", res);
    close(fd);
    return 0;
}