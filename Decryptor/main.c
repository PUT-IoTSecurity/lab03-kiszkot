#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <Romulus-M/crypto_aead.h>

#define MAX 40 
#define PORT 8080 
#define SA struct sockaddr
#define TLEN 128
int dht11_dat[4] = {0, 0, 0, 0};
unsigned char k[128] = "This is a super secret key that no one can crack. But since we need to have 128 characters, we have to type other stuff........";
unsigned char nsec[128] = "This is a SECRET NONCE, it could be any data actually, but we prefer to set is as it's definetely easier to do it this way.....";

// Function designed for chat between client and server. 
void func(int connfd) 
{ 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, MAX); 
   
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
   
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); 
   
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
}

int main(int argc, char ** argv) {

    int sockfd, connfd, len; 
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
   
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 

    unsigned long long clen = MAX + TLEN;
    unsigned char c[clen];

    unsigned char npub[128];

    unsigned char ad[] = "RaspberryPI data";
    unsigned long long adlen = 16; // Hard coded length of previous string

    unsigned char m[MAX];
    unsigned long long mlen = MAX;

    while (1) {
        // Receive data
        recv(connfd, npub, 128, 0);
        recv(connfd, &clen, sizeof(unsigned long long), 0);
        recv(connfd, c, clen, 0);

        // Decrypt data
        crypto_aead_decrypt(m, &mlen, nsec, c, clen, ad, adlen, npub, k);

        // Print data
        printf("%s\n", m);
    }
   
    // After chatting close the socket 
    close(sockfd); 

    return 0;
} 