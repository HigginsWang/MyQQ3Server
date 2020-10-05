#include "MyQQ3Server.h"
#include "MySqlDatabase.h"
MySqlDatabase mySqlDatabase;
#pragma comment(lib,"mysqlcppconn.lib")
int main() {

    MyQQ3Server ser;
    if (mySqlDatabase.init()) {
        std::cout << "successfully connect mysql database" << std::endl;
    }
    try {
        ser.Run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "other exception" << std::endl;
    }
    
    
    return 0;
}