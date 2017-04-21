#ifndef WEBSERVER_H
#define WEBSERVER_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WServer>
#include <Wt/WApplication>

#include "configurationparameters.h"
#include "Utilities/Logger/logger.h"
#include "Web/usersession.h"

using namespace Wt;

namespace Web
{
    class WebServer
    {
        private: WServer *_myWServer = nullptr;
        private: static WApplication *createUserSession(const WEnvironment& env)
        {
            UserSession * newSession = new UserSession(env);
            Log::GlobalLogger.msg(Log::INFO, "[Webserver] New user session "
                                  + newSession->sessionId() + "is started\n");
            return newSession;
        }
        public : WebServer()throw (std::exception)
        {
            _myWServer = new WServer();
            char *_param[] = {
                    "Webserver",
                    "--docroot",
                    ConfigurationParameters::instance()->webServerParameters.DocRoot,
                    "--http-address",
                    ConfigurationParameters::instance()->webServerParameters.HttpAddress,
                    "--http-port",
                    ConfigurationParameters::instance()->webServerParameters.HttpPort,
                    "--approot",
                    ConfigurationParameters::instance()->webServerParameters.AppRoot,
                    "--config",
                    ConfigurationParameters::instance()->webServerParameters.Config,
                    "--accesslog",
                    ConfigurationParameters::instance()->webServerParameters.AccessLog};
            _myWServer->setServerConfiguration(13, _param);
            _myWServer->addEntryPoint(Wt::Application, &createUserSession);
            Log::GlobalLogger.msg(Log::INFO, "[Webserver] Webserver is configured\n");
        }
        public : void startServer() throw (std::exception)
        {
            if(!_myWServer)
            {
                std::string errMsg = "[Webserver] ERROR: Can't start server - instance is not created yet\n";
                Log::GlobalLogger.msg(Log::ERROR, errMsg);
                throw std::runtime_error(errMsg);
            }
            else if (_myWServer->start())
            {
                Log::GlobalLogger.msg(Log::INFO, "[Webserver] Webserver has been started at port " +
                                   QString::number(_myWServer->httpPort()).toStdString() + "\n");
            }
            else
            {
                std::string errMsg = "[Webserver] ERROR: Webserver hasn't been started! \n";
                Log::GlobalLogger.msg(Log::ERROR, errMsg);
                throw std::runtime_error(errMsg);
            }
        }

        public : void stopServer() throw (std::exception)
        {
           _myWServer->stop();
           Log::GlobalLogger.msg(Log::INFO, "[Webserver] Webserver has been stoped\n");
        }
        public : bool isRunning() const {return _myWServer ? _myWServer->isRunning() : false;}
        public: ~WebServer()
        {
            if(_myWServer)
            {
                if(_myWServer->isRunning())
                    _myWServer->stop();
                delete _myWServer;
            }
        }
    };
}

#endif // WEBSERVER_H
