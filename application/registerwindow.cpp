#include "registerwindow.h"

// Constructor
RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent), currentPageIndex(0) {
    setupUI();
    showFullScreen(); // Start in full-screen mode
}

// Destructor
RegisterWindow::~RegisterWindow() {}

// Setup UI
void RegisterWindow::setupUI() {
    // Set background image
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/images/background.jpg").scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setAutoFillBackground(true); // Ensure the background is filled

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Exit Button (Red X)
    exitButton = new QPushButton("X", this);
    exitButton->setStyleSheet("background-color: red; color: white; font-weight: bold; border-radius: 15px; font-size: 16px;");
    exitButton->setFixedSize(30, 30);
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);

    // Logo (Circular)
    logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/images/logo (2).png").scaled(100, 100, Qt::KeepAspectRatio));
    logoLabel->setStyleSheet("border-radius: 50px; border: 2px solid black;");
    logoLabel->setAlignment(Qt::AlignCenter);

    // Stacked Widget for Multi-Page Navigation
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createPage1());
    stackedWidget->addWidget(createPage2());
    stackedWidget->addWidget(createPage3());
    stackedWidget->addWidget(createPage4());
    stackedWidget->addWidget(createPage5());

    // Navigation Buttons
    prevButton = new QPushButton("Previous", this);
    nextButton = new QPushButton("Next", this);
    prevButton->setVisible(false);  // Hide "Previous" on first page

    // Style navigation buttons
    prevButton->setFixedSize(100, 40);
    nextButton->setFixedSize(100, 40);
    prevButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 5px;");
    nextButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 5px;");

    connect(prevButton, &QPushButton::clicked, this, &RegisterWindow::prevPage);
    connect(nextButton, &QPushButton::clicked, this, &RegisterWindow::nextPage);

    // Layout for Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(nextButton);

    // Assemble Main Layout
    mainLayout->addWidget(exitButton, 0, Qt::AlignRight);
    mainLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(stackedWidget);
    mainLayout->addLayout(buttonLayout);

    // No need to call setLayout(mainLayout) because mainLayout is already set as the layout for 'this'
}

// Create Page 1 (Personal Info)
QWidget* RegisterWindow::createPage1() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page); // Set layout directly on the page

    nameEdit = new QLineEdit();
    emailEdit = new QLineEdit();
    phoneEdit = new QLineEdit();
    locationEdit = new QLineEdit();
    mapButton = new QPushButton("Select Location");

    // Style input fields
    nameEdit->setFixedSize(300, 40);
    emailEdit->setFixedSize(300, 40);
    phoneEdit->setFixedSize(300, 40);
    locationEdit->setFixedSize(300, 40);
    mapButton->setFixedSize(300, 40);

    nameEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    emailEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    phoneEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    locationEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    mapButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 5px;");

    // Style labels
    QLabel *nameLabel = new QLabel("Name:");
    QLabel *emailLabel = new QLabel("Email:");
    QLabel *phoneLabel = new QLabel("Phone:");
    QLabel *locationLabel = new QLabel("Location:");

    nameLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    emailLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    phoneLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    locationLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");

    // Add widgets to the layout
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(emailLabel);
    layout->addWidget(emailEdit);
    layout->addWidget(phoneLabel);
    layout->addWidget(phoneEdit);
    layout->addWidget(locationLabel);
    layout->addWidget(locationEdit);
    layout->addWidget(mapButton);

    // Center the layout
    layout->setAlignment(Qt::AlignCenter);

    return page;
}

// Create Page 2 (Crop Info)
QWidget* RegisterWindow::createPage2() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page); // Set layout directly on the page

    cropTypeCombo = new QComboBox();
    areaEdit = new QLineEdit();
    soilTypeCombo = new QComboBox();
    plantDateEdit = new QDateEdit();

    // Style input fields
    cropTypeCombo->setFixedSize(300, 40);
    areaEdit->setFixedSize(300, 40);
    soilTypeCombo->setFixedSize(300, 40);
    plantDateEdit->setFixedSize(300, 40);

    cropTypeCombo->setStyleSheet("padding: 5px; font-size: 16px;");
    areaEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    soilTypeCombo->setStyleSheet("padding: 5px; font-size: 16px;");
    plantDateEdit->setStyleSheet("padding: 5px; font-size: 16px;");

    // Style labels
    QLabel *cropTypeLabel = new QLabel("Crop Type:");
    QLabel *areaLabel = new QLabel("Area (sq.m):");
    QLabel *soilTypeLabel = new QLabel("Soil Type:");
    QLabel *plantDateLabel = new QLabel("Planting Date:");

    cropTypeLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    areaLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    soilTypeLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    plantDateLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");

    // Add widgets to the layout
    layout->addWidget(cropTypeLabel);
    layout->addWidget(cropTypeCombo);
    layout->addWidget(areaLabel);
    layout->addWidget(areaEdit);
    layout->addWidget(soilTypeLabel);
    layout->addWidget(soilTypeCombo);
    layout->addWidget(plantDateLabel);
    layout->addWidget(plantDateEdit);

    // Center the layout
    layout->setAlignment(Qt::AlignCenter);

    return page;
}

