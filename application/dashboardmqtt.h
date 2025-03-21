#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QMqttClient>
#include <QLabel>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class Dashboard; }
QT_END_NAMESPACE

class Dashboard : public QMainWindow {
    Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private slots:
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

private:
    Ui::Dashboard *ui;
    QMqttClient *mqttClient;
    QLabel *tempLabel;
    QLabel *humLabel;
    QtCharts::QLineSeries *tempSeries;
    QtCharts::QLineSeries *humSeries;
    int dataPointCounter;
};

#endif // DASHBOARD_H
