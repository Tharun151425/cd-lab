#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Run as: ./client 127.0.0.1
int main(int argc, char *argv[]) {
    int fd;
    struct sockaddr_in saddr;
    char buf[1024];

    // 1-4. Create Socket and fill server details
    fd = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(15001);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    // 5. Establish Connection
    if (connect(fd, (struct sockaddr *)&saddr, sizeof(saddr)) == 0) {
        printf("Connected to server! Type a message (Ctrl+C to exit):\n");
    }

    // Chat Loop
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        // 6. Send message
        send(fd, buf, strlen(buf), 0);
        
        memset(buf, 0, sizeof(buf)); // Clear buffer so old garbage doesn't print
        
        // 7-8. Receive result and write to output
        if (recv(fd, buf, sizeof(buf), 0) > 0) {
            printf("Server Echo: %s", buf);
        }
    }
    
    close(fd);
    return 0;
}