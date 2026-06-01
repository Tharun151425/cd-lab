#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(){
    long long p,g, a,b, A,B, keyA, keyB;
    printf("Enter the prime no(p) :");
    scanf("%lld",&p);
    printf("Enter the primitive root of p (g) :");
    scanf("%lld",&g);
    printf("Enter the alice private key (a) :");
    scanf("%lld",&a);
    printf("Enter the bob private key (b) :");
    scanf("%lld",&b);
    A = (long long int) pow(g,a) % p;
    B = (long long int) pow(g,b) % p;
    keyA = (long long int) pow(B,a) % p;
    keyB = (long long int) pow(A,b) % p;
    printf("Alice secret key : %lld \n Bob secret key : %lld",keyA, keyB);
}