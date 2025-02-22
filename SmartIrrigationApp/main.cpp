#include <QApplication>
#include "splashscreen.h"
#include "loginwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SplashScreen splash;
    splash.show();
    splash.startAnimation();  // Start splash screen animation

    // Wait for splash animation to finish before opening login window
    QTimer::singleShot(3000, [&]() {
        splash.close();
        LoginWindow *login = new LoginWindow();
        login->show();
    });

    return app.exec();
}
