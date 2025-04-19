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
#include <QDialog> // Include QDialog header

// Include Qt Charts headers (No more QtCharts namespace in Qt 6.8)
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class AlertsWindow; // Forward declaration for AlertsWindow

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();

    QPushButton* createQuickAccessCard(const QString &title, const QString &waterUsage, const QString &timeRemaining); // Change QLabel* to QPushButton*
private slots:
    void showPommierGraph();
    void showBasilicGraph();
    void showOrangerieGraph();
    void onCardClicked(const QString &cropName);
    void displayAlerts(); // Slot for displaying alerts

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
    QChartView *chartView;  // Use standard Qt 6.8 namespace

    // Right Panel
    QWidget *rightPanel;
    QVBoxLayout *rightPanelLayout;
    QLabel *userAvatarLabel;
    QLabel *waterUsageLabel;
    QProgressBar *waterUsageProgressBar;
    QLabel *waterUsageDetailsLabel;
    QLabel *alertsLabel;

    // Animation Timer
    QTimer *animationTimer;

    // Alerts Window
    AlertsWindow *alertsWindow; // Pointer to the AlertsWindow

    void setupCustomTitleBar();
    void setupLeftSidebar();
    void setupMainSection();
    void setupRightPanel();
    void applyStyles();
    void startAnimations();
    void updateGraph(const QString &culture, const QVector<QPointF> &data);  // Update graph function
};

// AlertsWindow class declaration
class AlertsWindow : public QDialog {
    Q_OBJECT

public:
    explicit AlertsWindow(QWidget *parent = nullptr);
    void setAlerts(const QStringList &alerts); // Function to set alerts

private:
    QWidget *alertsContainer; // Container for alerts
};

#endif // DASHBOARDWINDOW_H
