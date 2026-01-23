#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PORT 11111

int main() {
    int listenfd, connfd;
    struct sockaddr_in srv_addr = {0};
    char buf[128] = {0};

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srv_addr.sin_port = htons(PORT);

    int p = bind(listenfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    int l = listen(listenfd, 5);

    printf("Server listening on port %d...\n", PORT);

    connfd = accept(listenfd, NULL, NULL);

    while (1) {
        int n = recv(connfd, buf, sizeof(buf) - 1, 0);
        if (n < 0) {
            printf("recv() failed\n");
            break;
        }
        if (n == 0) {
            printf("Client disconnected\n");
            break;
        }

        buf[n] = '\0';

        if (strcmp(buf, "QUIT") == 0) {
            break;
        }

        for (int i = 0; i < n; i++) {
            buf[i] = (char)toupper((unsigned char)buf[i]);
        }

        printf("%s\n", buf);

        send(connfd, "Message received.\n", 18, 0);
    }
    close(connfd);
    close(listenfd);
    printf("Server exiting...\n");
    return 0;
}
