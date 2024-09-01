#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>  // For inet_ntoa
#include <asm-generic/socket.h>

#define PORT 9092
/**
 * The function createSocket creates a socket with the specified domain, type, and protocol, and
 * returns the socket file descriptor.
 *
 * @param domain The `domain` parameter in the `createSocket` function specifies the communication
 * domain in which the socket will be created. It defines the communication protocol family to be used,
 * such as PF_INET for IPv4 communication or PF_INET6 for IPv6 communication.
 * @param type The `type` parameter in the `createSocket` function specifies the communication
 * semantics to be used by the socket. It can have values such as `SOCK_STREAM` for a stream socket
 * (TCP) or `SOCK_DGRAM` for a datagram socket (UDP).
 * @param protocol The `protocol` parameter in the `createSocket` function represents the protocol to
 * be used with the socket. It specifies a particular protocol to be used with the socket, such as TCP
 * (Transmission Control Protocol) or UDP (User Datagram Protocol). The protocol parameter can be set
 * to 0 to automatically
 *
 * @return The function `createSocket` returns an integer value, which is the socket file descriptor.
 * If the socket creation is successful, it returns the socket file descriptor. If there is an error
 * during socket creation, it returns -1.
 */
int createSocket(int domain, int type, int protocol)
{
    int sockfd = socket(domain, type, protocol);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return -1;
    }
    printf("Socket created successfully!\n");
    return sockfd;
}

/**
 * The function `bindSocket` binds a socket to a specified port with the option to reuse address and
 * port.
 *
 * @param socketfd The `socketfd` parameter in the `bindSocket` function is the file descriptor of the
 * socket that you want to bind to a specific port. This file descriptor is obtained when you create a
 * socket using the `socket` system call. It uniquely identifies the socket in the operating system.
 * @param port The `port` parameter in the `bindSocket` function is an unsigned short integer that
 * represents the port number to which the socket will be bound. This port number is used to uniquely
 * identify the network endpoint of the socket on the host machine. It is essential for communication
 * between different network entities.
 */
void bindSocket(int socketfd, unsigned short port) {
    int opt = 1; // This is the variable to reuse address and port

    // Set socket options to reuse address and port
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_add;
    memset(&server_add, 0, sizeof(server_add)); // Clear structure
    server_add.sin_family = PF_INET;            // IPv4
    server_add.sin_port = htons(port);          // Set port
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);    // Bind to any available interface

    // Bind the socket to the specified port
    int socket_bind = bind(socketfd, (struct sockaddr*)&server_add, sizeof(server_add));

    if (socket_bind < 0) {
        perror("Error binding the socker");
        close(socketfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket bound successfully to port %d\n", port);
}



int main(int argc, char const* argv[])
{
    // Create a socket
    int sockfd = createSocket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Failed to create socket. Exiting...\n");
        return EXIT_FAILURE;
    }

    // Bind the socket to port 8080
    bindSocket(sockfd, PORT);

    // listen socket
    if (listen(sockfd, 3) < 0)
    {
        perror("Error listening socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Socket listening...\n");

    // Acept connections!
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    int new_socket;

    if ((new_socket = accept(sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) < 0)
    {
        perror("Error accepting connection");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    // Clean up: Close the socket
    close(sockfd);
    close(new_socket);

    return EXIT_SUCCESS;
}
