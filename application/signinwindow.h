#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SignInWindow : public QWidget {
    Q_OBJECT
public:
    explicit SignInWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *backgroundLabel;
    QLabel *logoLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *signInButton;
    QPushButton *exitButton;
};

#endif // SIGNINWINDOW_H
