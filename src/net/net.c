/*
#   CNetworking
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

/* History:
 *
 * Wednesday 10 may 2017 - added the possibity to choose between ipv4 or ipv6 (for real)
 * Monday 8 may 2017 - added the possiblity to choose between ipv4 and ipv6 + small changes
 * Tuesday 3 Jan 2017 - net_send_data accepts len as third param, added a bit more doc (1.0.1)
 * Mon 2 Jan 2017 - first real commit (1.0)
 */

#include "net.h"

// non OS idependent functions

const char* net_getVersion() {
    return "1.0.3";
}

const char* net_getSystem() {
#ifdef UNIX
    return "unix";
#endif
#ifdef WINDOWS
    return "windows";
#endif
}

// end non OS idependent functions

#ifdef UNIX

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

static struct {
    bool is_ipv4;
    bool debug;
} moduleData;

bool net_enable_debug_msg(bool enable)
{
    moduleData.debug = enable;
}

//Note: All ports below 1024 are RESERVED (unless you're the superuser)! You can have any port number above that, right up to 65535
int net_init_ipv4(struct sockaddr_in* dest, const char* address, int port) {
    memset(dest, 0, sizeof(&dest));

    moduleData.is_ipv4 = true;
    dest->sin_family = AF_INET;

    if (strcmp(address, "localhost") == 0)
        //INADDR_ANY = local address
        dest->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        dest->sin_addr.s_addr = inet_addr(address);   /* set destination address */

    dest->sin_port = htons(port);

    if (moduleData.debug)
        printf("net.c - net_init - done \n");

    return 1;
}

int net_init_ipv6(struct sockaddr_in6* dest, const char* address, int port) {
    memset(dest, 0, sizeof(&dest));

    moduleData.is_ipv4 = false;
    dest->sin6_family = AF_INET6;

    //char* str[INET6_ADDRSTRLEN];

    if (strcmp(address, "localhost") == 0)
        //INADDR_ANY = local address
        dest->sin6_addr = in6addr_any;
    else
        inet_pton(AF_INET6, address, &(dest->sin6_addr));   /* set destination address */

    dest->sin6_port = htons(port);

    if (moduleData.debug)
        printf("net.c - net_init - done \n");

    return 1;
}

int net_create_socket() {
    int socket_ = 0;

    if (moduleData.is_ipv4)
        socket_ = socket(AF_INET, SOCK_STREAM, 0);
    else
        socket_ = socket(AF_INET6, SOCK_STREAM, 0);

    if (socket_ == -1) {
        printf("%s \n", "Error creating socket!");
        return -1;
    }

    if (moduleData.debug)
        printf("net.c - net_create_socket - done \n");

    return socket_;
}

int net_bind_socket_ipv4(struct sockaddr_in* dest, int socket) {
    int err = bind(socket,(struct sockaddr *)dest, sizeof(struct sockaddr));
    if (err < 0)
    {
        printf("%s \n", "Error binding socket. Perhaps it's already binded");
        net_close_connection(socket);
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_bind_socket - done \n");

    return 1;
}

int net_bind_socket_ipv6(struct sockaddr_in6* dest, int socket) {
    int err = bind(socket,(struct sockaddr *)dest, sizeof(struct sockaddr));
    if (err < 0)
    {
        printf("%s \n", "Error binding socket. Perhaps it's already binded");
        net_close_connection(socket);
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_bind_socket - done \n");

    return 1;
}


/*
 * When connecting to a server the kernel will choose a local port for us.
 * This means you don't have to call net_bind_socket() !
*/
int net_connect_to_ipv4(struct sockaddr_in* dest, int socket) {
    int err = connect(socket, (struct sockaddr *)dest, sizeof(struct sockaddr_in));
    if (err < 0)
    {
        printf("%s \n","Error connecting!");
        net_close_connection(socket);
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_connect_to - done \n");

    return 1;
}

int net_connect_to_ipv6(struct sockaddr_in6* dest, int socket) {
    int err = connect(socket, (struct sockaddr *)dest, sizeof(struct sockaddr_in));
    if (err < 0)
    {
        printf("%s \n","Error connecting!");
        net_close_connection(socket);
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_connect_to - done \n");

    return 1;
}

//NOTE: a write() call made to a socket behaves in exactly the same way as send() with flags set to zero.
int net_send_data(int new_socket, void* data, int len) {
    int err = send(new_socket, data, len, 0);
    if (err < 0)
    {
        printf("%s \n", "Error, could not send data!");
        net_close_connection(new_socket);
        return -1;
    }
    // return the number of bytes sent
    return err;
}

/*
 * TODO look into flags for _recv_
 * If successful, recv() returns the number of bytes received
*/
int net_recieve_data(int socket, void* msg, int len) {
    int n_bytes = recv(socket, msg, len, 0);
    /* Note: A read() call made to a socket behaves in exactly the same way as a recv() with flags set to zero. */
    if (n_bytes < 0) {
        printf("%s \n", "Error, could not recieve data!");
        net_close_connection(socket);
        return -1;
    }
    if (n_bytes == 0) {
        printf("%s \n", "Error, socket is being blocked or connection to the remote note failed!");
        net_close_connection(socket);
        return -1;
    }
    return n_bytes;
}

int net_listen_for_connection(int socket, int max_connection_pending) {
    int err = listen(socket, max_connection_pending);
    if (err < 0) {
        printf("%s \n", "Error listening for connection.Perhaps there are too many pending connections");
        net_close_connection(socket);
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_listen_for_connection - done \n");

    return 1;
}

int net_accept_connection_ipv4(struct sockaddr_in* address, int socket) {
    socklen_t socksize = sizeof(struct sockaddr_in);
    int err = accept(socket, (struct sockaddr*)address, &socksize);
    if (err < 0)
    {
        printf("%s \n", "Error, could not accept data");
        net_close_connection(socket);
        return -1;
    }

    if (moduleData.debug)
        printf("net.c - net_accept_connection - done \n");

    return err;
}

int net_accept_connection_ipv6(struct sockaddr_in6* address, int socket) {
    socklen_t socksize = sizeof(struct sockaddr_in);
    int err = accept(socket, (struct sockaddr*)address, &socksize);
    if (err < 0)
    {
        printf("%s \n", "Error, could not accept data");
        net_close_connection(socket);
        return -1;
    }

    if (moduleData.debug)
        printf("net.c - net_accept_connection - done \n");

    return err;
}

int net_close_connection(int socket) {
    int err = close(socket);
    if (err < 0){
        printf("Error, could not close socket, perhaps the server has been already closed? \n!");
        return -1;
    }
    if (moduleData.debug)
        printf("net.c - net_close - done \n");

    return 1;
}

// Returns the IP address of someone who has connected to you
const char* net_getConnectedIP_ipv4(struct sockaddr_in* dest) {
    return inet_ntoa(dest->sin_addr);
}

const char* net_getConnectedIP_ipv6(struct sockaddr_in6* dest) {
    char* str = (char*)malloc(sizeof(char)*INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(dest->sin6_addr), str, INET6_ADDRSTRLEN);
    return str;
}

#endif // unix
