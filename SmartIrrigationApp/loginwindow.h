#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QFrame>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private slots:
    void handleLogin();
    void openRegister();
    void exitApp();

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *forgotPasswordButton;
    QPushButton *exitButton;
    QCheckBox *rememberMe;
};

#endif // LOGINWINDOW_H
