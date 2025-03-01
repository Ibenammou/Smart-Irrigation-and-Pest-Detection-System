#include <QApplication>
#include "logowindow.h"
#include "signinwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the LogoWindow (Splash Screen)
    LogoWindow *logoWindow = new LogoWindow();
    logoWindow->show();

    // Connect the LogoWindow's finished signal to open the SignInWindow
    QObject::connect(logoWindow, &LogoWindow::finished, [&]() {
        logoWindow->deleteLater(); // Delete splash screen after it's done

        // Create and show the SignInWindow
        SignInWindow *signInWindow = new SignInWindow();
        signInWindow->show();
    });

    return app.exec();
}
