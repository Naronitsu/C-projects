#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Function to handle server console input
void *handle_console_input(void *arg) {
    char input[BUFFER_SIZE];
    
    while (1) {
        printf("(server) > ");
        fflush(stdout);
        
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            continue;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcasecmp(input, "quit") == 0) {
            printf("Shutting down server...\n");
            exit(EXIT_SUCCESS);
        }
    }
    
    pthread_exit(NULL);
}

// Function to handle each client connection
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_read;
    
    printf("New client connected (socket: %d)\n", client_socket);
    
    // Send welcome message
    const char *welcome = "Connected to Time Server. Commands: 'time' or 'quit'\n";
    send(client_socket, welcome, strlen(welcome), 0);
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_read <= 0) {
            printf("Client disconnected (socket: %d)\n", client_socket);
            break;
        }
        
        // Remove newline character if present
        buffer[strcspn(buffer, "\r\n")] = 0;
        
        printf("Received from client %d: %s\n", client_socket, buffer);
        
        if (strcasecmp(buffer, "quit") == 0) {
            const char *goodbye = "Goodbye!\n";
            send(client_socket, goodbye, strlen(goodbye), 0);
            printf("Client %d requested to quit\n", client_socket);
            break;
            // Get current time
            time_t current_time;
            struct tm *time_info;
            
            time(&current_time);
            time_info = localtime(&current_time);
            
            strftime(response, BUFFER_SIZE, "Current time: %Y-%m-%d %H:%M:%S\n", time_info);
            send(client_socket, response, strlen(response), 0);
        } else {
            const char *error_msg = "Unknown command. Use 'time' or 'quit'\n";
            send(client_socket, error_msg, strlen(error_msg), 0);
        }
    }
    
    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, *client_socket_ptr;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    printf("Time Server started on port %d\n", PORT);
    printf("Waiting for client connections...\n");
    printf("Type 'quit' to shutdown the server\n\n");
    
    // Create a thread to handle console input
    pthread_t console_thread;
    if (pthread_create(&console_thread, NULL, handle_console_input, NULL) != 0) {
        perror("Failed to create console thread");
    }
    pthread_detach(console_thread);
    
    while (1) {
        client_socket_ptr = malloc(sizeof(int));
        if (client_socket_ptr == NULL) {
            perror("Memory allocation failed");
            continue;
        }
        
        *client_socket_ptr = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        
        if (*client_socket_ptr < 0) {
            perror("Accept failed");
            free(client_socket_ptr);
            continue;
        }
        
        printf("Connection accepted from %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));
        
        // Create a new thread for each client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_socket_ptr) != 0) {
            perror("Thread creation failed");
            close(*client_socket_ptr);
            free(client_socket_ptr);
            continue;
        }
        
        // Detach the thread so it cleans up automatically
        pthread_detach(thread_id);
    }
    
    close(server_socket);
    return 0;
}
