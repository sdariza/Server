#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Include this header for inet_ntoa
#include <unistd.h>     // for close()
#include <stdlib.h>     // for EXIT_FAILURE y EXIT_SUCCESS

int createSocket(int domain, int type, int protocol)
{
    int sockfd;
    sockfd = socket(domain, type, protocol);
    if (sockfd < 0)
    {
        switch (errno)
        {
        case EAFNOSUPPORT:
            printf("The implementation does not support the specified address family.\n");
            break;
        case EMFILE:
            printf("No more file descriptors are available for this process.\n");
            break;
        case ENFILE:
            printf("No more file descriptors are available for the system.\n");
            break;
        case EPROTONOSUPPORT:
            printf("The protocol is not supported by the address family, or the protocol is not supported by the implementation.\n");
            break;
        case EPROTOTYPE:
            printf("The socket type is not supported by the protocol.\n");
            break;
        case EACCES:
            printf("The process does not have appropriate privileges.\n");
            break;
        case ENOBUFS:
            printf("Insufficient resources were available in the system to perform the operation.\n");
            break;
        case ENOMEM:
            printf("Insufficient memory was available to fulfill the request\n");
            break;
        case ENOSR:
            printf("There were insufficient STREAMS resources available for the operation to complete.\n");
            break;
        default:
            fprintf(stderr, "Unknown error occurred when trying to create the socket: %s\n", strerror(errno));
            break;
        }
        return -1;
    }
    else {
        printf("Socket created successfully!\n");
        return sockfd;
    }
}

void bindSocket(int socketfd, unsigned short port) {
    struct sockaddr_in server_add;
    memset(&server_add, 0, sizeof(server_add)); // Clean structure
    server_add.sin_family = AF_INET;  // Correct family of addresses
    server_add.sin_port = htons(port);
    server_add.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketfd, (struct sockaddr*)&server_add, sizeof(server_add)) < 0) {
        fprintf(stderr, "Error binding socket: %s\n", strerror(errno));
        close(socketfd);
        exit(EXIT_FAILURE);
    }
    printf("Socket bound successfully to port %d\n", port);
}


int main(int argc, char const* argv[])
{
    int sockfd = createSocket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return -1;

    bindSocket(sockfd, 8080);

    close(sockfd);  // Clean up (though it never reaches here in this example)
    return 0;
}
