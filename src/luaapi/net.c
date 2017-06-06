/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "net.h"

#include "../net/net.h"

#include "tools.h"

//TODO
//Check if networking works with EMCC
//Add compatibility with Windows -_-*

#ifdef CLOVE_DESKTOP

static struct
{
    struct sockaddr_in server_ipv4;
    struct sockaddr_in6 server_ipv6;
    int socket;
    bool is_ipv4;
} moduleData;

/*
 * This function has to check if the developer has created a
 * socket before he tries to use it.
 */
static void l_net_check_socket(int socket, const char* in_what_function)
{
    if (socket == 0 || socket < 0)
        printf("%s %s \n", "Be sure you have created the socket before using it in: ", in_what_function);
}

static int l_net_createSocket(lua_State* state)
{
    moduleData.socket = net_create_socket();
    lua_pushinteger(state, moduleData.socket);

    return 1;
}

static int l_net_load(lua_State* state)
{
    const char* address = l_tools_toStringOrError(state, 1);
    int port = l_tools_toIntegerOrError(state, 2);
    const char* ip_version = l_tools_toStringOrError(state, 3);
    bool enable_debug = l_tools_optBoolean(state, 4, false);

    net_enable_debug_msg(enable_debug);

    moduleData.is_ipv4 = true;

    if (strcmp(ip_version, "ipv6") == 0)
    {
        moduleData.is_ipv4 = false;
        net_init_ipv6(&moduleData.server_ipv6, address, port);
    }
    else
    {
        moduleData.is_ipv4 = true;
        net_init_ipv4(&moduleData.server_ipv4, address, port);
    }

    return 0;
}

static int l_net_connect(lua_State* state)
{
    if (moduleData.is_ipv4)
        net_connect_to_ipv4(&moduleData.server_ipv4, moduleData.socket);
    else
        net_connect_to_ipv6(&moduleData.server_ipv6, moduleData.socket);

    return 0;
}

static int l_net_recieve(lua_State* state)
{
    char* buffer = lua_tostring(state, 1);
    int len = lua_tointeger(state, 2);

    l_net_check_socket(moduleData.socket, "recieve");

    int len_recieved = net_recieve_data(moduleData.socket, buffer, len);

    lua_pushinteger(state, len_recieved);

    return 1;
}

static int l_net_bind_socket(lua_State* state)
{
    int err = 0;

    l_net_check_socket(moduleData.socket, "bind");

    if (moduleData.is_ipv4)
        err = net_bind_socket_ipv4(&moduleData.server_ipv4, moduleData.socket);
    else
        err = net_bind_socket_ipv6(&moduleData.server_ipv6, moduleData.socket);

    if (err < 0)
        l_tools_trowError(state, "Error, could not bind socket");

    return 0;
}

static int l_net_listen(lua_State* state)
{

    int max_pending = l_tools_toIntegerOrError(state, 1);

    l_net_check_socket(moduleData.socket, "listen");

    net_listen_for_connection(moduleData.socket, max_pending);

    return 0;
}

static int l_net_accept(lua_State* state)
{
    struct sockaddr_in dest_ipv4;
    struct sockaddr_in6 dest_ipv6;

    l_net_check_socket(moduleData.socket, "recieve");

    if (moduleData.is_ipv4)
    {
        int temp_socket = net_accept_connection_ipv4(&dest_ipv4, moduleData.socket);
        lua_pushinteger(state, temp_socket);
    } else {
        int temp_socket = net_accept_connection_ipv6(&dest_ipv6, moduleData.socket);
        lua_pushinteger(state, temp_socket);
    }

    return 1;
}

static int l_net_send(lua_State* state)
{
    int temp_socket = l_tools_toIntegerOrError(state, 1);
    const char* data = l_tools_toStringOrError(state, 2);
    int len  = l_tools_toIntegerOrError(state, 3);

    int size = net_send_data(temp_socket, (char*)data, len);

    lua_pushinteger(state, size);

    return 1;
}

static int l_net_close(lua_State* state)
{

    l_net_check_socket(moduleData.socket, "close");
    net_close_connection(moduleData.socket);

    return 0;
}

static luaL_Reg const regFuncs[] = {
    {"load",         l_net_load},
    {"send",         l_net_send},
    {"connect",      l_net_connect},
    {"accept",       l_net_accept},
    {"listen",       l_net_listen},
    {"bind",         l_net_bind_socket},
    {"recieve",      l_net_recieve},
    {"createSocket", l_net_createSocket},
    {"close",        l_net_close},
    {NULL,NULL}
};

int l_net_register(lua_State* state) {
    l_tools_registerModule(state, "net", regFuncs);
    return 0;
}

#endif

#ifdef CLOVE_WEB
int l_net_register(lua_State* state) {
    return 0;
}
#endif


