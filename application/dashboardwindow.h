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

// Include Qt Charts headers (No more QtCharts namespace in Qt 6.8)
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class DashboardWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();
     QLabel* createQuickAccessCard(const QString &title, const QString &waterUsage, const QString &timeRemaining);

private slots:
    void showPommierGraph();
    void showBasilicGraph();
    void showOrangerieGraph();

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

    void setupCustomTitleBar();
    void setupLeftSidebar();
    void setupMainSection();
    void setupRightPanel();
    void applyStyles();
    void startAnimations();
    void updateGraph(const QString &culture, const QVector<QPointF> &data);  // Update graph function
};

#endif // DASHBOARDWINDOW_H
