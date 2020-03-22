#include "https_server.h"







std::string url_decode (std::string const & url)
{
    std::string ans = "";
    
    for (size_t i = 0; i < url.size(); ++i)
    {
        if (url[i] != '%')
        {
            if (url[i] != '+')
            {
                ans += url[i];
            }
            else
            {
                ans += ' ';
            }
        }
        else
        {
            int v;
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &v);
            ans += static_cast <char> (v);
            i += 2;
        }
    }
    
    return ans;
}






https_server::https_server 
(
    std::atomic <bool> const & _stop, 
    std::string const & port
) :
    stop(_stop)
{
    mg_mgr_init(&mgr, NULL);
    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.user_data = this;
    bind_opts.ssl_cert = s_ssl_cert;
    bind_opts.ssl_key = s_ssl_key;
    bind_opts.error_string = &err;

    nc = mg_bind_opt(&mgr, port.c_str(), ev_handler, this, bind_opts);
    if (nc == NULL) {
        throw std::runtime_error("can't bind");
    }

    // Set up HTTP server parameters
    mg_set_protocol_http_websocket(nc);
    //mg_mgr_poll(&mgr, 100);
}

void https_server::ev_handler (struct mg_connection *conn, int ev, void * en_data, void * data) 
{
    if (ev == MG_EV_HTTP_REQUEST) 
    {
        reinterpret_cast <https_server *> (conn->user_data) -> handler(conn, ev, en_data, data);
    }
}

void https_server::handler (struct mg_connection *conn, int ev, void *, void * data)
{
    if (ev == MG_EV_HTTP_REQUEST) 
    {
        struct mbuf *io = &conn->recv_mbuf;
        std::string request(io->buf, io->len);

        httpparser::Request parsed_request;
        httpparser::HttpRequestParser parser;
        httpparser::HttpRequestParser::ParseResult res = parser.parse(parsed_request, request.data(), request.data() + request.size());
        if (res == httpparser::HttpRequestParser::ParsingCompleted)
        {
            std::string response_value = "error";
            
            try
            {
                response_value = handle(parsed_request);
            }
            catch (...)
            {}
            //std::cout << response_value << "\n";
            //std::cout << request << "\n";
            
            mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                            "Server: MyWebServer\r\n"
                            "Content-Type: text/plain; charset=utf-8\r\n"
                            "Content-Length: %lu\r\n"
                            "Content-Language: %s\r\n"
                            "Connection: close\r\n"
                            "\r\n", response_value.size(), std::locale("").name().c_str());
            mg_send
            (
                conn, 
                response_value.c_str(), 
                response_value.size()
            );
        }

        mbuf_remove(io, io->len);
        conn->flags |= MG_F_SEND_AND_CLOSE;
    }
}

void https_server::main () 
{
    for (; !stop.load();) {
        mg_mgr_poll(&mgr, 100);
    }
}

https_server::~https_server ()
{
    mg_mgr_free(&mgr);
}

const char * https_server ::s_ssl_cert = "server/server.pem";
const char * https_server ::s_ssl_key = "server/server.key";

void start_server (https_server & server)
{
    server.main();
}

