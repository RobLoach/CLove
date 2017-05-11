/*
#   CNetworking
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifndef NET_H_
#define NET_H_

#ifdef _WIN32
    #define WINDOWS
#endif 
#ifdef _WIN64
    #ifndef WINDOWS 
        #define WINDOWS
    #endif
#endif

#ifdef __APPLE__ 
    #define UNIX
#endif 

#ifdef __linux__ 
    #define UNIX
#endif

#ifdef UNIX 

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int net_init_ipv4(struct sockaddr_in* dest, const char* address, int port);
int net_init_ipv6(struct sockaddr_in6* dest, const char *address, int port);
bool net_enable_debug_msg(bool enable);
int net_create_socket();
int net_bind_socket_ipv4(struct sockaddr_in* dest, int socket);
int net_connect_to_ipv4(struct sockaddr_in* dest, int socket);
int net_bind_socket_ipv6(struct sockaddr_in6* dest, int socket);
int net_connect_to_ipv6(struct sockaddr_in6* dest, int socket);
int net_send_data(int new_socket, void* data, int len);
int net_recieve_data(int socket, void* msg, int len);
int net_listen_for_connection(int socket, int max_connection_pending);
int net_accept_connection_ipv4(struct sockaddr_in* address, int socket);
int net_accept_connection_ipv6(struct sockaddr_in6* address, int socket);
int net_close_connection(int socket);

const char* net_getConnectedIP_ipv4(struct sockaddr_in* dest);
const char* net_getConnectedIP_ipv6(struct sockaddr_in6* dest);

#endif // unix

//Non OS related functions

const char* net_getSystem();
const char* net_getVersion();

#endif // net.h
