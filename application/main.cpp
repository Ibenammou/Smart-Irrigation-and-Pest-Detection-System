#include <QApplication>
#include "logowindow.h"
#include "signinwindow.h"
#include "dashboardwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the LogoWindow (Splash Screen)
    LogoWindow *logoWindow = new LogoWindow();
    logoWindow->show();

    // Connect the LogoWindow's finished signal
    QObject::connect(logoWindow, &LogoWindow::finished, [&]() {
        logoWindow->deleteLater();

        // Create and show the SignInWindow
        SignInWindow *signInWindow = new SignInWindow();

        // Connect the sign-in success signal
        QObject::connect(signInWindow, &SignInWindow::loginSuccess, [&]() {
            signInWindow->close();

            // Create and show the DashboardWindow
            DashboardWindow *dashboardWindow = new DashboardWindow();
            dashboardWindow->show();
        });

        signInWindow->show();
    });

    return app.exec();
}
