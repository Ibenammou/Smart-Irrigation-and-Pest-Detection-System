#include "PestDisplayApp.h"
#include <QDebug>
#include <QPixmap>
#include <QPalette>
#include <QLinearGradient>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QDir>
#include <QScreen>
#include <QApplication>
#include <QStatusBar>

PestDisplayApp::PestDisplayApp(QWidget *parent) :
    QMainWindow(parent)
{
    try {
        initializeUI();
        setupPestData();
        createPestDisplays();

        // Show in full screen mode
        showFullScreen();

        statusBar()->showMessage("Application initialized successfully");
        qDebug() << "Current working directory:" << QDir::currentPath();
    }
    catch (const std::exception& e) {
        qDebug() << "Error during initialization: " << e.what();
        statusBar()->showMessage("Error during initialization");
        QMessageBox::critical(this, "Error", QString("Initialization error: %1").arg(e.what()));
    }
    catch (...) {
        qDebug() << "Unknown error during initialization";
        statusBar()->showMessage("Unknown error during initialization");
        QMessageBox::critical(this, "Error", "Unknown initialization error occurred");
    }
}

PestDisplayApp::~PestDisplayApp()
{
    // Qt will handle cleanup of child widgets
}

void PestDisplayApp::exitApplication()
{
    QApplication::quit();
}

void PestDisplayApp::initializeUI()
{
    // Set window properties
    setWindowTitle("Catalogue des Parasites Agricoles");
    setMinimumSize(900, 700);

    // Create a green color scheme
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(240, 255, 240));
    gradient.setColorAt(1, QColor(220, 240, 220));

    QPalette pal = palette();
    pal.setBrush(QPalette::Window, gradient);
    setPalette(pal);

    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Create top bar with X button
    QWidget *topBar = new QWidget(centralWidget);
    QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBar->setMaximumHeight(30);

    // Create X button in top right corner
    exitButton = new QPushButton("X", topBar);
    exitButton->setStyleSheet("QPushButton { color: white; font-weight: bold; font-size: 12px; "
                              "background-color: #FF0000; border: none; border-radius: 3px; "
                              "width: 20px; height: 20px; padding: 0px; }"
                              "QPushButton:hover { background-color: #FF3333; }");
    exitButton->setCursor(Qt::PointingHandCursor);
    exitButton->setMaximumSize(20, 20);
    exitButton->setMinimumSize(20, 20);

    // Connect exit button signal to slot
    connect(exitButton, &QPushButton::clicked, this, &PestDisplayApp::exitApplication);

    // Add exit button to right side of top bar
    topBarLayout->addStretch();
    topBarLayout->addWidget(exitButton);
    topBar->setLayout(topBarLayout);

    // Create header
    headerLabel = new QLabel("Catalogue des Parasites Agricoles", centralWidget);
    headerLabel->setAlignment(Qt::AlignCenter);
    QFont headerFont = headerLabel->font();
    headerFont.setPointSize(22);
    headerFont.setBold(true);
    headerLabel->setFont(headerFont);
    headerLabel->setStyleSheet("color: darkgreen; margin-bottom: 20px; padding: 10px;");

    // Create scroll area for content
    scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // Create widget to hold all pest displays
    contentWidget = new QWidget(scrollArea);
    contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(30);
    contentLayout->setContentsMargins(20, 20, 20, 20);

    // Set the content widget in the scroll area
    scrollArea->setWidget(contentWidget);

    // Add widgets to main layout
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(headerLabel);
    mainLayout->addWidget(scrollArea);

    // Set the main layout
    centralWidget->setLayout(mainLayout);
}

