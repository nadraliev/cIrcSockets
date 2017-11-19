#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *host = "127.0.0.1";
    int port = 6667;

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int size = sizeof(struct sockaddr_in);

    if (connect(sock, (struct sockaddr *)&addr, size) < 0) {
        perror("connect");
        exit(2);
    }

    char *message = "USER testSocket * * :realName\n";
    char buf[2048];
    int received = 1;

    send(sock, message, strlen(message), 0);
    message = "NICK testSocket\n";
    send(sock, message, strlen(message), 0);
    sleep(5);
    message = "JOIN #test\n";
    send(sock, message, strlen(message), 0);
    sleep(5);
    message = "privmsg #test testMessage!\n";
    send(sock, message, strlen(message), 0);

    while (received > 0) {
        received = recv(sock, buf, sizeof(buf), 0);
        buf[received] = '\0';
        printf(buf);
    }



    close(sock);

    return 0;
}