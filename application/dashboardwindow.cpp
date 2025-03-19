#include "dashboardwindow.h"
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QPixmap>
#include <QPainter>
#include <QDockWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStyle>
#include <QDebug>

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent), alertsWindow(nullptr) {
    showFullScreen();
    setupCustomTitleBar();
    setupLeftSidebar();
    setupMainSection();
    setupRightPanel();
    applyStyles();
    startAnimations();
}

DashboardWindow::~DashboardWindow() {
    delete alertsWindow;
}

void DashboardWindow::setupCustomTitleBar() {
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    QHBoxLayout *titleBarLayout = new QHBoxLayout(titleBar);
    titleBarLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *titleLabel = new QLabel("Smart Irrigation Dashboard", titleBar);
    titleBarLayout->addWidget(titleLabel);

    titleBarLayout->addStretch();

    QToolButton *closeButton = new QToolButton(titleBar);
    closeButton->setText("X");
    closeButton->setStyleSheet("background-color: red; color: white; border: none; font-weight: bold;");
    closeButton->setFixedSize(30, 30);
    connect(closeButton, &QToolButton::clicked, this, &QMainWindow::close);

    titleBarLayout->addWidget(closeButton);

    setMenuWidget(titleBar);
}

void DashboardWindow::setupLeftSidebar() {
    leftSidebar = new QWidget(this);
    leftSidebar->setFixedWidth(240);
    sidebarLayout = new QVBoxLayout(leftSidebar);

    logoLabel = new QLabel("Smart Irrigation", leftSidebar);
    logoLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #FFFFFF; margin-top: 30px; margin-left: 20px;");
    sidebarLayout->addWidget(logoLabel);

    myParcelsButton = new QPushButton("🌱 Mes Parcelles", leftSidebar);
    waterConsumptionButton = new QPushButton("💧 Consommation d’eau", leftSidebar);
    settingsButton = new QPushButton("⚙ Paramètres", leftSidebar);
    alertsButton = new QPushButton("🚨 Alertes", leftSidebar);
    connect(alertsButton, &QPushButton::clicked, this, &DashboardWindow::displayAlerts);

    sidebarLayout->addWidget(myParcelsButton);
    sidebarLayout->addWidget(waterConsumptionButton);
    sidebarLayout->addWidget(settingsButton);
    sidebarLayout->addWidget(alertsButton);

    QPushButton *startIrrigationButton = new QPushButton("🚿 Lancer l'irrigation", leftSidebar);
    QPushButton *viewDetailsButton = new QPushButton("🔍 Voir les détails", leftSidebar);
    sidebarLayout->addWidget(startIrrigationButton);
    sidebarLayout->addWidget(viewDetailsButton);


    QDockWidget *sidebarDock = new QDockWidget("Sidebar", this);
    sidebarDock->setWidget(leftSidebar);
    sidebarDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);
}