// Create Page 3 (Cultivation Conditions)
QWidget* RegisterWindow::createPage3() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page); // Set layout directly on the page

    waterNeedsEdit = new QLineEdit();
    sunlightNeedsCombo = new QComboBox();
    irrigationFreqEdit = new QLineEdit();
    irrigationTypeCombo = new QComboBox();

    // Style input fields
    waterNeedsEdit->setFixedSize(300, 40);
    sunlightNeedsCombo->setFixedSize(300, 40);
    irrigationFreqEdit->setFixedSize(300, 40);
    irrigationTypeCombo->setFixedSize(300, 40);

    waterNeedsEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    sunlightNeedsCombo->setStyleSheet("padding: 5px; font-size: 16px;");
    irrigationFreqEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    irrigationTypeCombo->setStyleSheet("padding: 5px; font-size: 16px;");

    // Style labels
    QLabel *waterNeedsLabel = new QLabel("Water Needs (L/day):");
    QLabel *sunlightNeedsLabel = new QLabel("Sunlight Needs:");
    QLabel *irrigationFreqLabel = new QLabel("Irrigation Frequency (days):");
    QLabel *irrigationTypeLabel = new QLabel("Irrigation Type:");

    waterNeedsLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    sunlightNeedsLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    irrigationFreqLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    irrigationTypeLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");

    // Add widgets to the layout
    layout->addWidget(waterNeedsLabel);
    layout->addWidget(waterNeedsEdit);
    layout->addWidget(sunlightNeedsLabel);
    layout->addWidget(sunlightNeedsCombo);
    layout->addWidget(irrigationFreqLabel);
    layout->addWidget(irrigationFreqEdit);
    layout->addWidget(irrigationTypeLabel);
    layout->addWidget(irrigationTypeCombo);

    // Center the layout
    layout->setAlignment(Qt::AlignCenter);

    return page;
}

// Create Page 4 (Climate & Budget)
QWidget* RegisterWindow::createPage4() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page); // Set layout directly on the page

    climateConditionCombo = new QComboBox();
    budgetEdit = new QLineEdit();
    notificationCheckBox = new QCheckBox("Enable Notifications");
    notificationFreqCombo = new QComboBox();

    // Style input fields
    climateConditionCombo->setFixedSize(300, 40);
    budgetEdit->setFixedSize(300, 40);
    notificationFreqCombo->setFixedSize(300, 40);

    climateConditionCombo->setStyleSheet("padding: 5px; font-size: 16px;");
    budgetEdit->setStyleSheet("padding: 5px; font-size: 16px;");
    notificationFreqCombo->setStyleSheet("padding: 5px; font-size: 16px;");

    // Style labels
    QLabel *climateConditionLabel = new QLabel("Climate Condition:");
    QLabel *budgetLabel = new QLabel("Budget:");
    QLabel *notificationFreqLabel = new QLabel("Notification Frequency:");

    climateConditionLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    budgetLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");
    notificationFreqLabel->setStyleSheet("color: #0a3d02; font-size: 18px;");

    // Add widgets to the layout
    layout->addWidget(climateConditionLabel);
    layout->addWidget(climateConditionCombo);
    layout->addWidget(budgetLabel);
    layout->addWidget(budgetEdit);
    layout->addWidget(notificationCheckBox);
    layout->addWidget(notificationFreqLabel);
    layout->addWidget(notificationFreqCombo);

    // Center the layout
    layout->setAlignment(Qt::AlignCenter);

    return page;
}

// Create Page 5 (Profile Picture & Submit)
QWidget* RegisterWindow::createPage5() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page); // Set layout directly on the page

    uploadButton = new QPushButton("Upload Profile Picture");
    profilePictureLabel = new QLabel();
    registerButton = new QPushButton("Register");

    // Style input fields
    uploadButton->setFixedSize(300, 40);
    registerButton->setFixedSize(300, 40);

    uploadButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 10px;");
    registerButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 10px;");

    // Add widgets to the layout
    layout->addWidget(uploadButton);
    layout->addWidget(profilePictureLabel);
    layout->addWidget(registerButton);

    // Center the layout
    layout->setAlignment(Qt::AlignCenter);

    return page;
}

// Next Page Navigation
void RegisterWindow::nextPage() {
    if (currentPageIndex < stackedWidget->count() - 1) {
        currentPageIndex++;
        stackedWidget->setCurrentIndex(currentPageIndex);
    }
    prevButton->setVisible(currentPageIndex > 0);
    nextButton->setVisible(currentPageIndex < stackedWidget->count() - 1);
}

// Previous Page Navigation
void RegisterWindow::prevPage() {
    if (currentPageIndex > 0) {
        currentPageIndex--;
        stackedWidget->setCurrentIndex(currentPageIndex);
    }
    prevButton->setVisible(currentPageIndex > 0);
    nextButton->setVisible(currentPageIndex < stackedWidget->count() - 1);
}

// Choose Profile Picture
void RegisterWindow::chooseProfilePicture() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty()) {
        profilePictureLabel->setPixmap(QPixmap(filePath).scaled(100, 100, Qt::KeepAspectRatio));
    }
}

// Toggle notification options based on checkbox state
void RegisterWindow::toggleNotificationOptions(int state) {
    if (state == Qt::Checked) {
        notificationFreqCombo->setEnabled(true);
    } else {
        notificationFreqCombo->setEnabled(false);
    }
}

// Open map selection (dummy implementation for now)
void RegisterWindow::openMapSelection() {
    QMessageBox::information(this, "Map Selection", "Map selection feature not yet implemented.");
}

// Submit Form
void RegisterWindow::submitForm() {
    QMessageBox::information(this, "Success", "Registration Complete!");
}

// Handle window resize event
void RegisterWindow::resizeEvent(QResizeEvent *event) {
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/images/background.jpg").scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
    QWidget::resizeEvent(event);
}
