/*
Compile command:

clang++ -std=c++17 -o websocket websocket.cpp \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lboost_system -lpthread

*/

#include "boost/beast/core.hpp"
#include "boost/beast/websocket.hpp"
#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "cstdlib"

#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using namespace std;


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>



constexpr const char* WS_HOST = "tskopen.com";
constexpr const char* WS_PORT = "8080";
constexpr const char* WS_PATH = "/js";

std::string host = WS_HOST;
auto const port = WS_PORT;

int main(int argc, char** argv)
{
    try
    {
        if(argc != 4)
        {
            cout <<
                "Usage: websocket-client-sync <host> <port> <text>\n" <<
                "Example:\n" <<
                "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
            return EXIT_FAILURE;          
        }

        std::string host = argv[1];
        auto const  port = argv[2];
        auto const  text = argv[3];

        // The io_context is required for all I/O
        net::io_context ioc;

        // There objects perform out I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);


        auto ep = net::connect(ws.next_layer(), results);
        // Update the host_string. This will provide the value of the
        // Host HTTP header during the WEbsocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4

        host += ':' + std::to_string(ep.port());

        // Set a decortor to change the User-agent of the handshake
        ws.set_option(websocket::stream_base::decorator([](websocket::request_type& req)
        {
            req.set(http::field::user_agent,
                string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-vilent-coro");
        }));

        // Perform the websocket handshake
        ws.handshake(host, "/");

        ws.write(net::buffer(std::string(text)));
        //incoming msg buffer
        beast::flat_buffer buffer;

        ws.read(buffer);

        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << beast::make_printable(buffer.data()) << std::endl;


    }

    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

    
