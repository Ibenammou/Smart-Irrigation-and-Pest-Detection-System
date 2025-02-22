#include "loginwindow.h"
#include "dashboard.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QApplication>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setFixedSize(800, 600);  // Window size

    // 🌟 Step 1: Background Image (Placed Behind Everything)
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/assets/background.jpeg"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, width(), height());

    // 🌟 Step 2: Login Box (QFrame with White Background)
    QFrame *loginFrame = new QFrame(this);
    loginFrame->setStyleSheet("background-color: white; border-radius: 15px;");
    loginFrame->setFixedSize(350, 400);

    // Add Drop Shadow Effect (for better visibility)
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(3, 3);
    loginFrame->setGraphicsEffect(shadowEffect);

    // 🌟 Step 3: Login Title
    QLabel *titleLabel = new QLabel("Welcome back!", loginFrame);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #333;");

    QLabel *subtitleLabel = new QLabel("User Login", loginFrame);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: gray;");

    // 🌟 Step 4: Input Fields
    usernameField = new QLineEdit(loginFrame);
    usernameField->setPlaceholderText("Email");
    usernameField->setStyleSheet("border: 1px solid #ccc; padding: 8px; border-radius: 8px;");

    passwordField = new QLineEdit(loginFrame);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setStyleSheet("border: 1px solid #ccc; padding: 8px; border-radius: 8px;");

    // 🌟 Step 5: Buttons
    loginButton = new QPushButton("Login", loginFrame);
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 8px;");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    forgotPasswordButton = new QPushButton("Forgot Password?", loginFrame);
    forgotPasswordButton->setStyleSheet("border: none; color: blue; background: transparent; text-decoration: underline;");

    registerButton = new QPushButton("Register", loginFrame);
    registerButton->setStyleSheet("border: none; color: blue; background: transparent; text-decoration: underline;");
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegister);

    exitButton = new QPushButton("Exit", loginFrame);
    exitButton->setStyleSheet("background-color: red; color: white; padding: 8px; border-radius: 8px;");
    connect(exitButton, &QPushButton::clicked, this, &LoginWindow::exitApp);

    // 🌟 Step 6: Layouts
    QVBoxLayout *formLayout = new QVBoxLayout(loginFrame);
    formLayout->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);
    formLayout->addWidget(subtitleLabel);
    formLayout->addWidget(usernameField);
    formLayout->addWidget(passwordField);
    formLayout->addWidget(loginButton);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(forgotPasswordButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(registerButton);
    formLayout->addLayout(bottomLayout);

    formLayout->addWidget(exitButton);
    loginFrame->setLayout(formLayout);

    // 🌟 Step 7: Center the Login Box
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(loginFrame);
    setLayout(mainLayout);
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
    // Add register window logic here
}

void LoginWindow::exitApp() {
    QApplication::quit();
}
