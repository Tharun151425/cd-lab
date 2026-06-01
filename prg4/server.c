#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd;
    struct sockaddr_in gaddr;
    char msg[] = "Hello broadcast Clients!";
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    gaddr.sin_family = AF_INET;
    gaddr.sin_port = htons(5000);
    gaddr.sin_addr.s_addr = inet_addr("239.0.0.1");
    
    // unsigned char loop = 1;
    // setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    // struct in_addr local_if;
    // local_if.s_addr = inet_addr("0.0.0.0");
    // setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &local_if, sizeof(local_if));
    while (1) {
        printf("Enter message to broadcast: ");
        memset(msg, 0, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0'; // Always strip the newline!
        
        sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&gaddr, sizeof(gaddr));
        printf("Broadcasted!\n");
    }
    printf("Message broadcasted to 239.0.0.1 on port 5000");
    close(fd);
}

/*
// --- TO ENABLE MULTIPLE INFINITE MESSAGES, USE THIS INSTEAD ---

// In server.c:
// You must declare `msg` as a larger empty array, NOT a hardcoded string layout!
// Change `char msg[] = "Hello...";` to `char msg[1024];`
    while (1) {
        printf("Enter message to broadcast: ");
        memset(msg, 0, sizeof(msg));
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0'; // Always strip the newline!
        
        sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&gaddr, sizeof(gaddr));
        printf("Broadcasted!\n");
    }
*/   