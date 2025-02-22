#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "register.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private slots:
    void handleLogin();
    void openRegister();

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *forgotPasswordButton;
};

#endif // LOGINWINDOW_H
