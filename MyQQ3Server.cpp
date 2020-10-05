#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>

#include "dataStructure.h"
#include "MyQQ3Server.h"
#include "../../ChromeDownload/vcpkg-master/vcpkg-master/installed/x86-windows/tools/protobuf/MyQQ3/proto.pb.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/metadata.h>
#include <jdbc/cppconn/resultset_metadata.h>
#include <jdbc/cppconn/exception.h>
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


MyQQ3Server::MyQQ3Server()
{
    m_server.set_access_channels(websocketpp::log::alevel::all);
    m_server.init_asio();

    // Register handler callbacks
    m_server.set_open_handler(bind(&MyQQ3Server::on_open, this, ::_1));
    m_server.set_close_handler(bind(&MyQQ3Server::on_close, this, ::_1));
    m_server.set_message_handler(bind(&MyQQ3Server::on_message, this, ::_1, ::_2));
    m_server.listen(9002);
    m_server.start_accept();
}

void MyQQ3Server::Run()
{
    m_server.run();
}
std::string dec2hex(int i) //将int转成16进制字符串
{
    std::stringstream ioss; //定义字符串流
    std::string s_temp; //存放转化后字符
    ioss << std::resetiosflags(std::ios::uppercase) << std::setw(8) << std::setfill('0') << std::hex << i; //以十六制(小写)形式输出//取消大写的设置
    ioss >> s_temp;
    return s_temp;
}
void MyQQ3Server::on_open(websocketpp::connection_hdl hdl)
{
    static int id = 0;
    auto hdl_p = hdl.lock().get();
    std::cout << "on_open called with hdl: " << hdl_p << std::endl;
    conn_map.insert(std::pair<int, websocketpp::connection_hdl>(id++, hdl));
    m_server.get_con_from_hdl(hdl);
}

void MyQQ3Server::on_close(websocketpp::connection_hdl hdl)
{
}


