#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>

// https://github.com/zeromq/cppzmq
// https://zeromq.org/get-started/
// https://zeromq.org/languages/c/
// https://zeromq.org/get-started/?language=cpp&library=zmqpp#
// https://github.com/zeromq/zmqpp
// https://stackoverflow.com/questions/14233887/zmq-compilation-problems-for-c-examples
// https://github.com/uzh-rpg/flightmare/issues/170
// https://johnfarrier.com/zeromqs-req-rep-pattern-magic-lightning-fast-c-python-messaging/
// https://github.com/nlohmann/json/issues/1703
// https://ogbe.net/blog/zmq_helloworld
// https://launchpad.net/zmqpp/+packages
// https://www.ubuntuupdates.org/package/core/focal/universe/base/libzmqpp-dev
// https://zeromq.org/get-started/?language=cpp&library=cppzmq

// sudo apt-get install gnutls-bin
// sudo apt-get install libzmq3-dev
// sudo apt-get install libczmq-dev
// sudo apt-get install nlohmann-json3-dev
// g++ -o client -std=gnu++14 client.cpp -lzmq
// g++ -o server -std=gnu++14 server.cpp -lzmq
// #include <zmq.hpp> #include <nlohmann/json.hpp>
// #include <zmqpp/zmqpp.hpp>
// sudo apt-get install libzmqpp-dev
// g++ -c -Wall -O0 -g -fPIC -std=gnu++14 -o server_client server_client.cpp
// pip install pyzmq
// g++ -std=gnu++14 -o server_client server_client.cpp -lzmq

using namespace std;

void server()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    // prepare some static data for responses
    const std::string data{"World"};

    for (;;) 
    {
        zmq::message_t request;

        // receive a request from client
        socket.recv(request, zmq::recv_flags::none);
        std::cout << "Received " << request.to_string() << std::endl;

        // simulate work
        std::this_thread::sleep_for(1s);

        // send the reply to the client
        socket.send(zmq::buffer(data), zmq::send_flags::none);
    }

    return;
}

void client(int val)
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // set up some static data to send
    const std::string data = "Hello" + std::to_string(val) + " N";

    for (auto request_num = 0; request_num < 10; ++request_num) 
    {
        // send the request message
        std::cout << "Sending " << data << request_num << "..." << std::endl;
        socket.send(zmq::buffer(data), zmq::send_flags::none);
        
        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string(); 
        std::cout << " (" << request_num << ")";
        std::cout << std::endl;
    }

    return;
}

int main() 
{
    thread ts(server);
    thread tc0(client, 11);
    thread tc1(client, 22);

    tc0.join();
    tc1.join();
    ts.join();
    return 0;
}
