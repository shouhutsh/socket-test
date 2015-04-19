#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SOCK_ADDR "127.0.0.1"
#define SOCK_PORT 12345
#define BACKLOG 5

char buf[BUFSIZ];

void
read_sock(int);

int
main(void)
{
    int fd, confd;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SOCK_ADDR);
    server.sin_port = htons(SOCK_PORT);

    if(-1 == (fd = socket(AF_INET, SOCK_STREAM, 0))){
        exit(-1);
    }
    if(-1 == (bind(fd, (struct sockaddr*)&server, sizeof(server)))){
        exit(-1);
    }
    if(-1 == (listen(fd, BACKLOG))){
        exit(-1);
    }

    while(1){
        confd = accept(fd, NULL, NULL);
        read_sock(confd);
        close(confd);
    }
    return 0;
}

void
read_sock(int confd)
{
    memset(buf, 0, sizeof(buf));

    if(read(confd, buf, sizeof(buf))){
        printf("%s\n", buf);
    }
}