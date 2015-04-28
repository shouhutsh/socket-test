#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "tools.h"

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
    fd_set rest, allset;
    char buf[MAXLINE];
    int i, n, maxfd, maxi, nready, client[FD_SETSIZE];

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, BACKLOG);

    maxi = -1;
    maxfd = listenfd;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    for(i = 0; i < FD_SETSIZE; ++i){
        client[i] = -1;
    }
	while(1){
        rest = allset;
        nready = Select(maxfd+1, &rest, NULL, NULL, NULL);

        if(FD_ISSET(listenfd, &rest)){
            clilen = sizeof(cliaddr);
            connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
            for(i = 0; i < FD_SETSIZE; ++i){
                if(-1 == client[i]){
                    client[i] = connfd;
                    break;
                }
            }
            if(FD_SETSIZE == i){
                err_sys("too many client");
            }
            FD_SET(connfd, &allset);
            maxfd = max(maxfd, connfd);
            maxi = max(maxi, i);
            if(--nready <= 0){
                continue;
            }
        }

        for(i = 0; i < maxi; ++i){
            if(-1 == (connfd = client[i])) continue;
            if(FD_ISSET(connfd, &rest)){
                if(0 == (n = read(connfd, buf, MAXLINE))){
                    FD_CLR(connfd, &allset);
                    client[i] = -1;
                    close(connfd);
                }else{
                    Writen(connfd, buf, n);
                }
            }
            if(--nready <= 0){
                break;
            }
        }
    }
    return 0;
}
