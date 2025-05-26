#ifndef VALUEMQTT_H
#define VALUEMQTT_H

#include <QMainWindow>
#include <QMqttClient>
#include <QLabel>
#include <QVBoxLayout>
#include <QSlider>

class QChart;
class QLineSeries;
class QValueAxis;
class QPushButton;

class ValueMQTT : public QMainWindow
{
    Q_OBJECT

public:
    ValueMQTT(QWidget *parent = nullptr);
    ValueMQTT(const QString &title, QWidget *parent = nullptr);
    ~ValueMQTT();

public slots:
    void onMqttStateChanged(QMqttClient::ClientState state);
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onMqttError(QMqttClient::ClientError error);
    void startIrrigation();
    void stopIrrigation();
    void updatePumpPowerLabel(int value);
    void setPumpPower(int value);
    void setMqttValues(const QString &values);

private:
    void initUI();
    void initMQTT();

    QMqttClient *mqttClient;
    QWidget *centralWidget;
    QVBoxLayout *layout;

    QLabel *tempLabel;
    QLabel *humLabel;
    QLabel *lightLabel;
    QLabel *batteryLabel;
    QLabel *statusLabel;
    QSlider *pumpPowerSlider;
    QLabel *pumpPowerLabel;
    int currentPumpPower;

    QPushButton *startIrrigationButton;
    QPushButton *stopIrrigationButton;

    QChart *chart;
    QLineSeries *tempSeries;
    QLineSeries *humSeries;
    QLineSeries *lightSeries;
    QLineSeries *batterySeries;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int dataPointCounter;
};

#endif // VALUEMQTT_H
