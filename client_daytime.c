#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tools.h"

void
show_socket(int fd){
    struct sockaddr_in sockaddr;
    int len = sizeof(sockaddr);
    char addr[200];

    getsockname(fd, (SA *) &sockaddr, &len);
    inet_ntop(sockaddr.sin_family, &sockaddr.sin_addr, addr, sizeof(addr));
    printf("Socket = Len: %d, AF: %d, Addr: %s, Port: %d\n", len, sockaddr.sin_family, addr, ntohs(sockaddr.sin_port));
}

int
main(int argc, char *argv[])
{
    if(2 != argc){
        printf("Usage: %s <Ip address>\n", argv[0]);
        exit(0);
    }

    int fd, n;
    char recivline[MAXLINE+1];
    struct sockaddr_in addr;

    fd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    Inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);

    Connect(fd, (SA *)&addr, sizeof(addr));

    show_socket(fd);

    while((n = Readn(fd, recivline, MAXLINE))){
        recivline[n] = 0;
        Fputs(recivline, stdout);
    }
    exit(0);
}