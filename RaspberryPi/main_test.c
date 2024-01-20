#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <time.h>
#include <Romulus-M/crypto_aead.h>

#define MAX 41 
#define PORT 8080 
#define SA struct sockaddr
#define TLEN 128
int dht11_dat[4] = {0, 0, 0, 0};
unsigned char k[128] = "This is a super secret key that no one can crack. But since we need to have 128 characters, we have to type other stuff........";
unsigned char nsec[128] = "This is a SECRET NONCE, it could be any data actually, but we prefer to set is as it's definetely easier to do it this way.....";

void get_temperature(char * m) {
    dht11_dat[2] = rand() % 100 - 50;
    for (int i = 0; i < 4; i++) {
        if (i == 2)
            continue;
        dht11_dat[i] = rand() % 100;
    }
    snprintf(m, 40, "Humidity = %d.%d Temperature = %d.%d C",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
}

int main(int argc, char ** argv) {

    if (argc < 1) {
        printf("Usage: main <ip_address>\n");
        return 255;
    }

    srand(time(NULL));

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    char in[MAX];
    unsigned long long mlen = strlen(in);
    unsigned long long clen = 40 + TLEN; // Appears to be always 54 for this data
    unsigned char c[clen];

    unsigned char npub[128];

    unsigned char ad[] = "RaspberryPI data";
    unsigned long long adlen = 16; // Hard coded length of previous string

    while (1) {
        // Get temperature from sensor
        get_temperature(in);
        printf("%s\n", in);

        // Encrypt temperature from sensor
        mlen = strlen(in);
        for (int i = 0; i < 128; i++) {
            npub[i] = rand() % 256;
        }
        crypto_aead_encrypt(c, &clen, in, mlen, ad, adlen, nsec, npub, k);

        // Send temperature to server
        send(sockfd, npub, 128, 0);

        send(sockfd, &clen, sizeof(unsigned long long), 0);

        send(sockfd, c, clen, 0);

        sleep(10);
    }
 
    // close the socket
    close(sockfd);

    return 0;
} 