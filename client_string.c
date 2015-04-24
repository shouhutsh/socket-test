#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "tools.h"

void
str_cli(int fd){
    char line[MAXLINE+1];
    while(Fgets(line, MAXLINE, stdin) != NULL){
        Writen(fd, line, strlen(line));
        if(Readline(fd, line, MAXLINE) == 0){
            perror("readline error\n");
            exit(1);
        }
        Fputs(line, stdout);
    }
}

int
main(int argc, char *argv[])
{
    if(3 != argc){
        printf("Usage: %s <IP addr> <port>\n", argv[0]);
        exit(0);
    }

    int fd;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));

    inet_pton(AF_INET, (SA *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));

    fd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(fd, (SA *) &addr, sizeof(addr));

    str_cli(fd);

    close(fd);
    return 0;
}
