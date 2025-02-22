#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class Register : public QWidget {
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);

private slots:
    void handleRegister();

private:
    QLineEdit *usernameField;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QPushButton *registerButton;
};

#endif // REGISTER_H
