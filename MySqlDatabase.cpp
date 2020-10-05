#include "MySqlDatabase.h"

MySqlDatabase::MySqlDatabase()
{
}

bool MySqlDatabase::init()
{
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://localhost:3306/myqq3", "root", "1234");
        return true;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    
    return false;
}

Connection* MySqlDatabase::getConnection()
{
    return conn;
}
