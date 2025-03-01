#include "dashbordwindow.h"
#include <QLabel>
#include <QVBoxLayout>

DashbordWindow::DashbordWindow(QWidget *parent) : QMainWindow(parent) {
    // Set up the dashboard window
    setWindowTitle("Dashboard");
    setFixedSize(800, 600);

    // Create a central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Add a label to the dashboard
    QLabel *welcomeLabel = new QLabel("Welcome to the Dashboard!", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(welcomeLabel);

    // Set the central widget
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
