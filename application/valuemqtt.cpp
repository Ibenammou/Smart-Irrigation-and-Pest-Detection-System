#include "valuemqtt.h"
#include <QMessageBox>
#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QLineSeries>
#include <QMqttTopicName>
#include <QMqttTopicFilter>
#include <QPushButton>
#include <QDebug>

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
    // Set window to full screen
    setWindowState(Qt::WindowFullScreen);

    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(20, 20, 20, 20);
    setCentralWidget(centralWidget);

    // Create exit button with red style
    QPushButton *exitButton = new QPushButton("X", centralWidget);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ff0000;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 16px;"
        "   border: none;"
        "   min-width: 40px;"
        "   max-width: 40px;"
        "   min-height: 40px;"
        "   max-height: 40px;"
        "   border-radius: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff3333;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #cc0000;"
        "}"
        );
    connect(exitButton, &QPushButton::clicked, this, &ValueMQTT::close);

    // Create top bar layout with exit button
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->addStretch();
    topBarLayout->addWidget(exitButton);

    // Initialize labels with larger font
    QFont labelFont;
    labelFont.setPointSize(16);

    tempLabel = new QLabel("Temperature: -- °C", centralWidget);
    tempLabel->setFont(labelFont);
    humLabel = new QLabel("Humidity: -- %", centralWidget);
    humLabel->setFont(labelFont);
    lightLabel = new QLabel("Light: -- %", centralWidget);
    lightLabel->setFont(labelFont);
    batteryLabel = new QLabel("Battery: -- %", centralWidget);
    batteryLabel->setFont(labelFont);
    statusLabel = new QLabel("Status: Ready", centralWidget);
    statusLabel->setFont(labelFont);

    // Create sensor data layout
    QHBoxLayout *sensorLayout = new QHBoxLayout();
    sensorLayout->addWidget(tempLabel);
    sensorLayout->addWidget(humLabel);
    sensorLayout->addWidget(lightLabel);
    sensorLayout->addWidget(batteryLabel);
    sensorLayout->addStretch();

    // Initialize chart with larger fonts
    chart = new QChart();
    chart->setTitle("Sensor Data");
    QFont chartFont;
    chartFont.setPointSize(14);
    chart->setTitleFont(chartFont);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

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
    axisX->setTitleText("Time (samples)");
    axisX->setRange(0, 100);
    axisX->setTitleFont(chartFont);
    axisX->setLabelsFont(chartFont);
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis();
    axisY->setTitleText("Value");
    axisY->setRange(0, 100);
    axisY->setTitleFont(chartFont);
    axisY->setLabelsFont(chartFont);
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

    // Initialize control buttons with larger size
    startIrrigationButton = new QPushButton("Start Irrigation", centralWidget);
    stopIrrigationButton = new QPushButton("Stop Irrigation", centralWidget);

    QFont buttonFont;
    buttonFont.setPointSize(16);
    startIrrigationButton->setFont(buttonFont);
    stopIrrigationButton->setFont(buttonFont);
    startIrrigationButton->setMinimumHeight(50);
    stopIrrigationButton->setMinimumHeight(50);

    // Style buttons
    QString buttonStyle = "QPushButton {"
                          "   background-color: #4CAF50;"
                          "   color: white;"
                          "   border: none;"
                          "   border-radius: 5px;"
                          "   padding: 10px;"
                          "}"
                          "QPushButton:hover {"
                          "   background-color: #45a049;"
                          "}"
                          "QPushButton:pressed {"
                          "   background-color: #3d8b40;"
                          "}";

    startIrrigationButton->setStyleSheet(buttonStyle);
    stopIrrigationButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d32f2f;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #b71c1c;"
        "}"
        );

    connect(startIrrigationButton, &QPushButton::clicked, this, &ValueMQTT::startIrrigation);
    connect(stopIrrigationButton, &QPushButton::clicked, this, &ValueMQTT::stopIrrigation);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startIrrigationButton);
    buttonLayout->addWidget(stopIrrigationButton);

    // Add pump power control slider
    QVBoxLayout *sliderLayout = new QVBoxLayout();

    QHBoxLayout *sliderHeaderLayout = new QHBoxLayout();
    QLabel *sliderTitle = new QLabel("Pump Power:", centralWidget);
    sliderTitle->setFont(labelFont);
    pumpPowerLabel = new QLabel("50%", centralWidget);
    pumpPowerLabel->setFont(labelFont);
    sliderHeaderLayout->addWidget(sliderTitle);
    sliderHeaderLayout->addWidget(pumpPowerLabel);
    sliderHeaderLayout->addStretch();

    pumpPowerSlider = new QSlider(Qt::Horizontal, centralWidget);
    pumpPowerSlider->setMinimum(20);  // Minimum power 20%
    pumpPowerSlider->setMaximum(100); // Maximum power 100%
    pumpPowerSlider->setValue(50);    // Default to 50%
    pumpPowerSlider->setTickPosition(QSlider::TicksBelow);
    pumpPowerSlider->setTickInterval(10);
    pumpPowerSlider->setMinimumHeight(40);

    // Style the slider
    pumpPowerSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999999;"
        "    height: 10px;"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B1B1B1, stop:1 #c4c4c4);"
        "    margin: 2px 0;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #45a049, stop:1 #3d8b40);"
        "    border: 1px solid #5c5c5c;"
        "    width: 20px;"
        "    margin: -8px 0;"
        "    border-radius: 10px;"
        "}"
        );

    connect(pumpPowerSlider, &QSlider::valueChanged, this, &ValueMQTT::updatePumpPowerLabel);

    sliderLayout->addLayout(sliderHeaderLayout);
    sliderLayout->addWidget(pumpPowerSlider);

    // Set current power
    currentPumpPower = pumpPowerSlider->value();

    // Add widgets to layout
    layout->addLayout(topBarLayout); // Add the top bar with exit button first
    layout->addLayout(sensorLayout);
    layout->addWidget(statusLabel);
    layout->addLayout(buttonLayout);
    layout->addLayout(sliderLayout);
    layout->addWidget(chartView, 1); // The '1' makes the chart expand to fill available space
}

