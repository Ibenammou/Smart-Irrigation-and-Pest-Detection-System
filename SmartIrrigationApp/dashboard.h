#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressBar>

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private:
    QLabel *humidityLabel;
    QLabel *temperatureLabel;
    QProgressBar *humidityProgress;
    QProgressBar *temperatureProgress;
    QPushButton *toggleIrrigationButton;
    QPushButton *notificationButton;
};

#endif // DASHBOARD_H
