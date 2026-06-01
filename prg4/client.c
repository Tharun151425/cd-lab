#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd, reuse = 1;
    struct sockaddr_in laddr;
    struct ip_mreq group;
    char buf[256];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(5000);
    laddr.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sockaddr *)&laddr, sizeof(laddr));

    group.imr_multiaddr.s_addr = inet_addr("239.0.0.1");
    group.imr_interface.s_addr = INADDR_ANY;
    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));

    while (1) {
        memset(buf, 0, sizeof(buf));
        recv(fd, buf, sizeof(buf), 0);
        printf("Received msg from multicast: %s\n", buf);
    }
    close(fd);
}

/*
// --- TO ENABLE MULTIPLE INFINITE MESSAGES, USE THIS INSTEAD ---

// In client.c:
// You accidentally put `close(fd);` INSIDE the while(1) loop!
// It connects, receives ONE message, closes the socket immediately, 
// and then spins forever trying to read from a dead socket, causing the blank spam!
    while (1) {
        memset(buf, 0, sizeof(buf));
        recv(fd, buf, sizeof(buf), 0);
        printf("Received msg from multicast: %s\n", buf);
    }
    close(fd); // <--- Keep this outside the loop!
*/