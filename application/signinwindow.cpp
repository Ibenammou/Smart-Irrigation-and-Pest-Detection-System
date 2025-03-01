#include "signinwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <QDebug>

SignInWindow::SignInWindow(QWidget *parent) : QWidget(parent) {
    // Set full screen
    showFullScreen();

    // Load background image
    backgroundLabel = new QLabel(this);
    QPixmap backgroundPixmap(":/images/background.jpg");
    if (backgroundPixmap.isNull()) {
        qDebug() << "Failed to load background image!";
    } else {
        qDebug() << "Background image loaded successfully!";
        backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    backgroundLabel->setGeometry(0, 0, width(), height());

    // Add a semi-transparent overlay for better readability
    QWidget *overlay = new QWidget(this);
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.3);"); // 30% opacity
    overlay->setGeometry(0, 0, width(), height());

    // Load logo
    logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/images/logo.png");
    if (logoPixmap.isNull()) {
        qDebug() << "Failed to load logo image!";
    } else {
        qDebug() << "Logo image loaded successfully!";
        logoLabel->setPixmap(logoPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    logoLabel->setAlignment(Qt::AlignCenter);

    // Username field
    usernameLabel = new QLabel("Username:", this);
    usernameLabel->setStyleSheet("font-size: 18px; color: white;");
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Enter your username");
    usernameInput->setStyleSheet("padding: 10px; font-size: 16px; background-color: white; border-radius: 5px;");

    // Password field
    passwordLabel = new QLabel("Password:", this);
    passwordLabel->setStyleSheet("font-size: 18px; color: white;");
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 10px; font-size: 16px; background-color: white; border-radius: 5px;");

    // Sign-in button
    signInButton = new QPushButton("Sign In", this);
    signInButton->setStyleSheet("background-color: #2ECC71; color: white; font-size: 18px; padding: 10px; border-radius: 5px;");

    // Exit button
    exitButton = new QPushButton("X", this);
    exitButton->setStyleSheet("background-color: red; color: white; font-size: 16px; border-radius: 15px;");
    exitButton->setFixedSize(30, 30);
    connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Top layout for logo and exit button
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(logoLabel);
    topLayout->addStretch(); // Push the exit button to the right
    topLayout->addWidget(exitButton);

    // Form layout
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(10);
    formLayout->addWidget(usernameLabel);
    formLayout->addWidget(usernameInput);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordInput);
    formLayout->addWidget(signInButton);

    // Add everything to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void SignInWindow::resizeEvent(QResizeEvent *event) {
    // Adjust the background image when resizing
    if (backgroundLabel) {
        QPixmap backgroundPixmap(":/images/background.jpg");
        if (!backgroundPixmap.isNull()) {
            backgroundLabel->setPixmap(backgroundPixmap.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            backgroundLabel->resize(event->size());
        }
    }

    QWidget::resizeEvent(event); // Call the base class resizeEvent
}
