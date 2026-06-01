#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Run as: ./client 127.0.0.1
int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in saddr;
    socklen_t slen = sizeof(saddr);
    char buf[1024];

    if (argc != 2) {
        printf("Usage: %s <Server IP>\n", argv[0]);
        exit(1);
    }

    // 1-4. Create UDP Socket and fill server destination details
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(16001);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("Type a message to send via UDP (Ctrl+C to exit):\n");

    // Chat Loop
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        
        // 5. Send message directly to server's address
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&saddr, slen);
        
        memset(buf, 0, sizeof(buf)); // Clear buffer
        
        // 6-7. Receive the echo back and print to standard output
        n = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&saddr, &slen);
        if (n > 0) {
            printf("Server Echo: %s", buf);
        }
    }

    close(sockfd);
    return 0;
}