#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard), dataPointCounter(0) {
    ui->setupUi(this);

    // Create MQTT Client
    mqttClient = new QMqttClient(this);
    mqttClient->setHostname("192.168.1.100"); // Change to your broker IP
    mqttClient->setPort(1883);

    connect(mqttClient, &QMqttClient::messageReceived, this, &Dashboard::onMessageReceived);
    mqttClient->connectToHost();

    // Create Labels
    tempLabel = new QLabel("Temperature: -- °C", this);
    tempLabel->setGeometry(20, 20, 200, 30);

    humLabel = new QLabel("Humidity: -- %", this);
    humLabel->setGeometry(20, 60, 200, 30);

    // Create Chart
    auto *chart = new QtCharts::QChart();
    tempSeries = new QtCharts::QLineSeries();
    humSeries = new QtCharts::QLineSeries();
    chart->addSeries(tempSeries);
    chart->addSeries(humSeries);

    auto *axisX = new QtCharts::QValueAxis();
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    tempSeries->attachAxis(axisX);
    humSeries->attachAxis(axisX);

    auto *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    tempSeries->attachAxis(axisY);
    humSeries->attachAxis(axisY);

    auto *chartView = new QtCharts::QChartView(chart, this);
    chartView->setGeometry(20, 100, 400, 300);

    chart->setTitle("Sensor Data");
    chart->legend()->setVisible(true);

    // Subscribe to Topics
    mqttClient->subscribe("sensor/temperature");
    mqttClient->subscribe("sensor/humidity");
}

Dashboard::~Dashboard() {
    delete ui;
}

void Dashboard::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic) {
    bool ok;
    float value = message.toFloat(&ok);

    if (ok) {
        if (topic.name() == "sensor/temperature") {
            tempLabel->setText("Temperature: " + QString::number(value) + " °C");
            tempSeries->append(dataPointCounter++, value);
        } else if (topic.name() == "sensor/humidity") {
            humLabel->setText("Humidity: " + QString::number(value) + " %");
            humSeries->append(dataPointCounter++, value);
        }
    }
}
