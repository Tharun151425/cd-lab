#include <stdio.h>
#include <math.h>

int main(){
    int m,r = 1;
    int data[20], ham[20], recv[20];
    printf("Enter the no of data bits :");
    scanf("%d", &m);
    
    printf("Enter data bits :");
    for (int i=m; i>=1; i--)
        scanf("%d", &data[i]);
    while (pow(2,r) < m + r + 1) r++;

    int total = m + r, j = 1, k = 0;
    for(int i=1; i<=total; i++){
        if (i == pow(2,k)){
            ham[i] = 0;
            k++;
        }
        else{
            ham[i] = data[j];
            j++;
        }
    }

    for (int i=0; i<r; i++){
        int pos = pow(2,i), parity = 0;
        for (j = 1; j <= total; j++){
            if (j & pos)
                parity ^= ham[j];
        }
        ham[pos] = parity;
    }
    printf("Generated Hamming code :\n");
    for (int i=total; i>=1; i--) printf("%d",ham[i]);

    printf("Enter received hamming code :");
    for (int i=total; i>=1; i--)
        scanf("%d", &recv[i]);
    
    int err = 0;
    for (int i=0; i<r; i++){
        int pos = pow(2,i), parity = 0;
        for (j = 1; j <= total; j++){
            if (j & pos) parity ^= recv[j];
        }
        if (parity != 0) err += pos;
    }

    if (err == 0)
        printf("No error detected");
    else{
        printf("Error at pos : %d", err);
        recv[err] ^= 1;
        printf("Corrected code :");
        for (int i=total; i>=1; i--)
            printf("%d", recv[i]);
    }
    return 0;
}