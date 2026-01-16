#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "dummy_http.h"

int main(int argc, char *argv[]) {
	int sockfd, num_bytes;
	struct sockaddr_in serv_addr;
	struct hostent *url;
	char buffer[BUFFER_SIZE] = {0};
	int total_size = 0;
	int http_status = 0;
	int first_read = 1;

	/* Check command line arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
		return 1;
	}

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	/* get the url address */
	url = gethostbyname(argv[1]); /* does not support IP6 */
	if (url == NULL) {
		fprintf(stderr, "ERROR: Host not found\n");
		return 2;
	}

	/* Initialize socket structure (sockarrd_in) */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr, url->h_addr_list[0], url->h_length);
	serv_addr.sin_port = htons(HTTP_PORT);

	/* Connect to the server */
	if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR: Connection failed\n");
		return 3;
	}

	/* Set receive timeout to prevent indefinite blocking */
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		fprintf(stderr, "WARNING: Failed to set socket timeout\n");
	}

	/* Prepare request */
	snprintf(buffer, BUFFER_SIZE, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1]);
	printf("Sending: %s", buffer);
	printf("-------------------------------------------\n\n");

	/* Send message to the server */
	num_bytes = send(sockfd, buffer, strlen(buffer), 0);
	if (num_bytes < 0) {
		fprintf(stderr, "ERROR: Failed writing to socket\n");
		return 4;
	}

	/* Now read server response (may require multiple reads)*/
	do {
		memset(buffer, 0, BUFFER_SIZE);
		num_bytes = recv(sockfd, buffer, BUFFER_SIZE-1, 0);
		if (num_bytes > 0) {
			total_size += num_bytes;
			/* Parse HTTP status from first line of first read */
			if (first_read) {
				char *status_line = buffer;
				if (strncmp(status_line, "HTTP/", 5) == 0) {
					/* Find first space after HTTP/x.x */
					char *space = strchr(status_line, ' ');
					if (space != NULL) {
						http_status = atoi(space + 1);
					}
				}
				first_read = 0;
			}
		}
	} while (num_bytes > 0);

	if (num_bytes < 0) {
		fprintf(stderr, "ERROR: Failed reading from socket\n");
		return 5;
	}
	
	/* Print HTTP status and response size */
	printf("HTTP Status: %d\n", http_status);
	printf("Response Size: %d bytes\n", total_size);
	close(sockfd);
	return 0;
}
