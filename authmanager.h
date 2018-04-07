#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <string>
#include <vector>

// Database parameters
#define USERS_FILE_KEYWORD_DOCTOR   "DOCTOR"
#define USERS_FILE_KEYWORD_PATIENT  "PATIENT"
#define USERS_FILE_KEYWORD_ADMIN    "ADMIN"

class AuthManager
{
    public : class User
    {
        public : enum Status {DOCTOR, PATIENT, ADMIN};
        public : Status status;
        public : std::string username;
        public : std::string email;
        public : std::string password;
        public : User(){}
        public : User(const User &u):
            status(u.status),
            username(u.username),
            email(u.email),
            password(u.password){}
        public : User(
                Status status,
                const std::string &username,
                const std::string &email,
                const std::string password):
            status(status),
            username(username),
            email(email),
            password(password){}
        public : bool operator == (const User &u)
        {
            return !((username.compare(u.username)) || (password.compare(u.password)));}
    };

    public : std::vector<AuthManager::User*> users;
    public : void readUsersFile(const std::string usersFileName = "users.cfg") throw(std::exception);
    public : const AuthManager::User *checkAndLogInUser(const std::string &username, const std::string &password);
    public : void logOutCurrentUser();
    private: AuthManager(){}
    private: ~AuthManager(){for(auto i : users) delete i;}
    public : static AuthManager *instance()
    {
        static AuthManager _instanceOfAuthManager;
        return &_instanceOfAuthManager;
    }
};
#endif // AUTHMANAGER_H
