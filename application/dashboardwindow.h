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

class DashboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();

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

    void setupLeftSidebar();    // Function to set up the left sidebar
    void setupMainSection();    // Function to set up the main section
    void setupRightPanel();     // Function to set up the right panel
    void applyStyles();         // Function to apply styles
    void startAnimations();     // Function to start UI animations
};

#endif // DASHBOARDWINDOW_H
