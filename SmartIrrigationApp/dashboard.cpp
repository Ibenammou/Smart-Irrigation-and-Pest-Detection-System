#include "dashboard.h"
#include <QMessageBox>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent) {
    setFixedSize(500, 400);
    setStyleSheet("background-color: #f0f0f0;");

    QLabel *titleLabel = new QLabel("Irrigation Dashboard", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #4CAF50;");

    humidityLabel = new QLabel("Humidity: 70%", this);
    humidityProgress = new QProgressBar(this);
    humidityProgress->setRange(0, 100);
    humidityProgress->setValue(70);
    humidityProgress->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");

    temperatureLabel = new QLabel("Temperature: 25°C", this);
    temperatureProgress = new QProgressBar(this);
    temperatureProgress->setRange(0, 50);
    temperatureProgress->setValue(25);
    temperatureProgress->setStyleSheet("QProgressBar::chunk { background-color: #FF9800; }");

    toggleIrrigationButton = new QPushButton("Toggle Irrigation", this);
    connect(toggleIrrigationButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Irrigation", "Irrigation toggled for Zone 1.");
    });

    notificationButton = new QPushButton("Notifications", this);
    connect(notificationButton, &QPushButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Notifications", "No new notifications.");
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(humidityLabel);
    layout->addWidget(humidityProgress);
    layout->addWidget(temperatureLabel);
    layout->addWidget(temperatureProgress);
    layout->addWidget(toggleIrrigationButton);
    layout->addWidget(notificationButton);
    setLayout(layout);
}

Dashboard::~Dashboard() {}
