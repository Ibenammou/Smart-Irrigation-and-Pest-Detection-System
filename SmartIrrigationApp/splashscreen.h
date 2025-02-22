#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>

class SplashScreen : public QWidget {
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    void startAnimation();

private:
    QLabel *logoLabel;
    QPropertyAnimation *animation;
};

#endif // SPLASHSCREEN_H
