#include "authmanager.h"

void AuthManager::readUsersFile(const QString usersFileName) throw(std::exception)
{
    if(!QFile::exists(usersFileName))
    {
        std::string errMsg = "[Authentication manager] ERROR: Can't find " + usersFileName.toStdString() + "\n";
        Log::GlobalLogger.msg(Log::ERROR, errMsg);
        throw std::runtime_error(errMsg);
    }
    else
    {
        QFile _usersFile(usersFileName);
        if(!_usersFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            std::string errMsg = "[Authentication manager] ERROR: Can't open "+ usersFileName.toStdString() + "\n";
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

const AuthManager::User* AuthManager::checkUser(const std::string &username, const std::string &password)
{
    for(auto i : users)
        if(!((username.compare(i->username)) || (password.compare(i->password)))) return i;
    return nullptr;
}
