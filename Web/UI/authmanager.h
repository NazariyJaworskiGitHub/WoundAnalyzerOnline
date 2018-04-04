#ifndef AUTHMANAGER
#define AUTHMANAGER

// For usage of Qt native macro SIGNAL() and SLOT()
#ifndef WT_NO_SLOT_MACROS
#define WT_NO_SLOT_MACROS
#endif // WT_NO_SLOT_MACROS

// For disabling boost warnings
#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif // BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

using namespace Wt;

namespace Web
{
namespace Ui
{
class AuthLoginWidget : public WContainerWidget
{
    public : WPushButton *loginPushButton = nullptr;
    public : AuthLoginWidget(WContainerWidget *parent) : WContainerWidget(parent)
    {
        loginPushButton = new WPushButton(this);
        loginPushButton->setIcon(WLink("icons/Log-In.png"));
        loginPushButton->setToolTip("Log In");
    }

//    public : WHBoxLayout *hBoxLayout = nullptr;
//    public : WLabel *userLabel = nullptr;
//    public : WLineEdit *userLineEdit = nullptr;
//    public : WLabel *passwordLabel = nullptr;
//    public : WLineEdit *passwordLineEdit = nullptr;
//    public : WPushButton *loginPushButton = nullptr;
//    public : AuthLoginWidget(WContainerWidget *parent) : WContainerWidget(parent)
//    {
//        //hBoxLayout = new WHBoxLayout(this);
//        userLabel = new WLabel("User:",this);
//        userLineEdit = new WLineEdit(this);
//        userLabel->setBuddy(userLineEdit);
////        hBoxLayout->addWidget(userLabel);
////        hBoxLayout->addWidget(userLineEdit);
//        passwordLabel = new WLabel("Password:",this);
//        passwordLineEdit = new WLineEdit(this);
//        passwordLabel->setBuddy(passwordLineEdit);
////        hBoxLayout->addWidget(passwordLabel);
////        hBoxLayout->addWidget(passwordLineEdit);
//        loginPushButton = new WPushButton("Login",this);
////        hBoxLayout->addWidget(loginPushButton);
//    }
};
}
}
#endif // AUTHMANAGER

