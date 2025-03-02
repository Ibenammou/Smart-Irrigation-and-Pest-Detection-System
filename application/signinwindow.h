#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class SignInWindow : public QWidget {
    Q_OBJECT

public:
    explicit SignInWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void onLoginClicked();                // Function to handle login click
    void onExitClicked();                 // Function to handle exit button click
    void onRegisterClicked();             // Function to handle register click
    void onForgotPasswordClicked();       // Function to handle forgot password click

private:
    QLineEdit *usernameEdit;              // Username input field
    QLineEdit *passwordEdit;              // Password input field
    QPushButton *loginButton;             // Login button
    QPushButton *exitButton;              // Exit button
    QLabel *logoLabel;                    // Logo label
    QLabel *forgotPasswordLabel;          // Forgot password label
    QLabel *registerLabel;                // Register label
};

#endif // SIGNINWINDOW_H
