#pragma once
#include <iostream>
#include <map>
#include <string>
//#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
//#include "CallServerLogic.cpp"
//#include <SignalServer.cpp>
#include "CallServer/CallServer.h"
using json = nlohmann::json;


struct User {
	std::string id;
	std::string room;
	std::mutex write;
	boost::beast::websocket::stream<boost::beast::ssl_stream<boost::asio::ip::tcp::socket>>* ws;
	//boost::beast::websocket::stream<boost::asio::ip::tcp::socket>* ws;
	void onSuccess() {

	}
    void emit(std::string event, json data) {

        data["event"] = event;


        std::string dataToString = data.dump();

		ws->write(boost::asio::buffer(dataToString));
       // ws->async_write(boost::asio::buffer(dataToString), boost::bind(&User::onSuccess, this));

    };
};

class EventEmitter {
public:

	typedef void (CallServer::*f) (std::shared_ptr<User> , json);

    CallServer* callServer;
	std::map<std::string, f> eventMap;


    EventEmitter(CallServer* cs) {

        callServer = cs;
    }


	void on(std::string event_, f _f ) {
	
		eventMap.insert({ event_, _f });
	}

	void emit(std::shared_ptr<User>  user, std::string event_, json data) {
	
		try {

			std::bind(eventMap.find(event_)->second, callServer ,user, data)();

		}
		catch (const std::exception &e) {
			std::cout << "Error is " << e.what();
			std::cout << "Error emitting event \"" << event_ << "\" for sender " << user->id << std::endl;


		}
	
	}
};


