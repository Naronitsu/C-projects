/*===============================================================
  sockets_notes.c  —  Revision notes: SOCKETS (TCP/UDP, POSIX)
  ===============================================================

  BIG PICTURE
  - A socket is an endpoint for network communication.
  - Two main transport types you’ll use:
      TCP: reliable byte stream (like a pipe, but over a network)
      UDP: datagrams (message packets), can be lost/reordered

  ADDRESSING
  - IP + port:
      IPv4: 32-bit, e.g. 192.168.1.10
      IPv6: 128-bit, e.g. 2001:db8::1
  - Use getaddrinfo() to avoid hardcoding IPv4/IPv6 differences.

  TCP SERVER FLOW (classic)
    getaddrinfo(NULL, "8080", hints=PASSIVE)
    socket()
    setsockopt(SO_REUSEADDR)
    bind()
    listen()
    accept()  -> new connected socket per client
    recv()/send() (or read/write)
    close()

  TCP CLIENT FLOW
    getaddrinfo("example.com","80")
    socket()
    connect()
    send()/recv()
    close()

  UDP FLOW
    socket(SOCK_DGRAM)
    server: bind(); recvfrom(); sendto()
    client: sendto(); recvfrom()
    (connect() is optional in UDP; it just sets default peer)

  KEY CONCEPT: TCP IS A STREAM
  - No message boundaries.
  - If you send 100 bytes, receiver might recv:
      100 bytes, or 60+40, or 1+99, etc.
  - You must design a framing protocol:
      length-prefix, delimiter, fixed-size header, etc.

  COMMON PITFALLS
  - Not handling partial sends/receives.
  - Blocking calls causing hangs (consider timeouts or nonblocking + poll).
  - Not closing client socket after accept-handling.
  - Forgetting network byte order:
      htons/ntohs, htonl/ntohl for integers in protocols.
  - Misusing inet_addr (deprecated-ish); prefer getaddrinfo + inet_ntop.

  DEBUG TRICKS
  - Use perror for syscalls.
  - Print peer address with getnameinfo/inet_ntop.
  - Use netcat (nc) to test TCP servers quickly.

  BUILD
    gcc sockets_notes.c -o sockets_notes

*/

#ifdef __unix__
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int send_all(int fd, const void *buf, size_t n) {
    const unsigned char *p = (const unsigned char *)buf;
    size_t total = 0;
    while (total < n) {
        ssize_t s = send(fd, p + total, n - total, 0);
        if (s < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        total += (size_t)s;
    }
    return 0;
}

/* Minimal TCP client example: connect and send a line */
static int tcp_client_demo(const char *host, const char *port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;   // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    struct addrinfo *res = NULL;
    int rc = getaddrinfo(host, port, &hints, &res);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return -1;
    }

    int sock = -1;
    for (struct addrinfo *p = res; p; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock < 0) continue;
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) break;
        close(sock);
        sock = -1;
    }
    freeaddrinfo(res);

    if (sock < 0) {
        perror("connect/socket");
        return -1;
    }

    const char msg[] = "hello from client\n";
    if (send_all(sock, msg, sizeof msg - 1) < 0) {
        perror("send_all");
        close(sock);
        return -1;
    }

    /* Read something back (may block if server doesn't respond) */
    char buf[256];
    ssize_t r = recv(sock, buf, sizeof buf - 1, 0);
    if (r > 0) {
        buf[r] = '\0';
        printf("received: %s", buf);
    } else if (r == 0) {
        printf("server closed connection\n");
    } else {
        perror("recv");
    }

    close(sock);
    return 0;
}

/* Minimal TCP server example: accept one client, echo back */
static int tcp_server_demo(const char *port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE; // bind to all local addresses

    struct addrinfo *res = NULL;
    int rc = getaddrinfo(NULL, port, &hints, &res);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return -1;
    }

    int listenfd = -1;
    for (struct addrinfo *p = res; p; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) continue;

        int yes = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;

        close(listenfd);
        listenfd = -1;
    }
    freeaddrinfo(res);

    if (listenfd < 0) {
        perror("bind/socket");
        return -1;
    }

    if (listen(listenfd, 16) < 0) {
        perror("listen");
        close(listenfd);
        return -1;
    }

    printf("listening on port %s...\n", port);

    struct sockaddr_storage peer;
    socklen_t peerlen = sizeof peer;
    int clientfd = accept(listenfd, (struct sockaddr *)&peer, &peerlen);
    if (clientfd < 0) {
        perror("accept");
        close(listenfd);
        return -1;
    }

    char host[NI_MAXHOST], serv[NI_MAXSERV];
    if (getnameinfo((struct sockaddr *)&peer, peerlen,
                    host, sizeof host, serv, sizeof serv,
                    NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
        printf("client connected from %s:%s\n", host, serv);
    }

    char buf[512];
    ssize_t r = recv(clientfd, buf, sizeof buf, 0);
    if (r > 0) {
        /* Echo back exactly what we received */
        if (send_all(clientfd, buf, (size_t)r) < 0) {
            perror("send_all");
        }
    }

    close(clientfd);
    close(listenfd);
    return 0;
}

int main(int argc, char **argv) {
    /* Usage:
         ./sockets_notes server 8080
         ./sockets_notes client example.com 8080
    */
    if (argc >= 3 && strcmp(argv[1], "server") == 0) {
        return tcp_server_demo(argv[2]) == 0 ? 0 : 1;
    }
    if (argc >= 4 && strcmp(argv[1], "client") == 0) {
        return tcp_client_demo(argv[2], argv[3]) == 0 ? 0 : 1;
    }

    fprintf(stderr, "usage:\n  %s server <port>\n  %s client <host> <port>\n", argv[0], argv[0]);
    return 1;
}

#else
#include <stdio.h>
int main(void) {
    puts("These sockets notes target POSIX (Linux/macOS).");
    return 0;
}
#endif
