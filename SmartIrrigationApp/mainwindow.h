#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "loginwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLoginWindow();

private:
    QTimer *splashTimer;
    LoginWindow *loginWindow;
};

#endif // MAINWINDOW_H
