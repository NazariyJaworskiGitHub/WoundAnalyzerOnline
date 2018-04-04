#include "configurationparameters.h"
#include "authmanager.h"
#include "Utilities/Logger/logger.h"
#include "Web/webserver.h"
#include "DataBase/databasemanager.h"

#include <QCoreApplication>

namespace Log
{
    /// Should declare it here and left extern declaration in logger.h to avoid the
    /// static order initialization fiasco
    Logger GlobalLogger("GlobalLogFile.log");
}

int main(int argc, char *argv[])
{
    QCoreApplication _app(argc, argv);

    // Read configuration
    Log::GlobalLogger.msg(Log::INFO, "[Global logger] Reading configuration parameters\n");
    ConfigurationParameters::instance()->readConfigurationFile();

    Log::GlobalLogger.msg(Log::INFO, "[Global logger] Reading trusted users\n");
    AuthManager::instance()->readUsersFile();

    // Prepare global logger
    Log::GlobalLoggingLevel = ConfigurationParameters::instance()->loggingLevel;

//    // Connect to database
//    if(!DatabaseManager::instance()->connectToDatabase(
//                ConfigurationParameters::instance()->databaseParameters.HostName,
//                ConfigurationParameters::instance()->databaseParameters.DatabaseName,
//                ConfigurationParameters::instance()->databaseParameters.UserName,
//                ConfigurationParameters::instance()->databaseParameters.UserPassword))
//    {
//        std::string errMsg = "[Database] Can't connect to database. The program should be terminated.\n";
//        Log::GlobalLogger.msg(Log::ERROR, errMsg);
//        throw std::runtime_error(errMsg);
//    }

    // Start webserver
    Web::WebServer server;
    server.startServer();

    // Run event listener, wait for Ctrl+C from console to shutdown application
    return _app.exec();
}
