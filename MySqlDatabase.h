#pragma once
/* MySQL Connector/C++ specific headers */
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/metadata.h>
#include <jdbc/cppconn/resultset_metadata.h>
#include <jdbc/cppconn/exception.h>
#include <warning.h>

using namespace std;
using namespace sql;


class MySqlDatabase
{
public:
    MySqlDatabase();
    Connection* getConnection();
    bool init();
private:
    ConnectOptionsMap cop;
    //cop.insert("user", "root");
    sql::mysql::MySQL_Driver* driver = nullptr;
    sql::Connection* conn = nullptr;//driver->connect("tcp://localhost:3306/world", "root", "1234");

    
};

