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
#include <Wt/WDialog>
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
    public: class DialogLogIn : public WDialog
    {
        public : WLabel *userLabel = nullptr;
        public : WLineEdit *userLineEdit = nullptr;
        public : WLabel *passwordLabel = nullptr;
        public : WLineEdit *passwordLineEdit = nullptr;
        public : WPushButton *loginPushButton = nullptr;
        public : DialogLogIn(WContainerWidget *parent = 0);
    };

    public : WPushButton *logInOutPushButton = nullptr;
    public : void setLogInButton();
    public : void setLogOutButton();

    public : AuthLoginWidget(WContainerWidget *parent);
};
}
}
#endif // AUTHMANAGER

