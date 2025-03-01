#ifndef DASHBORDWINDOW_H
#define DASHBORDWINDOW_H

#include <QMainWindow> // Inherit from QMainWindow (or QWidget if it's not a main window)

class DashbordWindow : public QMainWindow {
    Q_OBJECT // This macro is required for signals and slots

public:
    explicit DashbordWindow(QWidget *parent = nullptr);

signals:
         // Declare your signals here (if any)

private slots:
               // Declare your slots here (if any)

private:
         // Add private members and functions here
};

#endif // DASHBORDWINDOW_H
