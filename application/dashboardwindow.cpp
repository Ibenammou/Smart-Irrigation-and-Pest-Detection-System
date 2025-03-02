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

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window size
    setFixedSize(1200, 800);

    // Set up the UI
    setupLeftSidebar();
    setupMainSection();
    setupRightPanel();

    // Apply styles
    applyStyles();

    // Start animations
    startAnimations();
}

DashboardWindow::~DashboardWindow()
{
    // Clean up dynamically allocated objects if necessary
}

void DashboardWindow::setupLeftSidebar()
{
    // Left Sidebar
    leftSidebar = new QWidget(this);
    leftSidebar->setFixedWidth(240);
    sidebarLayout = new QVBoxLayout(leftSidebar);

    // Logo
    logoLabel = new QLabel("Smart Irrigation", leftSidebar);
    logoLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #FFFFFF; margin-top: 30px; margin-left: 20px;");
    sidebarLayout->addWidget(logoLabel);

    // Menu Buttons
    myParcelsButton = new QPushButton("🌱 Mes Parcelles", leftSidebar);
    waterConsumptionButton = new QPushButton("💧 Consommation d’eau", leftSidebar);
    settingsButton = new QPushButton("⚙ Paramètres", leftSidebar);
    alertsButton = new QPushButton("🚨 Alertes", leftSidebar);

    sidebarLayout->addWidget(myParcelsButton);
    sidebarLayout->addWidget(waterConsumptionButton);
    sidebarLayout->addWidget(settingsButton);
    sidebarLayout->addWidget(alertsButton);

    // Add the left sidebar to the main window
    QDockWidget *sidebarDock = new QDockWidget("Sidebar", this);
    sidebarDock->setWidget(leftSidebar);
    sidebarDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // Disable floating and closing
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);
}

void DashboardWindow::setupMainSection()
{
    // Main Section
    mainSection = new QWidget(this);
    mainLayout = new QVBoxLayout(mainSection);

    // Search Bar
    searchBar = new QLineEdit(mainSection);
    searchBar->setPlaceholderText("Rechercher une parcelle...");
    searchBar->setFixedSize(600, 40);
    mainLayout->addWidget(searchBar);

    // Quick Access Section
    quickAccessSection = new QWidget(mainSection);
    quickAccessLayout = new QHBoxLayout(quickAccessSection);

    // Quick Access Cards
    QLabel *basilicCard = new QLabel("🌿 Basilic\n💧 45%\n⏳ 3h restantes", quickAccessSection);
    QLabel *pommierCard = new QLabel("🍏 Pommier\n💧 50%\n⏳ 2h restantes", quickAccessSection);
    QLabel *orangerieCard = new QLabel("🍊 Orangerie\n💧 30%\n⏳ 4h restantes", quickAccessSection);

    basilicCard->setFixedSize(200, 150);
    pommierCard->setFixedSize(200, 150);
    orangerieCard->setFixedSize(200, 150);

    quickAccessLayout->addWidget(basilicCard);
    quickAccessLayout->addWidget(pommierCard);
    quickAccessLayout->addWidget(orangerieCard);

    mainLayout->addWidget(quickAccessSection);

    // Parcel Details Table
    parcelDetailsTable = new QTableWidget(mainSection);
    parcelDetailsTable->setColumnCount(4);
    parcelDetailsTable->setHorizontalHeaderLabels({"Parcelle", "Dernière Irrigation", "Consommation d’eau", "État"});
    parcelDetailsTable->setRowCount(3);
    parcelDetailsTable->setItem(0, 0, new QTableWidgetItem("🌿 Basilic"));
    parcelDetailsTable->setItem(0, 1, new QTableWidgetItem("2023-10-01 10:00"));
    parcelDetailsTable->setItem(0, 2, new QTableWidgetItem("40L"));
    parcelDetailsTable->setItem(0, 3, new QTableWidgetItem("🟢 OK"));
    parcelDetailsTable->setItem(1, 0, new QTableWidgetItem("🍏 Pommier"));
    parcelDetailsTable->setItem(1, 1, new QTableWidgetItem("2023-10-01 09:00"));
    parcelDetailsTable->setItem(1, 2, new QTableWidgetItem("50L"));
    parcelDetailsTable->setItem(1, 3, new QTableWidgetItem("🟡 Attention"));
    parcelDetailsTable->setItem(2, 0, new QTableWidgetItem("🍊 Orangerie"));
    parcelDetailsTable->setItem(2, 1, new QTableWidgetItem("2023-10-01 08:00"));
    parcelDetailsTable->setItem(2, 2, new QTableWidgetItem("30L"));
    parcelDetailsTable->setItem(2, 3, new QTableWidgetItem("🔴 Problème"));

    mainLayout->addWidget(parcelDetailsTable);

    // Add the main section to the main window
    setCentralWidget(mainSection);
}

