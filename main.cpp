#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <zconf.h>

#include <arpa/inet.h>
#define MAXBUFSIZE 65536 // Max UDP Packet size is 64 Kbyte
int main() {
    std::cout << "Hello, World!" << std::endl;

    int sock, status, socklen;
    char buffer[MAXBUFSIZE];
    struct sockaddr_in saddr;
    struct in_addr iaddr;
    unsigned char ttl = 3;
    unsigned char one = 1;
// set content of struct saddr and imreq to zero
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    memset(&iaddr, 0, sizeof(struct in_addr));
// open a UDP socket
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if ( sock < 0 )
        perror("Error creating socket"), exit(0);
    saddr.sin_family = PF_INET;
    saddr.sin_port = htons(0); // Use the first free port
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind socket to any interface
    status = bind(sock, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if ( status < 0 )
        perror("Error binding socket to interface"), exit(0);
    iaddr.s_addr = INADDR_ANY; // use DEFAULT interface
// Set the outgoing interface to DEFAULT
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
               sizeof(struct in_addr));
// Set multicast packet TTL to 3; default TTL is 1
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
               sizeof(unsigned char));
// send multicast traffic to myself too
    status = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
                        &one, sizeof(unsigned char));
// set destination multicast address
    saddr.sin_family = PF_INET;
    saddr.sin_addr.s_addr = inet_addr("226.0.0.1");
    saddr.sin_port = htons(4096);
// put some data in buffer
    char msg[] = "Hello world\n";
    strcpy(buffer, msg);
    socklen = sizeof(struct sockaddr_in);
// receive packet from socket
    while(true)
    {

        status = sendto(sock, buffer, strlen(buffer), 0,
                        (struct sockaddr *)&saddr, socklen);
        printf("\nsendto Status = %d\n", status);
        printf(" \nsending [%d]bytes\n", sizeof(msg));
        printf("\nstr msg size is [%d]bytes\n", strlen(buffer));
        sleep(1);
    }
//    status = sendto(sock, buffer, strlen(buffer), 0,
//                    (struct sockaddr *)&saddr, socklen);
// shutdown socket
    //shutdown(sock, 2);
// close socket
    close(sock);
    return 0;
}