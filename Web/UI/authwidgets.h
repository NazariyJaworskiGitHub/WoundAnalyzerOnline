#ifndef AUTHWIDGETS
#define AUTHWIDGETS

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WDialog>
#include <Wt/WMessageBox>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WBreak>

#include "authmanager.h"

using namespace Wt;

namespace Web
{
namespace Ui {
    class LogInForm : public WDialog
    {
        WLabel *userNameLabel = nullptr;
        WLineEdit *userNameLineEdit = nullptr;
        WLabel *passwordLabel = nullptr;
        WLineEdit *passwordLineEdit = nullptr;
        WPushButton *loginPushButton = nullptr;
        public: void onLogin()
        {
            if(AuthManager::instance()->checkAndLogInUser(
                        userNameLineEdit->text().toUTF8(),
                        passwordLineEdit->text().toUTF8()))
            {
                this->accept();
            }
            else
            {
                WMessageBox::show("Warning", "Wrong username or password", Ok);
                //this->reject();
            }
        }
        public: LogInForm(WContainerWidget *parent = 0) : WDialog(parent)
        {
            this->rejectWhenEscapePressed(true);
            this->setTitleBarEnabled(false);
            this->setClosable(true);

            userNameLabel = new WLabel("Username:",this->contents());
            userNameLineEdit = new WLineEdit(this->contents());
            userNameLabel->setBuddy(userNameLineEdit);

            passwordLabel = new WLabel("Password:",this->contents());
            passwordLineEdit = new WLineEdit(this->contents());
            passwordLineEdit->setEchoMode(WLineEdit::Password);
            passwordLabel->setBuddy(passwordLineEdit);

            this->contents()->addWidget(new WBreak());

            loginPushButton = new WPushButton("Login",this->contents());
            loginPushButton->setStyleClass("btn-primary");
            loginPushButton->setFloatSide(Side::Right);

            loginPushButton->clicked().connect(this, &LogInForm::onLogin);
            this->enterPressed().connect(this, &LogInForm::onLogin);
            userNameLineEdit->enterPressed().connect(this, &LogInForm::onLogin);
            passwordLineEdit->enterPressed().connect(this, &LogInForm::onLogin);
        }
        public: std::string getUserName(){return userNameLineEdit->text().toUTF8();}
        public: std::string getPassword(){return passwordLineEdit->text().toUTF8();}
        public: ~LogInForm(){}
    };

//    class RegisterForm : public WDialog
//    {
//        WLabel *userNameLabel = nullptr;
//        WLineEdit *userNameLineEdit = nullptr;
//        WLabel *emailLabel = nullptr;
//        WLineEdit *emailLineEdit = nullptr;
//        WLabel *passwordLabel = nullptr;
//        WLineEdit *passwordLineEdit = nullptr;
//        WLabel *confirmPasswordLabel = nullptr;
//        WLineEdit *confirmPasswordLineEdit = nullptr;
//        WPushButton *loginPushButton = nullptr;
//        bool _verifyPasswords()
//        {
//            return passwordLineEdit->text() == confirmPasswordLineEdit->text();
//        }
//        public: RegisterForm(WContainerWidget *parent = 0) : WDialog(parent)
//        {
//            userNameLabel = new WLabel("Username:",this->contents());
//            userNameLineEdit = new WLineEdit(this->contents());
//            userNameLabel->setBuddy(userNameLineEdit);
//            emailLabel = new WLabel("E-mail:",this->contents());
//            emailLineEdit = new WLineEdit(this->contents());
//            emailLabel->setBuddy(emailLineEdit);
//            passwordLabel = new WLabel("Password:",this->contents());
//            passwordLineEdit = new WLineEdit(this->contents());
//            passwordLineEdit->setEchoMode(WLineEdit::Password);
//            passwordLabel->setBuddy(passwordLineEdit);
//            confirmPasswordLabel = new WLabel("Confirm password:",this->contents());
//            confirmPasswordLineEdit = new WLineEdit(this->contents());
//            confirmPasswordLineEdit->setEchoMode(WLineEdit::Password);
//            confirmPasswordLabel->setBuddy(confirmPasswordLineEdit);
//            this->contents()->addWidget(new WBreak());
//            loginPushButton = new WPushButton("Register",this->contents());
//            loginPushButton->setStyleClass("btn-primary");
//            loginPushButton->setFloatSide(Side::Right);
//            loginPushButton->clicked().connect(std::bind([=](){
//                if(_verifyPasswords()) this->accept();
//                else WMessageBox::show("Warning", "Confirmation password differs", Ok);
//            }));
//        }
//        public: WString getUserName(){return userNameLineEdit->text();}
//        public: WString getEmail(){return emailLineEdit->text();}
//        public: WString getPassword(){return passwordLineEdit->text();}
//        public: ~RegisterForm(){}
//    };
}
}

#endif // AUTHWIDGETS

