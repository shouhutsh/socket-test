#include <stdio.h>
#include <time.h>
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

void
disp_socket(struct sockaddr_in *sockaddr, int len){
    char addr[200];
    inet_ntop(sockaddr->sin_family, &sockaddr->sin_addr, addr, sizeof(addr));
    printf("Connect Socket = Len: %d, AF: %d, Addr: %s, Port: %d\n", len, sockaddr->sin_family, addr, ntohs(sockaddr->sin_port));
}

int
main(void)
{
    time_t tm;
    int servfd, connfd;
    char buf[MAXLINE+1];
    struct sockaddr_in addr, conn;
    int len = sizeof(conn);

    servfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);

    Bind(servfd, (SA *)&addr, sizeof(addr));
    show_socket(servfd);
    Listen(servfd, BACKLOG);

    while(1){
        connfd = Accept(servfd, (SA *)&conn, &len);
        if(fork() == 0){
            close(servfd);
            //            show_socket(connfd);
            disp_socket(&conn, len);
            tm = time(NULL);
            snprintf(buf, MAXLINE, "%.24s\r\n", ctime(&tm));
            Writen(connfd, buf, strlen(buf));
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
    close(servfd);
    return 0;
}
