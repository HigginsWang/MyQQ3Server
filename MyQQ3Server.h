#pragma once
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "MySqlDatabase.h"
#include "dataStructure.h"
typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::map<int, websocketpp::connection_hdl> map_int_hdl;
typedef server::message_ptr message_ptr;

extern MySqlDatabase mySqlDatabase;

class MyQQ3Server {
public:
	MyQQ3Server();
	void Run();
	//void on_open(websocketpp::connection_hdl hdl);
	void send(std::string messageContent, message_type mst,websocketpp::connection_hdl hdl);
private:
	server m_server;
	map_int_hdl conn_map;

	void on_open(websocketpp::connection_hdl hdl);
	void on_close(websocketpp::connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

	
};