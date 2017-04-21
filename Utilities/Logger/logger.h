#ifndef LOGGER_H
#define LOGGER_H

/// \warning make shure that you are using compiler native libstdc...dll,
/// but not QT libstdc...dll, or it causes errors
/// see http://forum.qt.io/topic/45306/solved-runtime-error-with-mingw-ifstream-qt-creator/11

#include <fstream>
#include <ctime>
#include <iomanip>
#ifdef _DEBUG_MODE
#   include <iostream>
#endif //DEBUG

namespace Log
{
    enum LEVEL {TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF};
    static LEVEL GlobalLoggingLevel = TRACE;
    class Logger
    {
//        private: LEVEL _loggingLevel;
//        public : void setLoggingLevel(const LEVEL L) noexcept {_loggingLevel = L;}
//        public : LEVEL getLoggingLevel() const noexcept {return _loggingLevel;}
        private: std::ofstream _logFileStream;
        public : Logger(const std::string &logFileName/*, LEVEL L = TRACE*/)/*:_loggingLevel(L)*/
        {
            _logFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            _logFileStream.open(logFileName);
            *this << "<INFO > [Global logger] Logging is started.\n";
        }
        private : void operator << (const std::string &msg) noexcept
        {
            if(GlobalLoggingLevel/*_loggingLevel*/ != OFF)
            {
                std::time_t _time = std::time(NULL);
                std::tm _timeInfo(*std::localtime(&_time));
                _logFileStream << "["
                               << std::setfill('0') << std::setw(2) << _timeInfo.tm_hour
                               << ":"
                               << std::setfill('0') << std::setw(2) << _timeInfo.tm_min
                               << ":"
                               << std::setfill('0') << std::setw(2) << _timeInfo.tm_sec
                               << "] "
                               << msg;
                _logFileStream.flush();
#ifdef _DEBUG_MODE
                std::cout << "["
                          << std::setfill('0') << std::setw(2) << _timeInfo.tm_hour
                          << ":"
                          << std::setfill('0') << std::setw(2) << _timeInfo.tm_min
                          << ":"
                          << std::setfill('0') << std::setw(2) << _timeInfo.tm_sec
                          << "] "
                          << msg;
#endif //_DEBUG_MODE
            }
        }
        public : void msg(const LEVEL L, const std::string &msg) noexcept
        {
            if(GlobalLoggingLevel/*_loggingLevel*/ != OFF)
            {
                switch(L)
                {
                    case TRACE: if(GlobalLoggingLevel/*_loggingLevel*/ <= TRACE) *this << "<TRACE> " + msg; break;
                    case DEBUG: if(GlobalLoggingLevel/*_loggingLevel*/ <= DEBUG) *this << "<DEBUG> " + msg; break;
                    case INFO : if(GlobalLoggingLevel/*_loggingLevel*/ <= INFO ) *this << "<INFO > " + msg; break;
                    case WARN : if(GlobalLoggingLevel/*_loggingLevel*/ <= WARN ) *this << "<WARN > " + msg; break;
                    case ERROR: if(GlobalLoggingLevel/*_loggingLevel*/ <= ERROR) *this << "<ERROR> " + msg; break;
                    case FATAL: if(GlobalLoggingLevel/*_loggingLevel*/ <= FATAL) *this << "<ERROR> " + msg; break;
                    case OFF  : break;
                }
            }
        }
        public : ~Logger()
        {
            *this << "<INFO > [Global logger] Logging is finished.\n";
            _logFileStream.flush();
            _logFileStream.close();
        }
//        public : static StaticLogger& instance(){
//            static StaticLogger _instanceOfStaticLogger;
//            return _instanceOfStaticLogger;}
    };

    static Logger GlobalLogger("GlobalLogFile.log");
}

#endif // LOGGER_H
