#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
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
    printf("Concurrent Server waiting for connections...\n");

    // Infinite loop to keep accepting new clients
    while (1) {
        // 7. Accept connection
        connfd = accept(listenfd, (struct sockaddr *)&caddr, &clen);
        printf("Client Connected!\n");

        // 8. Create child process using fork()
        if (fork() == 0) { 
            close(listenfd); // Child doesn't need to listen for new people

            // 9-10. Receive and Send (Echo Loop)
            while ((n = recv(connfd, buf, sizeof(buf), 0)) > 0) {
                printf("Client : %s", buf);
                send(connfd, buf, n, 0); 
            }
            
            close(connfd);
            exit(0); // Kill the child process when client disconnects
        }
        close(connfd); // Parent closes its copy of the connection and loops back to accept()
    }

    return 0;
}