#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCK_ADDR "127.0.0.1"
#define SOCK_PORT 12345

char buf[BUFSIZ];

void
write_sock(int);

int
main(void)
{
    int fd;
    struct sockaddr_in sock;
    memset(&sock, 0, sizeof(sock));

    sock.sin_family = AF_INET;
    sock.sin_addr.s_addr = inet_addr(SOCK_ADDR);
    sock.sin_port = htons(SOCK_PORT);

    if(-1 == (fd = socket(AF_INET, SOCK_STREAM, 0))){
        exit(-1);
    }
    if(-1 == connect(fd, (struct sockaddr*)&sock, sizeof(sock))){
        exit(-1);
    }

    write_sock(fd);
    return 0;
}

void
write_sock(int fd)
{
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "hello, world!");
    write(fd, buf, strlen(buf)+1);
}