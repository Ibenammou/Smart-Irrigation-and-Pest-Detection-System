#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QProgressBar>
#include <QPixmap>
#include <QPainter>
#include <QDockWidget>
#include <QTimer>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QDialog>

// Include Qt Charts headers
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Classe pour la fenêtre des alertes
class AlertsWindow : public QDialog {
    Q_OBJECT

public:
    explicit AlertsWindow(QWidget *parent = nullptr);
    void setAlerts(const QStringList &alerts);

private:
    QWidget *alertsContainer;
};

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();
    static QLabel* createQuickAccessCard(const QString &title, const QString &waterUsage, const QString &timeRemaining);

private slots:
    void showPommierGraph();
    void showBasilicGraph();
    void showOrangerieGraph();
    void displayAlerts();

private:
    // Left Sidebar
    QWidget *leftSidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;
    QPushButton *myParcelsButton;
    QPushButton *waterConsumptionButton;
    QPushButton *settingsButton;
    QPushButton *alertsButton;

    // Main Section
    QWidget *mainSection;
    QVBoxLayout *mainLayout;
    QLineEdit *searchBar;
    QWidget *quickAccessSection;
    QHBoxLayout *quickAccessLayout;
    QTableWidget *parcelDetailsTable;
    QChartView *chartView;

    // Right Panel
    QWidget *rightPanel;
    QVBoxLayout *rightPanelLayout;
    QLabel *userAvatarLabel;
    QLabel *waterUsageLabel;
    QProgressBar *waterUsageProgressBar;
    QLabel *waterUsageDetailsLabel;
    QLabel *alertsLabel;

    // Alerts Window
    AlertsWindow *alertsWindow;

    // Animation Timer
    QTimer *animationTimer;

    void setupCustomTitleBar();
    void setupLeftSidebar();
    void setupMainSection();
    void setupRightPanel();
    void applyStyles();
    void startAnimations();
    void updateGraph(const QString &culture, const QVector<QPointF> &data);
};

#endif // DASHBOARDWINDOW_H
