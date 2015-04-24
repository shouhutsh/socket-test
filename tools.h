#ifndef _TOOLS_H
#define _TOOLS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define BACKLOG 5
#define MAXLINE 4096
#define SA struct sockaddr

#define SERV_PORT 12345

void
err_sys(const char * msg){
    perror(msg);
    exit(1);
}

int
Socket(int domain, int type, int protocol){
    int fd;
    if(0 > (fd = socket(domain, type, protocol))){
        err_sys("socket error");
    }
    return fd;
}

void
Inet_pton(int af, const char *src, void *dst){
    if(0 > inet_pton(af, src, dst)){
        err_sys("inet_pton error");
    }
}

char *
Fgets(char *s, int size, FILE *stream){
    char * ptr;
    if(NULL == (ptr = fgets(s, size, stream)) && ferror(stream)){
        err_sys("fgets error");
    }
    return ptr;
}

void
Fputs(const char *s, FILE *stream){
    if(EOF == fputs(s, stream)){
        err_sys("fputs error");
    }
}

void
Connect(int sockfd, const SA * addr, socklen_t addrlen){
    if(0 > connect(sockfd, addr, addrlen)){
        err_sys("connect error");
    }
}

void
Bind(int sockfd, const SA * addr, socklen_t addrlen){
    if(0 > bind(sockfd, addr, addrlen)){
        err_sys("bind error");
    }
}

void
Listen(int sockfd, int backlog){
    if(0 > listen(sockfd, backlog)){
        err_sys("listen error");
    }
}

int
Accept(int fd, SA * sa, socklen_t *salenptr){
    int afd;
    if(0 > (afd = accept(fd, sa, salenptr))){
        err_sys("accept error");
    }
    return afd;
}

int
readn(int fd, char *buf, size_t nbyte){
    if(NULL == buf) return 0;

    char *ptr = buf;
    int n, count = nbyte;
    while(count > 0){
        if((n = read(fd, ptr, count)) < 0){
            if(EINTR == errno){
                n = 0;
            }else{
                return -1;
            }
        }else if(0 == n){
            break;
        }
        count -= n;
        ptr += n;
    }
    return nbyte - count;
}

int
Readn(int fd, char *buf, size_t nbyte){
    int n;
    if((n = readn(fd, buf, nbyte)) < 0){
        err_sys("readn error");
    }
    return n;
}

int
writen(int fd, const char *buf, size_t nbyte){
    if(NULL == buf) return 0;

    const char *ptr = buf;
    int n, count = nbyte;
    while(count > 0){
        if((n = write(fd, ptr, count)) <= 0){
            if(0 > n && EINTR == errno){
                n = 0;
            }else{
                return -1;
            }
        }
        count -= n;
        ptr += n;
    }
    return nbyte;
}

void
Writen(int fd, const char *buf, size_t nbyte){
    if(writen(fd, buf, nbyte) != nbyte){
        err_sys("writen error");
    }
}

int
Fork(){
    int pid;
    if((pid = fork()) < 0){
        err_sys("fork error");
    }
    return pid;
}


static int buf_length = 0;
static char	*read_ptr;
static char	read_buf[MAXLINE];

int
readch(int fd, char *ch)
{
    if(buf_length <= 0){
    again:
        if((buf_length = read(fd, read_buf, MAXLINE)) > 0){
            read_ptr = read_buf;
        }else if(0 == buf_length){
            return 0;
        }else if(EINTR == errno){
            goto again;
        }else{
            return -1;
        }
    }
    --buf_length;
    *ch = *(read_ptr++);
    return 1;
}

int
readline(int fd, void *str, size_t maxlen)
{
    int i, n;
    char c, * ptr = str;
    for(i = 1; i < maxlen; ++i){
        if(1 == (n = readch(fd, &c))){
            *ptr++ = c;
            if('\n' == c) break;
        }else if(0 == n){
            *ptr++ = 0;
            return i-1;
        }else if(-1 == n){
            err_sys("readline error");
        }
    }
    *ptr++ = 0;
    return i;
}

long
readlinebuf(void **vptrptr)
{
	if(buf_length)
		*vptrptr = read_ptr;
	return buf_length;
}

long
Readline(int fd, void *ptr, size_t maxlen)
{
	long n;
	if ((n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}

#endif