void PestDisplayApp::setupPestData()
{
    // Clear any existing data
    pestData.clear();

    // Add pest information with French descriptions
    pestData.append({
        "Mildiou (Downy Mildew)",
        ":/images/mildiou.jpg",
        "Une maladie fongique qui affecte de nombreuses cultures, notamment les raisins, les pommes de terre et les tomates. Elle prospère dans des conditions humides, provoquant des taches jaunâtres sur la surface supérieure des feuilles et une croissance moisie blanchâtre/grisâtre en dessous."
    });

    pestData.append({
        "Dragon Jaune (Huanglongbing)",
        ":/images/dragon_jaune.jpg",
        "Aussi connu sous le nom de Huanglongbing, c'est une maladie bactérienne dévastatrice des agrumes. Elle provoque un jaunissement marbré des feuilles, des fruits difformes au goût amer, et finit par tuer l'arbre."
    });

    pestData.append({
        "Xanthomonas Citri (Chancre des Agrumes)",
        ":/images/Xanthomonas_citri.jpg",
        "Une maladie bactérienne caractérisée par des lésions liégeuses surélevées sur les feuilles, les tiges et les fruits. Elle peut entraîner une défoliation, une chute des fruits et un déclin général de l'arbre."
    });

    pestData.append({
        "Botrytis (Pourriture Grise)",
        ":/images/botrytis.jpg",
        "Un agent pathogène fongique affectant de nombreuses plantes. Il apparaît comme une moisissure gris-brun duveteuse qui pourrit les fleurs, les fruits, les feuilles et les tiges, en particulier dans des conditions fraîches et humides."
    });

    pestData.append({
        "Fumagine (Moisissure Fuligineuse)",
        ":/images/fumagine.jpg",
        "Une croissance fongique noire qui se développe sur les surfaces des plantes couvertes de miellat sécrété par des insectes suceurs de sève. Bien qu'elle n'attaque pas directement les plantes, elle bloque la lumière du soleil et réduit la photosynthèse."
    });

    // Debug message about data
    qDebug() << "Pest data configured with" << pestData.size() << "entries";
}

void PestDisplayApp::createPestDisplays()
{
    // Create a display for each pest
    for (const PestInfo &pest : pestData) {
        // Create container for this pest
        QWidget *pestContainer = new QWidget(contentWidget);
        QVBoxLayout *pestLayout = new QVBoxLayout(pestContainer);

        // Create image label
        QLabel *imageLabel = new QLabel(pestContainer);
        imageLabel->setMinimumSize(500, 400);
        imageLabel->setMaximumSize(500, 400);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet("border: 2px solid darkgreen; border-radius: 10px; background-color: white;");

        // Load the image
        bool success = loadImageSafely(imageLabel, pest.imagePath);
        qDebug() << "Loading image for" << pest.name << (success ? "successful" : "failed");

        // Create name label
        QLabel *nameLabel = new QLabel(pest.name, pestContainer);
        QFont nameFont = nameLabel->font();
        nameFont.setPointSize(16);
        nameFont.setBold(true);
        nameLabel->setFont(nameFont);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("color: darkgreen; margin-top: 10px;");

        // Create description label
        QLabel *descLabel = new QLabel(pest.frenchDescription, pestContainer);
        descLabel->setWordWrap(true);
        descLabel->setAlignment(Qt::AlignJustify);
        QFont descFont = descLabel->font();
        descFont.setPointSize(12);
        descLabel->setFont(descFont);
        descLabel->setStyleSheet("color: #004d00; background-color: rgba(220, 240, 220, 0.7); padding: 15px; border-radius: 10px;");

        // Add components to pest layout
        pestLayout->addWidget(imageLabel, 0, Qt::AlignCenter);
        pestLayout->addWidget(nameLabel);
        pestLayout->addWidget(descLabel);
        pestContainer->setLayout(pestLayout);

        // Add a horizontal line separator except for the last item
        if (&pest != &pestData.last()) {
            QFrame *line = new QFrame(contentWidget);
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background-color: darkgreen;");

            // Add pest container and separator to main content layout
            contentLayout->addWidget(pestContainer);
            contentLayout->addWidget(line);
        } else {
            // Just add the pest container for the last item
            contentLayout->addWidget(pestContainer);
        }
    }

    // Add stretch at the end to push everything to the top
    contentLayout->addStretch();
}

bool PestDisplayApp::loadImageSafely(QLabel *label, const QString &path)
{
    if (!label) {
        qDebug() << "Error: Null label passed to loadImageSafely";
        return false;
    }

    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        qDebug() << "Could not load image from path:" << path;

        // Create a placeholder pixmap with text
        QPixmap placeholderPixmap(500, 400);
        placeholderPixmap.fill(Qt::lightGray);

        QPainter painter(&placeholderPixmap);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 14));

        // Get the filename from the path
        QString filename = path;
        int lastSlash = path.lastIndexOf('/');
        if (lastSlash >= 0)
            filename = path.mid(lastSlash + 1);

        QString placeholderText = "Image non trouvée:\n" + filename;
        QRect textRect = placeholderPixmap.rect().adjusted(20, 20, -20, -20);
        painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, placeholderText);

        label->setPixmap(placeholderPixmap);
        return false;
    } else {
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        return true;
    }
}
