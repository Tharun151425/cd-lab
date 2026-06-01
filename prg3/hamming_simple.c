#include <stdio.h>

int main(){
    int data[8], recv[8];
    printf("Enter four bit data :");
    scanf("%d", &data[3]);
    scanf("%d", &data[5]);
    scanf("%d", &data[6]);
    scanf("%d", &data[7]);

    data[1] = data[3] ^ data[5] ^ data[7];
    data[2] = data[3] ^ data[6] ^ data[7];
    data[4] = data[5] ^ data[6] ^ data[7];

    printf("The data encoded :");
    for(int i=1; i<8; i++){
        printf("%d", data[i]);
    }
    printf("\n Enter the received data :");
    for (int i=1; i<8; i++)
        scanf("%d", &recv[i]);
    int c0 = recv[1] ^ recv[3] ^ recv[5] ^ recv[7];
    int c1 = recv[2] ^ recv[3] ^ recv[6] ^ recv[7];
    int c2 = recv[4] ^ recv[5] ^ recv[6] ^ recv[7];
    int c = c2*4 + c1*2 + c0;

    if (c==0) printf("\n No errors \n");
    else{
        printf("Error at position : %d", c);
        recv[c] ^= 1;
    }
    printf("The correct data is :");
    for (int i=1; i<8; i++) printf("%d", recv[i]);
}