void DashboardWindow::setupMainSection()
{
    mainSection = new QWidget(this);
    mainLayout = new QVBoxLayout(mainSection);

    // Search Bar
    searchBar = new QLineEdit(mainSection);
    searchBar->setPlaceholderText("Rechercher une parcelle...");
    searchBar->setMinimumSize(200, 40);
    mainLayout->addWidget(searchBar);

    // Quick Access Section
    quickAccessSection = new QWidget(mainSection);
    quickAccessLayout = new QHBoxLayout(quickAccessSection);

    QLabel *basilicCard = createQuickAccessCard("🌿 Basilic", "45%", "3h restantes");
    QLabel *pommierCard = createQuickAccessCard("🍏 Pommier", "50%", "2h restantes");
    QLabel *orangerieCard = createQuickAccessCard("🍊 Orangerie", "30%", "4h restantes");

    quickAccessLayout->addWidget(basilicCard);
    quickAccessLayout->addWidget(pommierCard);
    quickAccessLayout->addWidget(orangerieCard);

    mainLayout->addWidget(quickAccessSection);

    // Parcel Details Table
    parcelDetailsTable = new QTableWidget(mainSection);
    parcelDetailsTable->setColumnCount(7);
    parcelDetailsTable->setHorizontalHeaderLabels({"Parcelle", "Dernière Irrigation", "Consommation d’eau", "État", "Humidité du sol", "Niveau de nutriments", "Détails d'alerte"});
    parcelDetailsTable->setRowCount(3);
    parcelDetailsTable->setItem(0, 0, new QTableWidgetItem("🌿 Basilic"));
    parcelDetailsTable->setItem(0, 1, new QTableWidgetItem("2023-10-01 10:00"));
    parcelDetailsTable->setItem(0, 2, new QTableWidgetItem("40L"));
    parcelDetailsTable->setItem(0, 3, new QTableWidgetItem("🟢 OK"));
    parcelDetailsTable->setItem(0, 4, new QTableWidgetItem("60%"));
    parcelDetailsTable->setItem(0, 5, new QTableWidgetItem("Normal"));
    parcelDetailsTable->setItem(0, 6, new QTableWidgetItem("Aucune alerte"));
    parcelDetailsTable->setItem(1, 0, new QTableWidgetItem("🍏 Pommier"));
    parcelDetailsTable->setItem(1, 1, new QTableWidgetItem("2023-10-01 09:00"));
    parcelDetailsTable->setItem(1, 2, new QTableWidgetItem("50L"));
    parcelDetailsTable->setItem(1, 3, new QTableWidgetItem("🟡 Attention"));
    parcelDetailsTable->setItem(1, 4, new QTableWidgetItem("45%"));
    parcelDetailsTable->setItem(1, 5, new QTableWidgetItem("Faible"));
    parcelDetailsTable->setItem(1, 6, new QTableWidgetItem("Arrosage en retard de 2h"));
    parcelDetailsTable->setItem(2, 0, new QTableWidgetItem("🍊 Orangerie"));
    parcelDetailsTable->setItem(2, 1, new QTableWidgetItem("2023-10-01 08:00"));
    parcelDetailsTable->setItem(2, 2, new QTableWidgetItem("30L"));
    parcelDetailsTable->setItem(2, 3, new QTableWidgetItem("🔴 Problème"));
    parcelDetailsTable->setItem(2, 4, new QTableWidgetItem("30%"));
    parcelDetailsTable->setItem(2, 5, new QTableWidgetItem("Très faible"));
    parcelDetailsTable->setItem(2, 6, new QTableWidgetItem("Manque d'eau détecté"));

    mainLayout->addWidget(parcelDetailsTable);

    // Chart View for Watering Progress
    QChartView *weeklyProgressChartView = new QChartView(mainSection);
    weeklyProgressChartView->setRenderHint(QPainter::Antialiasing);

    QChart *weeklyProgressChart = new QChart();
    weeklyProgressChart->setTitle("Weekly Watering Progress");

    QLineSeries *basilicSeries = new QLineSeries();
    basilicSeries->setName("🌿 Basilic");
    basilicSeries->append(1, 5);
    basilicSeries->append(2, 15);
    basilicSeries->append(3, 25);
    basilicSeries->append(4, 35);
    basilicSeries->append(5, 45);
    basilicSeries->append(6, 55);
    basilicSeries->append(7, 65);

    QLineSeries *pommierSeries = new QLineSeries();
    pommierSeries->setName("🍏 Pommier");
    pommierSeries->append(1, 10);
    pommierSeries->append(2, 20);
    pommierSeries->append(3, 30);
    pommierSeries->append(4, 40);
    pommierSeries->append(5, 50);
    pommierSeries->append(6, 60);
    pommierSeries->append(7, 70);

    QLineSeries *orangerieSeries = new QLineSeries();
    orangerieSeries->setName("🍊 Orangerie");
    orangerieSeries->append(1, 15);
    orangerieSeries->append(2, 25);
    orangerieSeries->append(3, 35);
    orangerieSeries->append(4, 45);
    orangerieSeries->append(5, 55);
    orangerieSeries->append(6, 65);
    orangerieSeries->append(7, 75);

    weeklyProgressChart->addSeries(basilicSeries);
    weeklyProgressChart->addSeries(pommierSeries);
    weeklyProgressChart->addSeries(orangerieSeries);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Day");
    axisX->setRange(1, 7);
    weeklyProgressChart->addAxis(axisX, Qt::AlignBottom);
    basilicSeries->attachAxis(axisX);
    pommierSeries->attachAxis(axisX);
    orangerieSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Water Usage (L)");
    axisY->setRange(0, 100);
    weeklyProgressChart->addAxis(axisY, Qt::AlignLeft);
    basilicSeries->attachAxis(axisY);
    pommierSeries->attachAxis(axisY);
    orangerieSeries->attachAxis(axisY);

    weeklyProgressChartView->setChart(weeklyProgressChart);
    weeklyProgressChartView->setMinimumSize(740, 300); // Set minimum size for the chart

    mainLayout->addWidget(weeklyProgressChartView);

    // Add stretch to ensure the chart stays at the bottom
    mainLayout->addStretch();

    setCentralWidget(mainSection);
}
QLabel* DashboardWindow::createQuickAccessCard(const QString &title, const QString &waterUsage, const QString &timeRemaining)
{
    QLabel *card = new QLabel(quickAccessSection);
    card->setText(QString("%1\n💧 %2\n⏳ %3").arg(title, waterUsage, timeRemaining));
    card->setFixedSize(200, 150);
    card->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; padding: 10px; border: 1px solid #CCCCCC; font-size: 14px; color: #333333;");
    card->setAlignment(Qt::AlignCenter);
    return card;
}


