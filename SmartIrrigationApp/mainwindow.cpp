#include "mainwindow.h"
#include "splashscreen.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Show the splash screen
    SplashScreen *splash = new SplashScreen();
    splash->show();

    // Timer to switch to login after 2 seconds
    splashTimer = new QTimer(this);
    connect(splashTimer, &QTimer::timeout, this, &MainWindow::showLoginWindow);
    splashTimer->start(2000);
}

MainWindow::~MainWindow() {}

void MainWindow::showLoginWindow() {
    splashTimer->stop();  // Stop the timer

    loginWindow = new LoginWindow();
    loginWindow->show();  // Show the login window

    this->close();  // Close the main window
}
