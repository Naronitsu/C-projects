#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT 11111

int main() {
    int listenfd, connfd;
    struct sockaddr_in srv_addr = {0};
    char buf[128];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    listen(listenfd, 5);

    printf("Server listening on port %d...\n", PORT);

    connfd = accept(listenfd, NULL, NULL);

    int n = recv(connfd, buf, sizeof(buf) - 1, 0);
    buf[n] = '\0';
    printf("Server received: %s", buf);

    send(connfd, "Hello from server\n", 18, 0);
    //sleep(60); // Keep the connection open for a while
	printf("Press any key to exit...");// pause before closing
    getchar();
    close(connfd);
    close(listenfd);
    printf("Server exiting...\n");
    return 0;
}
