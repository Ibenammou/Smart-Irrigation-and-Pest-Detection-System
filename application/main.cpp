#include <QApplication>
#include "logowindow.h"
#include "signinwindow.h"
#include "dashboardwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the LogoWindow (Splash Screen)
    LogoWindow *logoWindow = new LogoWindow();
    logoWindow->show();

    // Connect the LogoWindow's finished signal to open the SignInWindow
    QObject::connect(logoWindow, &LogoWindow::finished, [&]() {
        logoWindow->deleteLater(); // Delete the splash screen after it's done

        // Create and show the SignInWindow
        SignInWindow *signInWindow = new SignInWindow();

        // Connect the sign-in success signal to show the DashboardWindow
        QObject::connect(signInWindow, &SignInWindow::loginSuccess, [&]() {
            // Close the SignInWindow
            signInWindow->close();

            // Create and show the DashboardWindow
            DashboardWindow *dashboardWindow = new DashboardWindow();


            dashboardWindow->show();
        });

        // Show the SignInWindow
        signInWindow->show();
    });

    // Start the application event loop
    return app.exec();
}
