#include "authmanager.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include "Web/usersession.h"
#include "Utilities/Logger/logger.h"

void AuthManager::readUsersFile(const std::string usersFileName) throw(std::exception)
{
    if(!QFile::exists(QString(usersFileName.data())))
    {
        std::string errMsg = "[Authentication manager] ERROR: Can't find " + usersFileName + "\n";
        Log::GlobalLogger.msg(Log::ERROR, errMsg);
        throw std::runtime_error(errMsg);
    }
    else
    {
        QFile _usersFile(QString(usersFileName.data()));
        if(!_usersFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::string errMsg = "[Authentication manager] ERROR: Can't open "+ usersFileName + "\n";
            Log::GlobalLogger.msg(Log::ERROR, errMsg);
            throw std::runtime_error(errMsg);
        }
        else
        {
            // Processing configuration file line by line as text stream
            QTextStream _input(&_usersFile);
            QString _currentLine;
            QString _currentWord;
            while(!_input.atEnd())
            {
                User *u = new User();
                _currentLine = _input.readLine();
                if(_currentLine[0]=='#') continue;//i.e. comment ssection
                // Read first word, scip whitespaces "\\s+"
                for(int i=0; i<4; ++i)
                {
                    _currentWord = _currentLine.section(QRegExp("\\s+"),i,i,QString::SectionSkipEmpty);
                    if(!_currentWord.size())
                    {
                        std::string errMsg = "[Authentication manager] ERROR: wrong parameter in the users file\n";
                        Log::GlobalLogger.msg(Log::ERROR, errMsg);
                        throw std::runtime_error(errMsg);
                    }
                    if(i==0)
                    {
                        if(_currentWord.compare(QString(USERS_FILE_KEYWORD_DOCTOR), Qt::CaseSensitive) == 0)
                            u->status = User::DOCTOR;
                        else if(_currentWord.compare(QString(USERS_FILE_KEYWORD_PATIENT), Qt::CaseSensitive) == 0)
                            u->status = User::PATIENT;
                        else if(_currentWord.compare(QString(USERS_FILE_KEYWORD_ADMIN), Qt::CaseSensitive) == 0)
                            u->status = User::ADMIN;
                        else
                        {
                            std::string errMsg = "[Authentication manager] ERROR: wrong parameter in the users file\n";
                            Log::GlobalLogger.msg(Log::ERROR, errMsg);
                            throw std::runtime_error(errMsg);
                        }
                    }
                    else if(i==1)
                        u->username = _currentWord.toStdString();
                    else if(i==2)
                        u->password = _currentWord.toStdString();
                    else if(i==3)
                        u->email = _currentWord.toStdString();
                }
                users.push_back(u);
            }
        }
    }
}

const AuthManager::User* AuthManager::checkAndLogInUser(
        const std::string &username,
        const std::string &password)
{
    for(auto i : users)
        if(!((username.compare(i->username)) || (password.compare(i->password))))
        {
            CURRENT_SESSION->currentUser = i;
            Log::GlobalLogger.msg(
                        Log::INFO,
                        "[Authentication manager] User <" + username + "> is logged in\n");
            return i;
        }
    return nullptr;
}

void AuthManager::logOutCurrentUser()
{
    if(CURRENT_SESSION->currentUser)
    {
        Log::GlobalLogger.msg(
                    Log::INFO,
                    "[Authentication manager] User <" +
                    CURRENT_SESSION->currentUser->username +
                    "> is logged out\n");
        CURRENT_SESSION->currentUser = nullptr;
        if(CURRENT_SESSION->currentDatabaseModel) delete
            CURRENT_SESSION->currentDatabaseModel;
        CURRENT_SESSION->currentDatabaseModel = nullptr;
    }
}
