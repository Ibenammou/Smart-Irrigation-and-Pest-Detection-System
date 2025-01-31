#ifndef FARMDASHBOARD_H
#define FARMDASHBOARD_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QTableWidget>

class FarmDashboard : public QMainWindow {
    Q_OBJECT

public:
    FarmDashboard(QWidget *parent = nullptr);
    ~FarmDashboard();

private slots:
    void showDashboard();

private:
    QStackedWidget *stackedWidget;

    QWidget *createInputPage();
    QWidget *createDashboardPage();

    // Champs d'entrée utilisateur
    QLineEdit *nameInput;
    QLineEdit *farmSizeInput;
    QLineEdit *cropTypeInput;

    // Tableau pour afficher les alertes et recommandations
    QTableWidget *alertsTable;
};

#endif // FARMDASHBOARD_H
