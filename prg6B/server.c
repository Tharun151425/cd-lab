#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int listenfd, connfd, n;
    struct sockaddr_in saddr, caddr;
    socklen_t clen = sizeof(caddr);
    char buf[1024];

    // 1-4. Create Socket and fill address details
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(15001);

    // 5-6. Bind and Listen
    bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr));
    listen(listenfd, 3);
    printf("Iterative Server waiting for connections...\n");

    // Infinite loop to accept clients ONE AT A TIME
    while (1) {
        // 7. Accept the next completed connection
        connfd = accept(listenfd, (struct sockaddr *)&caddr, &clen);
        printf("Client Connected!\n");

        // 8-9. Receive and Send (Echo Loop)
        // -> THE SERVER IS TRAPPED HERE UNTIL THE CLIENT DISCONNECTS <-
        while ((n = recv(connfd, buf, sizeof(buf), 0)) > 0) {
            send(connfd, buf, n, 0); 
        }
        
        printf("Client Disconnected. Ready for the next client in line.\n");
        close(connfd); // Hang up on this client, loop back up to accept() the next one
    }

    close(listenfd);
    return 0;
}