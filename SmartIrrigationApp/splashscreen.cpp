#include "splashscreen.h"
#include <QVBoxLayout>

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 300);
    setStyleSheet("background-color: white;");

    logoLabel = new QLabel(this);
    QPixmap logo(":/assets/basil.glb");
    logoLabel->setPixmap(logo.scaled(200, 200, Qt::KeepAspectRatio));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    setLayout(layout);

    animation = new QPropertyAnimation(logoLabel, "geometry");
    animation->setDuration(2000);
    animation->setStartValue(QRect(150, 300, 50, 50));  // Start small
    animation->setEndValue(QRect(100, 50, 200, 200));   // Expand to normal size
}

void SplashScreen::startAnimation() {
    animation->start();
}
