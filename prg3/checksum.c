#include <stdio.h>

unsigned short checksum(int n){
    unsigned short data;
    unsigned long sum = 0;
    for (int i=0; i<n; i++){
        printf("Enter 16-bit word %d (hex): ", i + 1);
        scanf("%hx", &data);
        sum += data;
        if (sum >> 16)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}


int main(){
    int n;
    unsigned short sender,receiver;
    printf("(Sender side) Enter no of 16 bit words :");
    scanf("%d", &n);
    sender = checksum(n);
    printf("\nComputer checksum at sender : %hx\n", sender);
    printf("Reciever side, Enter the recieved words along with checksum generated at sender size :");
    receiver = checksum(n+1);
    printf("\nComputer checksum at receiver : %hx\n", receiver);

    if (receiver == 0x0000)
        printf("\n NO errors in data recieved");
    else
        printf("\n Errors detected in data recieved");
    return 0;
}