void ValueMQTT::initMQTT()
{
    mqttClient = new QMqttClient(this);
    mqttClient->setHostname("192.168.1.170");
    mqttClient->setPort(1883);
    mqttClient->setClientId("QtIrrigationControl");
    mqttClient->setKeepAlive(60);

    connect(mqttClient, &QMqttClient::messageReceived,
            this, &ValueMQTT::onMessageReceived);
    connect(mqttClient, &QMqttClient::errorChanged,
            this, &ValueMQTT::onMqttError);
    connect(mqttClient, &QMqttClient::stateChanged,
            this, &ValueMQTT::onMqttStateChanged);

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
        if (dataPointCounter > 100) {
            axisX->setRange(dataPointCounter - 100, dataPointCounter);
        }
        chart->update();
    }
}

void ValueMQTT::onMqttError(QMqttClient::ClientError error)
{
    QString errorMsg;
    switch(error) {
    case QMqttClient::NoError: errorMsg = "No error"; break;
    case QMqttClient::InvalidProtocolVersion: errorMsg = "Invalid protocol version"; break;
    case QMqttClient::IdRejected: errorMsg = "Client ID rejected"; break;
    case QMqttClient::ServerUnavailable: errorMsg = "Server unavailable"; break;
    case QMqttClient::BadUsernameOrPassword: errorMsg = "Bad username or password"; break;
    case QMqttClient::NotAuthorized: errorMsg = "Not authorized"; break;
    case QMqttClient::TransportInvalid: errorMsg = "Transport invalid"; break;
    case QMqttClient::ProtocolViolation: errorMsg = "Protocol violation"; break;
    case QMqttClient::UnknownError: errorMsg = "Unknown error"; break;
    default: errorMsg = QString("Error code: %1").arg(static_cast<int>(error));
    }

    statusLabel->setText("Error: " + errorMsg);
    QMessageBox::warning(this, "MQTT Error", "Connection error: " + errorMsg);
}

void ValueMQTT::updatePumpPowerLabel(int value)
{
    currentPumpPower = value;
    pumpPowerLabel->setText(QString("%1%").arg(value));

    // If irrigation is already active, update the power level
    if (statusLabel->text().contains("Starting irrigation") ||
        statusLabel->text().contains("Irrigation active")) {
        setPumpPower(value);
    }
}

void ValueMQTT::setPumpPower(int value)
{
    if (mqttClient->state() == QMqttClient::Connected) {
        QMqttTopicName topic("irrigation/power");
        mqttClient->publish(topic, QString::number(value).toUtf8(), 1, false);
    }
}

void ValueMQTT::startIrrigation()
{
    if (mqttClient->state() == QMqttClient::Connected) {
        // First publish the power level
        QMqttTopicName powerTopic("irrigation/power");
        mqttClient->publish(powerTopic, QString::number(currentPumpPower).toUtf8(), 1, false);

        // Then start irrigation
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
