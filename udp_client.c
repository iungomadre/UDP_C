#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 512
#define DATA "abcdefghijklmnoprstuwxyz"


int main(int argc, char* argv[])
{
    struct sockaddr_in receiver;// server (receiver) address info
    int sock;                   // server (receiver) socket

    // parse arguments
    if (argc != 3) {
        printf("wrong number of arguments\n");
        printf("try: ./<f_name> <host_addr_ipv4> <host_port>\n");
        exit(1);
    }
    const char* host = argv[1];
    const int port = atoi(argv[2]);

    // configure server (receiver) address
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(port);
    receiver.sin_addr.s_addr = inet_addr(host);
    memset(receiver.sin_zero, '\0', sizeof receiver.sin_zero);

    // ----------------------------------------------------

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Error creating socket\n");
        exit(1);
    }

    // no need to bind, will only be sending data

    printf("Sending data -> %s", DATA);
    if (sendto(sock, DATA, sizeof DATA, 0,
               (struct sockaddr *) &receiver,
               sizeof receiver) == -1) {
        perror("Error sending datagram\n");
        exit(1);
    }

    close(sock);
    return 0;

}