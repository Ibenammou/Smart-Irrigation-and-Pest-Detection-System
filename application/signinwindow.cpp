#include "signinwindow.h"
#include "dashboardwindow.h"
#include "registerwindow.h" // Inclusion de la fenêtre d'inscription
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>

SignInWindow::SignInWindow(QWidget *parent) : QWidget(parent) {
    // Make the window full screen
    setWindowState(Qt::WindowFullScreen);
    setGeometry(QGuiApplication::primaryScreen()->geometry());

    // Set background image
    QLabel *backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap(":/images/background.jpg");
    backgroundLabel->setPixmap(backgroundPixmap);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Apply stronger transparency effect to background
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.3);  // 30% visible (More transparent)
    backgroundLabel->setGraphicsEffect(opacityEffect);

    // Small circular logo at the top
    QLabel *logoContainer = new QLabel(this);
    logoContainer->setFixedSize(80, 80);
    logoContainer->setStyleSheet("border-radius: 40px; background-color: white; border: 2px solid #ddd;");

    logoLabel = new QLabel(logoContainer);
    QPixmap logoPixmap(":/images/logo (2).png");
    int size = 80; // Size of the circular logo

    // Create a circular mask
    QPixmap circularPixmap(size, size);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(logoPixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Draw the circular shape
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    // Set the logo to the QLabel
    logoLabel->setPixmap(circularPixmap);
    logoLabel->setFixedSize(size, size);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Username and Password fields
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Enter your username");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Enter your password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Style input fields
    usernameEdit->setFixedSize(300, 40);
    passwordEdit->setFixedSize(300, 40);
    usernameEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    passwordEdit->setStyleSheet("padding: 5px; font-size: 16px;");

    // Login Button
    loginButton = new QPushButton("Sign In", this);
    loginButton->setFixedSize(300, 40);
    loginButton->setStyleSheet("background-color: #2ECC71; color: white; font-size: 16px; font-weight: bold;");
    connect(loginButton, &QPushButton::clicked, this, &SignInWindow::onLoginClicked);

    // Register and Forgot Password options
    registerLabel = new QLabel("<a href='#'>Register Now</a>", this);
    forgotPasswordLabel = new QLabel("<a href='#'>Forgot Password?</a>", this);
    registerLabel->setStyleSheet("color: blue; font-size: 17px;");
    forgotPasswordLabel->setStyleSheet("color: blue; font-size: 17px;");

    // Make links clickable
    registerLabel->setTextFormat(Qt::RichText);
    registerLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerLabel->setOpenExternalLinks(false);
    connect(registerLabel, &QLabel::linkActivated, this, &SignInWindow::onRegisterClicked);

    forgotPasswordLabel->setTextFormat(Qt::RichText);
    forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotPasswordLabel->setOpenExternalLinks(false);
    connect(forgotPasswordLabel, &QLabel::linkActivated, this, &SignInWindow::onForgotPasswordClicked);

    // Layout for Register and Forgot Password
    QHBoxLayout *bottomLinksLayout = new QHBoxLayout();
    bottomLinksLayout->addWidget(forgotPasswordLabel);
    bottomLinksLayout->addWidget(registerLabel);

    // Centered Form Layout
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoContainer, 0, Qt::AlignCenter);
    formLayout->addSpacing(10);
    formLayout->addWidget(usernameEdit);
    formLayout->addWidget(passwordEdit);
    formLayout->addWidget(loginButton);
    formLayout->addLayout(bottomLinksLayout);

    // Wrapper layout to center form in middle of screen
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addLayout(formLayout);
    centerLayout->addStretch();

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(centerLayout);
    setLayout(mainLayout);

    // Exit Button
    exitButton = new QPushButton("X", this);
    exitButton->setStyleSheet("background-color: red; color: white; font-weight: bold; border-radius: 10px;");
    exitButton->setFixedSize(30, 30);
    connect(exitButton, &QPushButton::clicked, this, &SignInWindow::onExitClicked);

    // Position the exit button at the top-right corner manually
    exitButton->setParent(this);
    exitButton->setGeometry(width() - 40, 10, 30, 30);

    // Adjust exit button position when window resizes
    connect(qApp, &QApplication::aboutToQuit, this, [this]() {
        exitButton->setGeometry(width() - 40, 10, 30, 30);
    });
}

// Handle login button click
void SignInWindow::onLoginClicked() {
    DashboardWindow *dashboardWindow = new DashboardWindow();
    dashboardWindow->show();
    this->close();
}

// Handle register button click
void SignInWindow::onRegisterClicked() {
    RegisterWindow *registerWindow = new RegisterWindow();
    registerWindow->show();
}

// Handle forgot password button click
void SignInWindow::onForgotPasswordClicked() {
    // TODO: Open forgot password window
}

// Handle exit button click
void SignInWindow::onExitClicked() {
    close();
}
