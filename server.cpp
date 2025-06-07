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

int main() 
{
    using namespace std::chrono_literals;

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

    return 0;
}