void MyQQ3Server::send(std::string messageContent, message_type mst, websocketpp::connection_hdl hdl) {
    websocketpp::lib::error_code ec;

    std::string str = dec2hex((unsigned int)mst) + dec2hex(messageContent.length()) + dec2hex(50);
    std::string message = str + messageContent;
    m_server.send(hdl, message, websocketpp::frame::opcode::BINARY, ec);
    if (ec) {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }

    //con_m->record_sent_message(message);
}
/// <summary>
/// 消息协议定义
/// 0-7字节       表示消息的内容长度，16进制表示
/// 8-15字节      表示消息的类型，16进制表示
/// 16-23字节     保留
/// 24-字节       表示消息的内容
/// </summary>
void MyQQ3Server::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get() << " and message: " << msg->get_payload() << std::endl;
    /*
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }
    */
    try {
        //std::cout << "header:" << msg->get_payload().substr(0, 16) << std::endl;
        //消息类型
        message_type a_messageType = (message_type)std::stoi(msg->get_payload().substr(0, 8), nullptr, 16);
        //std::cout << "a_messageType:" << (int)a_messageType << std::endl;

        //消息内容长度
        size_t a_messageConentLength = std::stoi(msg->get_payload().substr(8, 8), nullptr, 16);
        //std::cout << "a_messageConentLength:" << (int)a_messageConentLength << std::endl;

        //保留
        unsigned int a_save = std::stoi(msg->get_payload().substr(16, 8), nullptr, 16);
        //std::cout << "a_save:" << (int)a_save << std::endl;

        //消息内容
        std::string a_messageContent = msg->get_payload().substr(24);
        //std::cout << "a_save:" << a_messageContent << std::endl;
        switch (a_messageType)
        {
        case message_type::BOTH_TEXT: 
            std::cout <<"text:" + a_messageContent << std::endl;
            break;
        case message_type::C2S_LOGIN:
        {
            MyQQ3Proto::C2S_LOGIN msgObject;
            if (msgObject.ParseFromString(a_messageContent)){
                //std::cout << msgObject.name() << std::endl;
                //std::cout << msgObject.name().length() << std::endl;
                //std::cout << msgObject.password() << std::endl;


                sql::PreparedStatement* stat = mySqlDatabase.getConnection()->prepareStatement("select * from users where username = ? and password = ?");// and password = ?");
                stat->setString(1, msgObject.name().c_str());
                stat->setString(2, msgObject.password().c_str());
                ResultSet* res = stat->executeQuery();
                if (res->rowsCount() == 1)
                {
                    try {
                        if (res->next()) 
                        {
                            //cout << string(res->getString("username").c_str()) << endl;
                            //cout << string(res->getString("password").c_str()) << endl;
                        }
                    }
                    catch (std::exception& e) {
                        cout << e.what() << endl;
                    }
                    MyQQ3Proto::S2C_LOGINRET s2c_loginret;
                    //res->
                    cout << "--------------" << endl;
                    cout << res->getString("userid").c_str() << endl;
                    cout << res->getString("username").c_str() << endl;
                    s2c_loginret.set_iflogin(1);
                    s2c_loginret.set_userid(res->getString("userid").c_str());
                    s2c_loginret.set_username(res->getString("username").c_str());
                    std::string s;
                    if (s2c_loginret.SerializeToString(&s))send(s, message_type::S2C_LOGINRET, hdl);
                }
                else
                {
                    MyQQ3Proto::S2C_LOGINRET s2c_loginret;
                    s2c_loginret.set_iflogin(0);
                    std::string s;
                    if (s2c_loginret.SerializeToString(&s))send(s, message_type::S2C_LOGINRET, hdl);
                }
                //cout << res->rowsCount() << endl;
                
            }else {
                std::cout << "初始化C2S_LOGIN对象失败" << std::endl;
            }
            break;
        }
        case message_type::C2S_GETALLFRIENDS :
        {
            MyQQ3Proto::C2S_GETALLFRIENDS msgObject;
            if (msgObject.ParseFromString(a_messageContent)) {
                sql::PreparedStatement* stat = mySqlDatabase.getConnection()->prepareStatement("select * from contacts where user1id = ? or user2id = ?");// and password = ?");
                stat->setString(1, msgObject.userid().c_str());
                stat->setString(2, msgObject.userid().c_str());
                ResultSet* res = stat->executeQuery();

                try {
                    MyQQ3Proto::S2C_GETALLFRIENDSRET s2c_getallfriendsret;
                    s2c_getallfriendsret.set_userid(msgObject.userid());
                    //s2c_getallfriendsret.set_username(msgObject.user)
                    while (res->next())
                    {
                        /*cout << res->getString("user1id").c_str() << " "  << res->getString("user1name").c_str() <<" " << res->getString("user2id").c_str() << " " << res->getString("user2name").c_str() << endl;
                        cout << msgObject.userid() << endl;
                        cout << res->getString("user1id").c_str() << endl;*/
                        if (res->getString("user1id").c_str() == msgObject.userid())
                        {
                            //cout << "---------------+++++++++++++++++++++++++++++" << std::endl;
                            MyQQ3Proto::USER_USERID_USERNAME* user_userid_username;
                            user_userid_username = s2c_getallfriendsret.add_friends();
                            user_userid_username->set_userid(res->getString("user2id").c_str());
                            //std::cout << user_userid_username->userid() << std::endl;
                            user_userid_username->set_username(res->getString("user2name").c_str());
                            //std::cout << user_userid_username->username() << std::endl;
                            s2c_getallfriendsret.set_username(res->getString("user1name").c_str());
                        }
                        else if (res->getString("user2id").c_str() == msgObject.userid())
                        {
                            MyQQ3Proto::USER_USERID_USERNAME* user_userid_username;
                            user_userid_username = s2c_getallfriendsret.add_friends();
                            user_userid_username->set_userid(res->getString("user1id").c_str());
                            user_userid_username->set_username(res->getString("user1name").c_str());
                        }
                    }
                    auto t = s2c_getallfriendsret.mutable_friends();
                    for (auto it = t->begin(); it != t->end(); ++it)
                    {
                        std::cout << it->userid() << std::endl;
                        std::cout << it->username() << std::endl;
                    }
                    std::string s;
                    if (s2c_getallfriendsret.SerializeToString(&s))send(s, message_type::S2C_GETALLFRIENDSRET, hdl);
                    
                }
                catch (std::exception& e) {
                    cout << e.what() << endl;
                }
                
            }
            else {
                std::cout << "初始化C2S_LOGIN对象失败" << std::endl;
            }
            break;
        }
        default:
            std::cout << "收到了一条服务器无法处理的消息，服务器已经自动将其忽略" << std::endl;
            break;
        }
        //std::cout << msg->get_header().length() << std::endl;
        //std::cout << "msg->get_header():" << msg->get_header() << std::endl;
        //s->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (websocketpp::exception const& e) {
        std::cout << "Echo failed because: "
            << "(" << e.what() << ")" << std::endl;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << endl;
    }
}


