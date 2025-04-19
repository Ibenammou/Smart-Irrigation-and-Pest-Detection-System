#include "valuemqtt.h"
#include <QMessageBox>
#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QLineSeries>
#include <QMqttTopicName>
#include <QMqttTopicFilter>

ValueMQTT::ValueMQTT(QWidget *parent) : QMainWindow(parent), dataPointCounter(0)
{
    initUI();
    initMQTT();
}

ValueMQTT::ValueMQTT(const QString &title, QWidget *parent) : QMainWindow(parent), dataPointCounter(0)
{
    initUI();
    initMQTT();
    setWindowTitle(title);
}

ValueMQTT::~ValueMQTT()
{
    if (mqttClient) {
        mqttClient->disconnectFromHost();
        delete mqttClient;
    }
}

void ValueMQTT::initUI()
{
    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Initialize labels
    tempLabel = new QLabel("Temperature: -- °C", centralWidget);
    humLabel = new QLabel("Humidity: -- %", centralWidget);
    lightLabel = new QLabel("Light: -- %", centralWidget);
    batteryLabel = new QLabel("Battery: -- %", centralWidget);
    statusLabel = new QLabel("Status: Ready", centralWidget);

    // Initialize chart
    chart = new QChart();
    chart->setTitle("Sensor Data");

    tempSeries = new QLineSeries();
    tempSeries->setName("Temperature");
    humSeries = new QLineSeries();
    humSeries->setName("Humidity");
    lightSeries = new QLineSeries();
    lightSeries->setName("Light");
    batterySeries = new QLineSeries();
    batterySeries->setName("Battery");

    chart->addSeries(tempSeries);
    chart->addSeries(humSeries);
    chart->addSeries(lightSeries);
    chart->addSeries(batterySeries);

    axisX = new QValueAxis();
    axisX->setTitleText("Time");
    axisX->setRange(0, 100);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Value");
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);

    tempSeries->attachAxis(axisX);
    tempSeries->attachAxis(axisY);
    humSeries->attachAxis(axisX);
    humSeries->attachAxis(axisY);
    lightSeries->attachAxis(axisX);
    lightSeries->attachAxis(axisY);
    batterySeries->attachAxis(axisX);
    batterySeries->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Initialize control buttons
    startIrrigationButton = new QPushButton("Start Irrigation", centralWidget);
    stopIrrigationButton = new QPushButton("Stop Irrigation", centralWidget);

    connect(startIrrigationButton, &QPushButton::clicked, this, &ValueMQTT::startIrrigation);
    connect(stopIrrigationButton, &QPushButton::clicked, this, &ValueMQTT::stopIrrigation);

    // Add widgets to layout
    layout->addWidget(tempLabel);
    layout->addWidget(humLabel);
    layout->addWidget(lightLabel);
    layout->addWidget(batteryLabel);
    layout->addWidget(statusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startIrrigationButton);
    buttonLayout->addWidget(stopIrrigationButton);
    layout->addLayout(buttonLayout);

    layout->addWidget(chartView);

    // Set window size
    resize(800, 600);
}

// Add this to your initMQTT() function in valuemqtt.cpp
void ValueMQTT::initMQTT()
{
    mqttClient = new QMqttClient(this);

    // Make sure the hostname is correct - this should be the IP where your MQTT broker is running
    // If Mosquitto is running on the same machine as the Qt app, use "localhost" or "127.0.0.1"
   mqttClient->setHostname("localhost"); // or "127.0.0.1"
    mqttClient->setPort(1883);
    mqttClient->setClientId("QtIrrigationControl");

    // Add these lines to increase connection timeout and keepalive
    mqttClient->setKeepAlive(60); // 60 seconds keepalive

    // Connect signals
    connect(mqttClient, &QMqttClient::messageReceived,
            this, &ValueMQTT::onMessageReceived);
    connect(mqttClient, &QMqttClient::errorChanged,
            this, &ValueMQTT::onMqttError);
    connect(mqttClient, &QMqttClient::stateChanged,
            this, &ValueMQTT::onMqttStateChanged);

    // Add this debug output
    qDebug() << "Connecting to MQTT broker at" << mqttClient->hostname() << ":" << mqttClient->port();

    mqttClient->connectToHost();
}