void DashboardWindow::setupRightPanel()
{
    // Right Panel
    rightPanel = new QWidget(this);
    rightPanel->setFixedWidth(240);
    rightPanelLayout = new QVBoxLayout(rightPanel);

    // User Avatar (Circular Profile Picture)
    QLabel *profilePictureLabel = new QLabel(rightPanel);
    QPixmap profilePixmap(":/images/profil.png"); // Load the profile image from resources
    profilePixmap = profilePixmap.scaled(80, 80, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // Create a circular mask
    QPixmap circularPixmap(80, 80);
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(profilePixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, 80, 80);
    profilePictureLabel->setPixmap(circularPixmap);

    // Add the profile picture to the layout
    rightPanelLayout->addWidget(profilePictureLabel, 0, Qt::AlignCenter);

    // User Greeting
    userAvatarLabel = new QLabel("Bonjour, Imane", rightPanel);
    userAvatarLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3A3A3A; margin-top: 10px;");
    rightPanelLayout->addWidget(userAvatarLabel, 0, Qt::AlignCenter);

    // Water Usage Progress Bar
    waterUsageLabel = new QLabel("120L sur 200L utilisés", rightPanel);
    waterUsageProgressBar = new QProgressBar(rightPanel);
    waterUsageProgressBar->setValue(60); // 120L / 200L

    rightPanelLayout->addWidget(waterUsageLabel);
    rightPanelLayout->addWidget(waterUsageProgressBar);

    // Water Usage Details
    waterUsageDetailsLabel = new QLabel("🌿 Basilic → 40L\n🍏 Pommier → 50L\n🍊 Orangerie → 30L", rightPanel);
    rightPanelLayout->addWidget(waterUsageDetailsLabel);

    // Alerts
    alertsLabel = new QLabel("🔴 Orangerie : manque d’eau détecté !\n🟠 Pommier : arrosage en retard de 2h\n🟢 Basilic : état optimal", rightPanel);
    rightPanelLayout->addWidget(alertsLabel);

    // Add the right panel to the main window
    QDockWidget *rightPanelDock = new QDockWidget("Right Panel", this);
    rightPanelDock->setWidget(rightPanel);
    rightPanelDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // Disable floating and closing
    addDockWidget(Qt::RightDockWidgetArea, rightPanelDock);
}

void DashboardWindow::applyStyles()
{
    // Set background color
    setStyleSheet("background-color: #1E1E1E; color: #FFFFFF;");

    // Left Sidebar Styles
    leftSidebar->setStyleSheet("background-color: rgba(45, 156, 104, 0.8); border-radius: 10px;");
    myParcelsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    waterConsumptionButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    settingsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");
    alertsButton->setStyleSheet("background-color: #57C78F; color: #FFFFFF; border-radius: 10px; padding: 10px;");

    // Main Section Styles
    searchBar->setStyleSheet("font-size: 14px; color: #3A3A3A; background-color: #FFFFFF; border-radius: 5px; padding: 5px;");
    quickAccessSection->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); border-radius: 10px;");
    parcelDetailsTable->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); border-radius: 10px; font-size: 14px; color: #FFFFFF;");

    // Right Panel Styles
    rightPanel->setStyleSheet("background-color: rgba(255, 255, 255, 0.1); border-radius: 10px;");
    waterUsageProgressBar->setStyleSheet("QProgressBar { border: 1px solid #444; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #57C78F; }");
}

void DashboardWindow::startAnimations()
{
    // Example: Pulse animation for buttons
    QPropertyAnimation *animation = new QPropertyAnimation(myParcelsButton, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(myParcelsButton->geometry());
    animation->setEndValue(myParcelsButton->geometry().adjusted(-5, -5, 5, 5));
    animation->setLoopCount(-1); // Infinite loop
    animation->start();
}
