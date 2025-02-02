#include "FarmDashboard.h"

FarmDashboard::FarmDashboard(QWidget *parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget;

    // Ajoute les pages au système de navigation
    stackedWidget->addWidget(createInputPage());
    stackedWidget->addWidget(createDashboardPage());

    setCentralWidget(stackedWidget);
    setWindowTitle("SmartAgrico - Dashboard");
    resize(800, 600);
}

FarmDashboard::~FarmDashboard() {}

// 📝 Page 1 : Formulaire de saisie des données agricoles
QWidget* FarmDashboard::createInputPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *title = new QLabel("Bienvenue sur SmartAgrico !");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");

    nameInput = new QLineEdit;
    farmSizeInput = new QLineEdit;
    cropTypeInput = new QLineEdit;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Nom de l'agriculteur :", nameInput);
    formLayout->addRow("Taille de la ferme (hectares) :", farmSizeInput);
    formLayout->addRow("Type de culture :", cropTypeInput);

    QPushButton *submitButton = new QPushButton("Valider");
    connect(submitButton, &QPushButton::clicked, this, &FarmDashboard::showDashboard);

    layout->addWidget(title);
    layout->addLayout(formLayout);
    layout->addWidget(submitButton);
    layout->addStretch();

    return page;
}

// 🏡 Page 2 : Tableau de bord agricole
QWidget* FarmDashboard::createDashboardPage() {
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *title = new QLabel("Tableau de bord - État de la ferme");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");

    alertsTable = new QTableWidget(5, 2);
    alertsTable->setHorizontalHeaderLabels({"Paramètre", "Valeur"});
    alertsTable->setItem(0, 0, new QTableWidgetItem("Température"));
    alertsTable->setItem(0, 1, new QTableWidgetItem("25°C"));
    alertsTable->setItem(1, 0, new QTableWidgetItem("Humidité du sol"));
    alertsTable->setItem(1, 1, new QTableWidgetItem("40%"));
    alertsTable->setItem(2, 0, new QTableWidgetItem("mhm chi haja"));
    alertsTable->setItem(2, 1, new QTableWidgetItem("Bonne"));
    alertsTable->setItem(3, 0, new QTableWidgetItem("Prochain arrosage"));
    alertsTable->setItem(3, 1, new QTableWidgetItem("Demain à 6h"));
    alertsTable->setItem(4, 0, new QTableWidgetItem("Recommandation"));
    alertsTable->setItem(4, 1, new QTableWidgetItem("Surveillez l'humidité"));

    QPushButton *backButton = new QPushButton("Retour");
    connect(backButton, &QPushButton::clicked, [=]() { stackedWidget->setCurrentIndex(0); });

    layout->addWidget(title);
    layout->addWidget(alertsTable);
    layout->addWidget(backButton);
    layout->addStretch();

    return page;
}

// 🔄 Changer de page après la validation du formulaire
void FarmDashboard::showDashboard() {
    stackedWidget->setCurrentIndex(1);
}
