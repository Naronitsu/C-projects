#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT 11111    /* the port client will be connecting to */
#define MSG "Hi i am a client\n"
#define MAX_DATA_SIZE 25 /* max number of bytes we can get at once */

int main(){
    int sockfd, numbytes;
    char buf[MAX_DATA_SIZE] = {0};
    struct sockaddr_in srv_addr = {0};

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("connect");
        return -2;
    }

    printf("Entering chat. Type your messages below, type QUIT to exit:\n");
    scanf("%s", buf);  

    while (strcmp(buf, "QUIT") != 0) {
        if (send(sockfd, buf, strlen(buf), 0) == -1){
            perror("send");
            return -3;
        }

        printf("Data sent: %s\n", buf);

        numbytes = recv(sockfd, buf, MAX_DATA_SIZE, 0);
        if (numbytes == -1) {
            perror("recv");
            return -4;
        }

        buf[numbytes] = '\0';
        printf("Received: %s\n", buf);

        scanf("%s", buf);  
    }
    
}