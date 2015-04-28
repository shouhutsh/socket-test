#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "tools.h"

void
sig_chld(int signo){
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0){
        printf("(%d) child %d terminated.\n", stat, pid);
    }
    return;
}

void
str_echo(int sockfd){
	long n, x, y;
	char buf[MAXLINE];

again:
	while ((n = read(sockfd, buf, MAXLINE)) > 0){
		Writen(sockfd, buf, n);
    }

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo error");
}

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

    signal(SIGCHLD, sig_chld);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, BACKLOG);

	while(1){
		clilen = sizeof(cliaddr);
		if(0 > (connfd = accept(listenfd, (SA *) &cliaddr, &clilen))){
            if(EINTR == errno){
                continue;
            }else{
                printf("accept error");
            }
        }

		if ((childpid = Fork()) == 0) {
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
    close(listenfd);
    return 0;
}
