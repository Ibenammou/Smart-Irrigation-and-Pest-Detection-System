#include "register.h"
#include <QMessageBox>

Register::Register(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 350);
    setStyleSheet("background-color: white;");

    QLabel *titleLabel = new QLabel("Register Account", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #4CAF50;");

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Enter username");

    emailField = new QLineEdit(this);
    emailField->setPlaceholderText("Enter email");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter password");
    passwordField->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Register", this);
    registerButton->setStyleSheet("background-color: #4CAF50; color: white;");
    connect(registerButton, &QPushButton::clicked, this, &Register::handleRegister);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(usernameField);
    layout->addWidget(emailField);
    layout->addWidget(passwordField);
    layout->addWidget(registerButton);
    setLayout(layout);
}

void Register::handleRegister() {
    if (usernameField->text().isEmpty() || emailField->text().isEmpty() || passwordField->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required!");
        return;
    }

    QMessageBox::information(this, "Success", "Account registered successfully!");
    this->close();
}
