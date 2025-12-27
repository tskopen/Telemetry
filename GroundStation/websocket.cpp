/*
Compile command:
clang++ -std=c++17 -o websocket websocket.cpp \
  -I/opt/homebrew/include \
  -I/opt/homebrew/opt/boost/include \
  -L/opt/homebrew/opt/boost/lib \
  -lpthread
  */

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>

#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using namespace std;


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class WebSocketClient
{
public:
  string host;
  string port;
  string text;
  WebSocketClient(string host, string port, string text) : host_(host), port_(port), ws_(ioc_) {}

  string handshakeServer()
  {
    try
    {

           boost::asio::ip::tcp::resolver resolver(ioc_);
            auto const results = resolver.resolve(host_, port_);
        


         connect(ws_.next_layer(), results.begin(), results.end());
        
         ws_.handshake(host_, "/");
        

ws_.write(boost::asio::buffer(text_));
        
        beast::flat_buffer buffer;
        ws_.read(buffer);
        string received_message = beast::buffers_to_string(buffer.data());
        ws_.close(websocket::close_code::normal);

        return received_message;
    }

    catch(std::exception const& e)
    {
        return e.what();
    }

  }
  private:
        string host_;
        string port_;
        string text_;
        boost::asio::io_context ioc_;
        websocket::stream<tcp::socket> ws_;

};

int main()
{
        WebSocketClient client("websocket.tskopen.com", "3000", "test");
        
        string response = client.handshakeServer();

        std::cout << response << std::endl;

        return 0;
}

    
