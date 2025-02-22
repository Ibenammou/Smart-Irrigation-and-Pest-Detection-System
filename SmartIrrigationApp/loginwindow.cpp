#include "loginwindow.h"
#include "dashboard.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 300);
    setStyleSheet("background-color: white;");

    QLabel *titleLabel = new QLabel("Login to Irrigation System", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #4CAF50;");

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Username");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    loginButton->setStyleSheet("background-color: #4CAF50; color: white;");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    registerButton = new QPushButton("Register", this);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegister);

    forgotPasswordButton = new QPushButton("Forgot Password?", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(usernameField);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(forgotPasswordButton);
    setLayout(layout);
}

void LoginWindow::handleLogin() {
    if (usernameField->text() == "admin" && passwordField->text() == "1234") {
        Dashboard *dashboard = new Dashboard();
        dashboard->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials!");
    }
}

void LoginWindow::openRegister() {
    Register *registerWindow = new Register();
    registerWindow->show();
}
