#ifndef VALUEMQTT_H
#define VALUEMQTT_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMqttClient>

QT_BEGIN_NAMESPACE
class QChart;
class QLineSeries;
class QValueAxis;
QT_END_NAMESPACE

class ValueMQTT : public QMainWindow
{
    Q_OBJECT

public:
    explicit ValueMQTT(QWidget *parent = nullptr);
    explicit ValueMQTT(const QString &title, QWidget *parent = nullptr);
    ~ValueMQTT();

public slots:
    void setMqttValues(const QString &values);
    void startIrrigation();
    void stopIrrigation();

private slots:
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onMqttError(QMqttClient::ClientError error);
    void onMqttStateChanged(QMqttClient::ClientState state);

private:
    void initUI();
    void initMQTT();

    QWidget *centralWidget;
    QVBoxLayout *layout;
    QLabel *tempLabel;
    QLabel *humLabel;
    QLabel *lightLabel;
    QLabel *batteryLabel;
    QLabel *statusLabel;
    QPushButton *startIrrigationButton;
    QPushButton *stopIrrigationButton;

    QMqttClient *mqttClient;
    int dataPointCounter;

    // Chart components
    QChart *chart;
    QLineSeries *tempSeries;
    QLineSeries *humSeries;
    QLineSeries *lightSeries;
    QLineSeries *batterySeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
};

#endif // VALUEMQTT_H
