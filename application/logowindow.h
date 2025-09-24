#ifndef LOGOWINDOW_H
#define LOGOWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class LogoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogoWindow(QWidget *parent = nullptr);

signals:
    void finished(); // Signal emitted when animation finishes

private:
    QLabel *logoLabel;
    QLabel *textLabel;

    void setupLogoAnimation();
    void animateText(); // Declare the animateText function here
};

#endif // LOGOWINDOW_H
