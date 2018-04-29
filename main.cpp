#include "configurationparameters.h"
#include "authmanager.h"
#include "Utilities/Logger/logger.h"
#include "Web/webserver.h"
#include "DataBase/databasemanagerwt.h"

#include <QCoreApplication>

namespace Log
{
    /// Should declare it here and left extern declaration in logger.h to avoid the
    /// static order initialization fiasco
    Logger GlobalLogger("GlobalLogFile.log");
}

int main(int argc, char *argv[])
{
    //std::setlocale(LC_ALL, "");

    QCoreApplication _app(argc, argv);

    // Read configuration
    Log::GlobalLogger.msg(Log::INFO, "[Global logger] Reading configuration parameters\n");
    ConfigurationParameters::instance()->readConfigurationFile();

    // Prepare global logger
    Log::GlobalLoggingLevel = ConfigurationParameters::instance()->loggingLevel;

    // Load users
    Log::GlobalLogger.msg(Log::INFO, "[Global logger] Reading trusted users\n");
    AuthManager::instance()->readUsersFile();

    // Connect to database
    DatabaseManagerWt::instance()->connectToDatabase();

    // Start webserver
    Web::WebServer server;
    server.startServer();

    // Run event listener, wait for Ctrl+C from console to shutdown application
    return _app.exec();
}