void DashboardWindow::setupRightPanel() {
    rightPanel = new QWidget(this);
    rightPanel->setFixedWidth(240);
    rightPanelLayout = new QVBoxLayout(rightPanel);

    QLabel *profilePictureLabel = new QLabel(rightPanel);
    QPixmap profilePixmap(":/images/profil.png");
    profilePixmap = profilePixmap.scaled(80, 80, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap circularPixmap(80, 80);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(profilePixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 80, 80);
    profilePictureLabel->setPixmap(circularPixmap);

    rightPanelLayout->addWidget(profilePictureLabel, 0, Qt::AlignCenter);

    userAvatarLabel = new QLabel("Bonjour, Imane", rightPanel);
    userAvatarLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333; margin-top: 10px;");
    rightPanelLayout->addWidget(userAvatarLabel, 0, Qt::AlignCenter);

    waterUsageLabel = new QLabel("120L sur 200L utilisés", rightPanel);
    waterUsageProgressBar = new QProgressBar(rightPanel);
    waterUsageProgressBar->setValue(60);

    rightPanelLayout->addWidget(waterUsageLabel);
    rightPanelLayout->addWidget(waterUsageProgressBar);

    waterUsageDetailsLabel = new QLabel("🌿 Basilic → 40L\n🍏 Pommier → 50L\n🍊 Orangerie → 30L", rightPanel);
    rightPanelLayout->addWidget(waterUsageDetailsLabel);

    alertsLabel = new QLabel("🔴 Orangerie : manque d’eau détecté !\n🟠 Pommier : arrosage en retard de 2h\n🟢 Basilic : état optimal", rightPanel);
    rightPanelLayout->addWidget(alertsLabel);

    QDockWidget *rightPanelDock = new QDockWidget("Right Panel", this);
    rightPanelDock->setWidget(rightPanel);
    rightPanelDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, rightPanelDock);
}

void DashboardWindow::applyStyles() {
    setStyleSheet("background-color: #F5F5F5; color: #333333;");

    leftSidebar->setStyleSheet("background-color: rgba(45, 156, 104, 0.8); border-radius: 10px;");
    myParcelsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    waterConsumptionButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    settingsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    alertsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");

    searchBar->setStyleSheet("font-size: 14px; color: #333333; background-color: #FFFFFF; border-radius: 5px; padding: 5px; border: 1px solid #CCCCCC;");
    quickAccessSection->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; border: 1px solid #CCCCCC;");
    parcelDetailsTable->setStyleSheet(
        "background-color: #FFFFFF; border-radius: 10px; font-size: 14px; color: #333333;"
        "QHeaderView::section { background-color: #57C78F; color: #FFFFFF; padding: 5px; }"
        "QTableWidget::item { padding: 5px; }"
        );

    rightPanel->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; border: 1px solid #CCCCCC;");
    waterUsageProgressBar->setStyleSheet(
        "QProgressBar { background-color: #E0E0E0; border: 1px solid #CCCCCC; border-radius: 5px; text-align: center; color: #333333; }"
        "QProgressBar::chunk { background-color: #57C78F; }"
        );

    waterUsageLabel->setStyleSheet("font-size: 14px; color: #333333;");
    waterUsageDetailsLabel->setStyleSheet("font-size: 14px; color: #333333;");
    alertsLabel->setStyleSheet("font-size: 14px; color: #333333;");
    userAvatarLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333; margin-top: 10px;");
}

