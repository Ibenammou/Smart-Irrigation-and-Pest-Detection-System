#include <QApplication>
#include "FarmDashboard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    FarmDashboard dashboard;
    dashboard.show();

    return app.exec();
}
