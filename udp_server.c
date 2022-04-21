#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 512


int main(int argc, char* argv[])
{
    struct sockaddr_in server;  // server (self) address info
    int sock;                   // server (self) socket
    char buf[BUF_SIZE];         // message buffer

    // parse arguments
    if (argc != 3) {
        printf("wrong number of arguments\n");
        printf("try: ./<f_name> <host_addr_ipv4> <host_port>\n");
        exit(1);
    }
    const char* host = argv[1];
    const int port = atoi(argv[2]);

    // configure server (self) address
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);
    memset(server.sin_zero, '\0', sizeof server.sin_zero);

    // ----------------------------------------------------

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Error creating socket\n");
        exit(1);
    }

    if (bind(sock, (struct sockaddr *) &server, sizeof server) == -1) {
        printf("Could not bind address %s\n", host);
        exit(1);
    }
    printf("Socket successfully bound at %s:%d\n",
           inet_ntoa(server.sin_addr),
           ntohs(server.sin_port));

    while (1) {
        if (read(sock, buf, BUF_SIZE) == -1) {
            perror("Error receiving datagram\n");
            exit(1);
        }
        printf("Data -> %s\n", buf);
    }
    close(sock);
    return 0;

}