void ValueMQTT::onMqttStateChanged(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Disconnected:
        statusLabel->setText("Status: Disconnected from broker");
        break;
    case QMqttClient::Connecting:
        statusLabel->setText("Status: Connecting to broker...");
        break;
    case QMqttClient::Connected:
        statusLabel->setText("Status: Connected to broker");
        // Subscribe to topics once connected
        mqttClient->subscribe(QMqttTopicFilter("sensors/basilic/temperature"));
        mqttClient->subscribe(QMqttTopicFilter("sensors/basilic/humidity"));
        mqttClient->subscribe(QMqttTopicFilter("sensors/basilic/light"));
        mqttClient->subscribe(QMqttTopicFilter("sensors/basilic/battery"));
        mqttClient->subscribe(QMqttTopicFilter("irrigation/status"));
        break;
    }
}


void ValueMQTT::setMqttValues(const QString &values)
{
    statusLabel->setText(values);
}

void ValueMQTT::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString msg = QString::fromUtf8(message);
    bool ok = true;
    float value = 0.0;

    // Only try to convert to float for sensor data
    if (topic.name().startsWith("sensors/")) {
        value = msg.toFloat(&ok);
        if (!ok) {
            qWarning() << "Invalid number received:" << msg << "for topic:" << topic.name();
            return;
        }
    }

    if (topic.name() == "sensors/basilic/temperature") {
        tempLabel->setText("Temperature: " + msg + " °C");
        tempSeries->append(dataPointCounter, value);
    } else if (topic.name() == "sensors/basilic/humidity") {
        humLabel->setText("Humidity: " + msg + " %");
        humSeries->append(dataPointCounter, value);
    } else if (topic.name() == "sensors/basilic/light") {
        lightLabel->setText("Light: " + msg + " %");
        lightSeries->append(dataPointCounter, value);
    } else if (topic.name() == "sensors/basilic/battery") {
        batteryLabel->setText("Battery: " + msg + " %");
        batterySeries->append(dataPointCounter, value);
    } else if (topic.name() == "irrigation/status") {
        statusLabel->setText("Status: " + msg);
    }

    if (topic.name().startsWith("sensors/")) {
        dataPointCounter++;

        // Update X axis range to show all data points
        if (dataPointCounter > 100) {
            axisX->setRange(dataPointCounter - 100, dataPointCounter);
        }

        // Update chart
        chart->update();
    }
}

void ValueMQTT::onMqttError(QMqttClient::ClientError error)
{
    QString errorMsg;
    switch(error) {
    case QMqttClient::NoError:
        errorMsg = "No error";
        break;
    case QMqttClient::InvalidProtocolVersion:
        errorMsg = "Invalid protocol version";
        break;
    case QMqttClient::IdRejected:
        errorMsg = "Client ID rejected";
        break;
    case QMqttClient::ServerUnavailable:
        errorMsg = "Server unavailable";
        break;
    case QMqttClient::BadUsernameOrPassword:
        errorMsg = "Bad username or password";
        break;
    case QMqttClient::NotAuthorized:
        errorMsg = "Not authorized";
        break;
    case QMqttClient::TransportInvalid:
        errorMsg = "Transport invalid";
        break;
    case QMqttClient::ProtocolViolation:
        errorMsg = "Protocol violation";
        break;
    case QMqttClient::UnknownError:
        errorMsg = "Unknown error";
        break;
    default:
        errorMsg = QString("Error code: %1").arg(static_cast<int>(error));
    }

    statusLabel->setText("Error: " + errorMsg);
    QMessageBox::warning(this, "MQTT Error", "Connection error: " + errorMsg);
}

void ValueMQTT::startIrrigation()
{
    if (mqttClient->state() == QMqttClient::Connected) {
        QMqttTopicName topic("irrigation/control");
        mqttClient->publish(topic, "start", 1, false);
        statusLabel->setText("Status: Starting irrigation...");
    } else {
        QMessageBox::warning(this, "MQTT Error", "Not connected to broker. Cannot start irrigation.");
    }
}

void ValueMQTT::stopIrrigation()
{
    if (mqttClient->state() == QMqttClient::Connected) {
        QMqttTopicName topic("irrigation/control");
        mqttClient->publish(topic, "stop", 1, false);
        statusLabel->setText("Status: Stopping irrigation...");
    } else {
        QMessageBox::warning(this, "MQTT Error", "Not connected to broker. Cannot stop irrigation.");
    }
}
