#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sockfd, n;
    struct sockaddr_in saddr, caddr;
    socklen_t clen = sizeof(caddr);
    char buf[1024];

    // 1-4. Create UDP Socket and fill server details
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM for UDP
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(16001); // Manual uses 16001 for this one

    // 5. Bind the address
    bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    printf("UDP Echo Server ready on port 16001... Catching airplanes!\n");

    // Infinite loop to catch messages and throw them back
    while (1) {
        memset(buf, 0, sizeof(buf)); // Clear old garbage
        
        // 6. Receive message (Catch the airplane and note who sent it in 'caddr')
        n = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&caddr, &clen);
        
        // 7. Send the exact message back to the client
        sendto(sockfd, buf, n, 0, (struct sockaddr *)&caddr, clen);
    }

    close(sockfd);
    return 0;
}