void DashboardWindow::startAnimations() {
    QPropertyAnimation *animation = new QPropertyAnimation(myParcelsButton, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(myParcelsButton->geometry());
    animation->setEndValue(myParcelsButton->geometry().adjusted(-5, -5, 5, 5));
    animation->setLoopCount(-1);
    animation->start();
}

void DashboardWindow::updateGraph(const QString &culture, const QVector<QPointF> &data)
{
    QLineSeries *series = new QLineSeries();
    series->setName(culture + " Watering Progress");

    for (const QPointF &point : data) {
        series->append(point);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Weekly Watering Progress for " + culture);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Day");
    axisX->setRange(1, 7);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Water Usage (L)");
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView->setChart(chart);
    chartView->setFixedSize(600, 400);
    chartView->update();
}

void DashboardWindow::showPommierGraph()
{
    QVector<QPointF> data = {
        {1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}, {6, 60}, {7, 70}
    };
    updateGraph("Pommier", data);
}

void DashboardWindow::showBasilicGraph()
{
    QVector<QPointF> data = {
        {1, 5}, {2, 15}, {3, 25}, {4, 35}, {5, 45}, {6, 55}, {7, 65}
    };
    updateGraph("Basilic", data);
}

void DashboardWindow::showOrangerieGraph()
{
    QVector<QPointF> data = {
        {1, 15}, {2, 25}, {3, 35}, {4, 45}, {5, 55}, {6, 65}, {7, 75}
    };
    updateGraph("Orangerie", data);
}

void DashboardWindow::displayAlerts() {
    if (!alertsWindow) {
        alertsWindow = new AlertsWindow(this);
    }

    QStringList alerts = {
        "⚠️ Le réservoir est presque vide !",
        "🦠 Les arbres de pommier sont suspectés d'avoir la bactérie amylo.",
        "🌧️ Aucune pluie prévue dans les 7 prochains jours.",
        "🚰 La consommation d'eau est supérieure à la moyenne.",
        "🌡️ Température élevée détectée dans la parcelle de basilic."
    };

    alertsWindow->setAlerts(alerts);
    alertsWindow->showFullScreen();
}

QLabel* DashboardWindow::createQuickAccessCard(const QString &title, const QString &waterUsage, const QString &timeRemaining) {
    QLabel *card = new QLabel();
    card->setText(QString("%1\n💧 %2\n⏳ %3").arg(title, waterUsage, timeRemaining));
    card->setFixedSize(200, 150);
    card->setStyleSheet("background-color: #FFFFFF; border-radius: 10px; padding: 10px; border: 1px solid #CCCCCC; font-size: 14px; color: #333333;");
    card->setAlignment(Qt::AlignCenter);
    return card;
}

// AlertsWindow Implementation
AlertsWindow::AlertsWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Alertes");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setStyleSheet("background-color: #FFFFFF;");

    alertsContainer = new QWidget(this);
    alertsContainer->setLayout(new QVBoxLayout());
    alertsContainer->layout()->setAlignment(Qt::AlignTop);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(alertsContainer);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);

    QPushButton *closeButton = new QPushButton("Fermer", this);
    closeButton->setStyleSheet("background-color: #FF0000; color: #FFFFFF; padding: 10px;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);
}

void AlertsWindow::setAlerts(const QStringList &alerts) {
    QLayoutItem* item;
    while ((item = alertsContainer->layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    for (const QString &alert : alerts) {
        QLabel *alertLabel = new QLabel(alert, alertsContainer);
        alertLabel->setStyleSheet("background-color: #FFCCCC; border-radius: 5px; padding: 10px; margin: 5px;");
        alertLabel->setWordWrap(true);
        alertsContainer->layout()->addWidget(alertLabel);
    }
}
