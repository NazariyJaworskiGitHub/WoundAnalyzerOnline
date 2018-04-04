#include "Web/UI/authmanager.h"
#include "Web/UI/mainwindow.h"

Web::Ui::AuthLoginWidget::DialogLogIn::DialogLogIn(Wt::WContainerWidget *parent) : WDialog(parent)
{
    userLabel = new WLabel("User:",this->contents());
    userLineEdit = new WLineEdit(this->contents());
    userLabel->setBuddy(userLineEdit);
    passwordLabel = new WLabel("Password:",this->contents());
    passwordLineEdit = new WLineEdit(this->contents());
    passwordLineEdit->setEchoMode(WLineEdit::Password);
    passwordLabel->setBuddy(passwordLineEdit);
    this->contents()->addWidget(new WBreak());
    loginPushButton = new WPushButton("Login",this->contents());
    loginPushButton->setStyleClass("btn-primary");
    loginPushButton->setFloatSide(Side::Right);
    loginPushButton->clicked().connect(this, &DialogLogIn::accept);
}

void Web::Ui::AuthLoginWidget::setLogInButton()
{
    logInOutPushButton->setIcon(WLink("icons/Log-In.png"));
    logInOutPushButton->setToolTip("Log In");
    logInOutPushButton->show();
    logInOutPushButton->clicked().connect(std::bind([=]()
    {
        DialogLogIn *dialogLogIn = new DialogLogIn(this);
        dialogLogIn->show();
        for(auto child : ((MainWindow*)this->parent())->mainContainer->children())
            delete child;
        ((MainWindow*)this->parent())->windowImageEdit = new WindowImageEdit(((MainWindow*)this->parent())->mainContainer);
        setLogOutButton();
    }));
}

void Web::Ui::AuthLoginWidget::setLogOutButton()
{
    logInOutPushButton->setIcon(WLink("icons/Log-In.png"));
    logInOutPushButton->setToolTip("Log In");
    logInOutPushButton->show();
}

Web::Ui::AuthLoginWidget::AuthLoginWidget(WContainerWidget *parent) : WContainerWidget(parent)
{
    logInOutPushButton = new WPushButton(this);
    setLogInButton();
    this->hide();
}
