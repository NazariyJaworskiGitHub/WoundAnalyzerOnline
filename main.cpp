#include "configurationparameters.h"
#include "Utilities/Logger/logger.h"
#include "Web/webserver.h"

//#include <stdexcept>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication _app(argc, argv);

    // Read configuration
    ConfigurationParameters::instance()->readConfigurationFile();

    // Prepare global logger
    Log::GlobalLoggingLevel = ConfigurationParameters::instance()->loggingLevel;

    // Start webserver
    Web::WebServer server;
    server.startServer();

    // Run event listener, wait for Ctrl+C from console to shutdown application
    return _app.exec();
}
