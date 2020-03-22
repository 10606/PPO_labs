#ifndef HTTPS_SERVER
#define HTTPS_SERVER

/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * This example starts an SSL web server on https://localhost:8443/
 *
 * Please note that the certificate used is a self-signed one and will not be
 * recognised as valid. You should expect an SSL error and will need to
 * explicitly allow the browser to proceed.
 */

#define MG_ENABLE_SSL true
#define MG_ENABLE_CALLBACK_USERDATA true
#include <atomic>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <utility>
#include "mongoose.h"
#include "../processing/processing.h"
#include "../httpparser/src/httpparser/request.h"
#include "../httpparser/src/httpparser/httprequestparser.h"

std::string url_decode (std::string const & url);

struct https_server
{
    https_server 
    (
        std::atomic <bool> const & _stop, 
        std::string const & port
    );

    static const char *s_ssl_cert;
    static const char *s_ssl_key;

    static void ev_handler (struct mg_connection *conn, int ev, void * en_data, void * data);
    void handler (struct mg_connection *conn, int ev, void *, void * data);
    virtual std::string handle (httpparser::Request const & request) noexcept = 0;
    void main (); 
    virtual ~https_server ();
    
private:
    std::atomic <bool> const & stop;

    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
    const char *err;
};

void start_server (https_server & server);

template <typename T>
struct server_starter 
{
    server_starter 
    (
        std::string port
    ) : 
        stop_server(0),
        server
        (
            stop_server, 
            port
        ),
        t_server(start_server, std::ref(server))
    {}

    ~server_starter ()
    {
        stop_server.store(1);
        t_server.join();
    }

private:
    std::atomic <bool> stop_server;
    T server;
    std::thread t_server;
};



#endif
