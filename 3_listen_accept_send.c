#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h> //Close the socket

#define MYPORT "8000" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    // Retrieve current machine address information
    getaddrinfo(NULL, MYPORT, &hints, &res);

    // make a socket, bind it, and listen on it:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    listen(sockfd, BACKLOG);

    // now accept an incoming connection:
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    // Try to telnet 127.0.0.1 MYPORT
    if(new_fd==-1) {
        printf("Oh dear, something went wrong! %s\n", strerror(errno));
    }

    // I send a message to the client connected
    char *msg = "Hey its me!\n";
    int len, bytes_sent;
    len = strlen(msg);
    bytes_sent = send(new_fd, msg, len, 0);
    if(bytes_sent==-1) {
        printf("Oh dear, something went wrong! %s\n", strerror(errno));
    }

    close(sockfd);
    close(new_fd);

    return 0;
}
