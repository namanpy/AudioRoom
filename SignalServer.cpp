#pragma once
#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>

#include <string>
#include <thread>
#include <nlohmann/json.hpp>
#include <boost/uuid/uuid.hpp>        
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include "WSCert.cpp"
#include "EventEmitter.cpp"

#ifdef _DEBUG
#define DBG_NEW new
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif



using json = nlohmann::json;



class SignalServer {
public:
	SignalServer(const char* host, const char* port) {

		try {

			this->address = boost::asio::ip::make_address(host);
			this->port = (unsigned short)(std::atoi(port));

		}
		catch (std::exception e) {

			std::cerr << "Error: " << e.what() << std::endl;
		}


	};

	void run() {
		
        try {
            boost::asio::io_context ioContext{ 1 };
            boost::asio::ip::tcp::acceptor tcpAcceptor{ ioContext, {this->address, static_cast<unsigned short>(port) } };



			while (this->runServer) {

                //std::cout << "Waiting for another connection..." << std::endl;
                boost::asio::ip::tcp::socket socket{ ioContext };
				//boost::asio::ssl::context ctx{ boost::asio::ssl::context::tlsv12 };
				//load_server_certificate(ctx);
                
	
				
				tcpAcceptor.accept(socket);
                //std::cout << "New TCP Connection.." << std::endl;
                std::thread(&SignalServer::startSession, this, std::move(socket)).detach();

            }
        } catch(boost::system::system_error e) {
            std::cout << "Error" << e.what() << std::endl;
        } 
	};

	void startSession(boost::asio::ip::tcp::socket socket) {
        std::shared_ptr<User> user = std::shared_ptr<User>(DBG_NEW User());
		boost::asio::ssl::context ctx{ boost::asio::ssl::context::tlsv12 };
		//ctx.use_certificate_chain_file("qefasf");
		//load_server_certificate(ctx);
		ctx.use_certificate_chain_file("/root/Desktop/AudioRoom/cert/certificate.crt");
		ctx.use_private_key_file("/root/Desktop/AudioRoom/cert/private.key", boost::asio::ssl::context::pem);
		boost::beast::websocket::stream<boost::beast::ssl_stream<boost::asio::ip::tcp::socket>> ws{ std::move(socket), ctx };

		try {
			//Upgrade TCP Connection to web socket

			//ctx.use_tmp_dh_file("C:/Users/naman/Desktop/Development/AudioRoomVS/AudioRoom/cert/dh512.pem");



			ws.next_layer().handshake(boost::asio::ssl::stream_base::server);
		

			//new User;

			ws.accept();
			user->ws = &ws;


			std::cout << "Upgraded to TCP to Web Socket Connection." << std::endl;

			//Emit connect event
			json j;
			j["a"] = 1;
			//emitter->emit(user, "connect", j);


			boost::beast::flat_buffer readBuffer;

			while (true) {
                //std::cout << "[" << user->id << "]" << "  Waiting for msg.." << std::endl;
				readBuffer.clear();
				ws.read(readBuffer);

				if (ws.got_text()) {

					json j = json::parse(boost::beast::buffers_to_string(readBuffer.data()));

					emitter->emit(user, j["event"], j["data"]);

				}
               // std::cout << "[" << user->id << "]" << "  Processed msg.." << std::endl;
			}
            //std::cout << std::endl << std::endl << "[~] [~] [~] Closing" << std::endl << std::endl;
			emitter->emit(user, "disconnect", j);

		}
		catch (boost::beast::system_error e) {
			if (e.code() == boost::beast::websocket::error::closed) {
			//On Close Event
                std::cout << std::endl << std::endl << "[~] [~] [~] Closing" << std::endl << std::endl;
             json j;
                emitter->emit(user, "disconnect", j);
           }
			else {
				std::cout << e.what() << std::endl;
                std::cout << std::endl << std::endl << "[~] [~] [~] Closing" << std::endl << std::endl;
                json j;
               emitter->emit(user, "disconnect", j);
			}

		}
		catch (std::exception e) {

			json j;
			emitter->emit(user, "disconnect", j);

			std::cout << e.what() << std::endl;
		}

    }

	void setEventEmitter(EventEmitter* e) {
		emitter = e;
	};

	boost::asio::ip::address address;
	unsigned short port = 25565;
	bool runServer = true;

	EventEmitter* emitter;
};
//struct User {
//	std::string id;
//	boost::beast::websocket::stream<boost::asio::ip::tcp::socket>* ws;
//};

