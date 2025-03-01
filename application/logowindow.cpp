#include "logowindow.h"
#include <QPixmap>
#include <QScreen>
#include <QApplication>
#include <QLinearGradient>
#include <QPainter>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QRandomGenerator>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

LogoWindow::LogoWindow(QWidget *parent) : QWidget(parent) {
    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Add the logo image
    logoLabel = new QLabel(this);
    QPixmap logoPixmap(":/images/logo (2).png");
    logoLabel->setPixmap(logoPixmap.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->hide(); // Hide initially

    // Spacer to push text towards the bottom
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Add the text below the logo
    textLabel = new QLabel(this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet(
        "font-size: 36px; font-weight: 600; color: #2ECC71; "
        "font-family: 'French Script MT';"
        );

    textLabel->setText(""); // Start with an empty text
    textLabel->setGraphicsEffect(nullptr); // Ensure no opacity effects interfere
    textLabel->hide(); // Hide initially

    // Add widgets to layout
    layout->addWidget(logoLabel);
    layout->addItem(spacer); // Push text down
    layout->addWidget(textLabel);
    setLayout(layout);

    // Set full screen mode
    setWindowState(Qt::WindowFullScreen);
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #DDDDDD);");

    // Start logo animation
    setupLogoAnimation();
}

void LogoWindow::setupLogoAnimation() {
    QSequentialAnimationGroup *logoAnimationGroup = new QSequentialAnimationGroup(this);

    // Split the logo into 16 pieces (4x4 grid)
    const int gridSize = 4;
    const int pieceWidth = logoLabel->pixmap().width() / gridSize;
    const int pieceHeight = logoLabel->pixmap().height() / gridSize;

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            QLabel *pieceLabel = new QLabel(this);
            QPixmap piecePixmap = logoLabel->pixmap().copy(j * pieceWidth, i * pieceHeight, pieceWidth, pieceHeight);
            pieceLabel->setPixmap(piecePixmap);
            pieceLabel->setAlignment(Qt::AlignCenter);

            // Create opacity effect
            QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(pieceLabel);
            pieceLabel->setGraphicsEffect(opacityEffect);
            opacityEffect->setOpacity(0);

            // Position pieces correctly
            pieceLabel->setGeometry(j * pieceWidth + (width() - logoLabel->pixmap().width()) / 2,
                                    i * pieceHeight + (height() - logoLabel->pixmap().height()) / 2,
                                    pieceWidth, pieceHeight);
            pieceLabel->show();

            // Opacity animation
            QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
            opacityAnimation->setDuration(500);
            opacityAnimation->setStartValue(0);
            opacityAnimation->setEndValue(1);

            // Add animation
            logoAnimationGroup->addAnimation(opacityAnimation);
        }
    }

    // When all pieces are shown, delay before showing text
    connect(logoAnimationGroup, &QSequentialAnimationGroup::finished, this, [this]() {
        QTimer::singleShot(500, this, [this]() {
            textLabel->show();  // Ensure text is visible before animation
            animateText();  // Start text animation
        });
    });

    logoAnimationGroup->start();
}

// Function to animate text like it's being typed
void LogoWindow::animateText() {
    QString fullText = "welcome to SmartIrrigapp";
    textLabel->setText(""); // Start with an empty text
    textLabel->show(); // Make it visible
    textLabel->adjustSize(); // Force layout update
    layout()->update(); // Refresh layout

    // Ensure no opacity effect interferes
    textLabel->setGraphicsEffect(nullptr);

    // Timer for typewriter effect
    QTimer *textTimer = new QTimer(this);
    int *index = new int(0);

    connect(textTimer, &QTimer::timeout, this, [this, textTimer, fullText, index]() {
        if (*index < fullText.length()) {
            textLabel->setText(fullText.left(*index + 1)); // Add one letter
            textLabel->adjustSize(); // Update size
            layout()->update(); // Refresh layout
            (*index)++;
        } else {
            textTimer->stop();
            delete index; // Free memory

            // Ensure text stays visible after animation
            QTimer::singleShot(2000, this, [this]() {
                emit finished();
                close();
            });
        }
    });

    textTimer->start(100); // Delay between letters
}
