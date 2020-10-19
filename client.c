#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define BUFFERSIZE 2000 
#define PORT 12000
#define SA struct sockaddr 


static inline void timespec_diff(struct timespec *a, struct timespec *b,
    struct timespec *result) {
    result->tv_sec  = a->tv_sec  - b->tv_sec;
    result->tv_nsec = a->tv_nsec - b->tv_nsec;
    if (result->tv_nsec < 0) {
        --result->tv_sec;
        result->tv_nsec += 1000000000L;
    }
}

void func(int sockfd) 
{ 
    char buffer[BUFFERSIZE]; 
    int n; 
    const int iterations = 1000;
    struct timespec tstart={0,0}, tend={0,0}, tresult={0,0};

    for(int c=0; c<20; c++)
    {    
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        int i=0;
        for (i=0; i<iterations;i++) { 
            bzero(buffer, sizeof(buffer));            
            buffer[0]=i;
            write(sockfd, buffer, sizeof(buffer)); 
            bzero(buffer, sizeof(buffer)); 
            read(sockfd, buffer, sizeof(buffer)); 
        }
        clock_gettime(CLOCK_MONOTONIC, &tend);
        timespec_diff(&tend, &tstart, &tresult);
        printf("%lld.%.9ld\n", (long long)tresult.tv_sec, tresult.tv_nsec);
    }
} 
  
int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
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
    servaddr.sin_addr.s_addr = inet_addr("10.11.64.30"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    func(sockfd); 
  
    // close the socket 
    close(sockfd); 
} 