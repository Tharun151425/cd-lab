#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

long int gcd(long int a, long int b){
    if (b==0) return a;
    return gcd(b,a%b);
}

long int isprime(long int a){
    if (a < 2) return 0;
    for (int i=2; i<= sqrt(a); i++)
        if (a%i == 0) return 0;
    return 1;
}

long int encrypt(char ch, long int n, long int e){
    long int res = 1, base = (long int)ch;
    while (e > 0){
        if (e%2) res = (res * base) % n;
        base = (base * base) % n;
        e = e / 2;
    }
    return res;
}

long int decrypt(long int ch, long int n, long int d){
    long int res = 1, base = ch;
    for(int i=0;i<d;i++)
        res = (res * base) % n;
    return (char)res;
}

int main(){
    srand(time(NULL));
    char msg[50];
    long int p,q,n,phi,d,e,cipher[50];

    printf("Enter the text to be encrypted : ");
    fgets(msg, sizeof(msg), stdin);
    msg[strcspn(msg,"\n")] = '\0';
    int len = strlen(msg);

    do {
        p = rand() % 50;
    } while(!isprime(p));

    do {
        q = rand() % 50;
    } while(!isprime(q) || p == q);

    n = p * q;
    phi = (p-1)*(q-1);

    do {
        e = rand() % phi;
    } while(gcd(e,phi) != 1 || e <= 1);

    d = 1;
    while ( ((d*e) % phi) != 1) d++;

    printf("Two prime numbers (p and q) are: %ld and %ld\n", p, q);
    printf("n(p * q) = %ld\n", p * q);
    printf("(p - 1) * (q - 1) = %ld\n", phi);
    printf("Public key (n,  e): (%ld, %ld)\n", n, e);
    printf("Private key (n, d): (%ld, %ld)\n", n, d);

    for(int i=0; i<len; i++){
        cipher[i] = encrypt(msg[i],n,e);
    }
    printf("Encrypted msg :");
    for (int i=0; i<len; i++) printf("%ld ", cipher[i]);

    for (int i=0; i<len; i++) 
        msg[i] = decrypt(cipher[i], n, d);
    msg[len] = '\0';

    printf("\nDecrypted message: \n");
    for (int i = 0; i < len; i++)
        printf("%c", msg[i]);
}
