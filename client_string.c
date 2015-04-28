#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "tools.h"

#define max(x, y) ((x) > (y) ? (x) : (y))

void
sig_pipe(int signo){
    printf("sig pipe error\n");
    return;
}

void
str_cli(FILE *file, int fd){
    fd_set res;
    int nfds, in_close;
    char line[MAXLINE+1];

    FD_ZERO(&res);
    in_close = 0;
    while(1){
        FD_SET(fileno(file), &res);
        FD_SET(fd, &res);
        nfds = max(fileno(file), fd) + 1;
        select(nfds, &res, NULL, NULL, NULL);

        if(FD_ISSET(fd, &res)){
            if(Readline(fd, line, MAXLINE) == 0){
                if(1 == in_close){
                    return ;
                }
                perror("readline error\n");
                exit(1);
            }
            Fputs(line, stdout);
        }
        if(FD_ISSET(fileno(file), &res)){
            if(Fgets(line, MAXLINE, file) == NULL){
                in_close = 1;
                shutdown(fileno(file), SHUT_RD);
                FD_CLR(fileno(file), &res);
                continue;
            }
            Writen(fd, line, strlen(line));
        }

    }
}

int
main(int argc, char *argv[])
{
    if(3 != argc){
        printf("Usage: %s <IP addr> <port>\n", argv[0]);
        exit(0);
    }

    int i, fd[5];
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    signal(SIGPIPE, sig_pipe);

    for(i = 0; i < 5; ++i){
        Inet_pton(AF_INET, (SA *) &addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(argv[2]));

        fd[i] = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(fd[i], (SA *) &addr, sizeof(addr));
    }
    str_cli(stdin, fd[0]);
    return 0;
}
