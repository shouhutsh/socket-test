#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tools.h"

int
main(void)
{
    time_t tm;
    int servfd, connfd;
    char buf[MAXLINE+1];
    struct sockaddr_in addr;

    servfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);

    Bind(servfd, (SA *)&addr, sizeof(addr));
    Listen(servfd, BACKLOG);

    while(1){
        connfd = Accept(servfd, NULL, NULL);
        tm = time(NULL);
        snprintf(buf, MAXLINE, "%.24s\r\n", ctime(&tm));
        write(connfd, buf, strlen(buf));
        close(connfd);
    }
    return 0;
}
