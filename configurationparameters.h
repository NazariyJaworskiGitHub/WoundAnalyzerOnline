#ifndef CONFIGURATIONPARAMETERS_H
#define CONFIGURATIONPARAMETERS_H

#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "Utilities/Logger/logger.h"

// Logging parameters
#define CONFIGURATION_FILE_KEYWORD_LOGGINGLEVEL             "LOGGINGLEVEL"

// Webserver parameters
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERDOCROOT         "WEBSERVERDOCROOT"
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPADDRESS     "WEBSERVERHTTPADDRESS"
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPPORT        "WEBSERVERHTTPPORT"
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERAPPROOT         "WEBSERVERAPPROOT"
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERCONFIG          "WEBSERVERCONFIG"
#define CONFIGURATION_FILE_KEYWORD_WEBSERVERACCESSLOG       "WEBSERVERACCESSLOG"

class ConfigurationParameters
{
public: Log::LEVEL loggingLevel = Log::TRACE;
    public: struct WebServerParameters
    {
        char* DocRoot      = nullptr;
        char* HttpAddress  = nullptr;
        char* HttpPort     = nullptr;
        char* AppRoot      = nullptr;
        char* Config       = nullptr;
        char* AccessLog    = nullptr;
    } webServerParameters;
    private: bool _checkWord(QString &line, QString &word, const char* lable, char** param)
    {
        if(word.compare(QString(lable), Qt::CaseInsensitive) == 0)
        {
            word = line.section(QRegExp("\\s+"),1,1,QString::SectionSkipEmpty);
            if(*param) delete *param;
            *param = new char[word.length()];
            strcpy(*param, word.toStdString().data());
            return true;
        }
        return false;
    }

    public : void readConfigurationFile(const QString configurationFileName = "config.cfg") throw(std::exception)
    {
        if(!QFile::exists(configurationFileName))
        {
            std::string errMsg = "[Configuration parameters] ERROR: Can't find " + configurationFileName.toStdString() + "\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else
        {
            QFile _configurationFile(configurationFileName);
            if(!_configurationFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                std::string errMsg = "[Configuration parameters] ERROR: Can't open "+ configurationFileName.toStdString() + "\n";
                Log::GlobalLogger.msg(Log::ERROR, errMsg);
                throw std::runtime_error(errMsg);
            }
            else
            {
                // Processing configuration file line by line as text stream
                QTextStream _input(&_configurationFile);
                QString _currentLine;
                QString _currentWord;
                while(!_input.atEnd())
                {
                    _currentLine = _input.readLine();
                    // Read first word
                    _currentWord = _currentLine.section(QRegExp("\\s+"),0,0,QString::SectionSkipEmpty);

                    if(_currentWord.compare(QString(CONFIGURATION_FILE_KEYWORD_LOGGINGLEVEL), Qt::CaseInsensitive) == 0)
                    {
                        _currentWord = _currentLine.section(QRegExp("\\s+"),1,1,QString::SectionSkipEmpty);
                        if(_currentWord.compare(QString("TRACE"), Qt::CaseInsensitive) == 0) loggingLevel = Log::TRACE;
                        else if(_currentWord.compare(QString("DEBUG"), Qt::CaseInsensitive) == 0) loggingLevel = Log::DEBUG;
                        else if(_currentWord.compare(QString("INFO"), Qt::CaseInsensitive) == 0) loggingLevel = Log::INFO;
                        else if(_currentWord.compare(QString("WARN"), Qt::CaseInsensitive) == 0) loggingLevel = Log::WARN;
                        else if(_currentWord.compare(QString("ERROR"), Qt::CaseInsensitive) == 0) loggingLevel = Log::ERROR;
                        else if(_currentWord.compare(QString("FATAL"), Qt::CaseInsensitive) == 0) loggingLevel = Log::FATAL;
                        else if(_currentWord.compare(QString("OFF"), Qt::CaseInsensitive) == 0) loggingLevel = Log::OFF;
                        else
                        {
                            std::string errMsg = "[Configuration parameters] ERROR: " CONFIGURATION_FILE_KEYWORD_LOGGINGLEVEL " wrong parameter in the configuration file\n";
                            Log::GlobalLogger.msg(Log::ERROR, errMsg);
                            throw std::runtime_error(errMsg);
                        }
                        continue;
                    }
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERDOCROOT, &webServerParameters.DocRoot))continue;
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPADDRESS, &webServerParameters.HttpAddress))continue;
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPPORT, &webServerParameters.HttpPort))continue;
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERAPPROOT, &webServerParameters.AppRoot))continue;
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERCONFIG, &webServerParameters.Config))continue;
                    else if(_checkWord(_currentLine, _currentWord, CONFIGURATION_FILE_KEYWORD_WEBSERVERACCESSLOG, &webServerParameters.AccessLog))continue;
                }
                _configurationFile.close();
            }
        }
        // check empty parameters
        if(webServerParameters.AccessLog == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERACCESSLOG " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else if(webServerParameters.AppRoot == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERAPPROOT " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else if(webServerParameters.Config == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERCONFIG " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else if(webServerParameters.DocRoot == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERDOCROOT " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else if(webServerParameters.HttpAddress == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPADDRESS " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else if(webServerParameters.HttpPort == nullptr)
        {
            std::string errMsg = "[Configuration parameters] ERROR: There aren't " CONFIGURATION_FILE_KEYWORD_WEBSERVERHTTPPORT " parameter in the configuration file\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
    }

    private: ConfigurationParameters(){}
    private: ~ConfigurationParameters()
    {
        if(webServerParameters.AccessLog) delete webServerParameters.AccessLog;
        if(webServerParameters.AppRoot) delete webServerParameters.AppRoot;
        if(webServerParameters.Config) delete webServerParameters.Config;
        if(webServerParameters.DocRoot) delete webServerParameters.DocRoot;
        if(webServerParameters.HttpAddress) delete webServerParameters.HttpAddress;
        if(webServerParameters.HttpPort) delete webServerParameters.HttpPort;
    }

    public : static ConfigurationParameters *instance()
    {
        static ConfigurationParameters _instanceOfConfigurationParameters;
        return &_instanceOfConfigurationParameters;
    }
};

#endif // CONFIGURATIONPARAMETERS_H
