#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPalette>
#include <QResizeEvent>

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void nextPage();
    void prevPage();
    void chooseProfilePicture();
    void toggleNotificationOptions(int state);
    void openMapSelection();
    void submitForm();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    QWidget* createPage1();
    QWidget* createPage2();
    QWidget* createPage3();
    QWidget* createPage4();
    QWidget* createPage5();

    int currentPageIndex;
    QStackedWidget *stackedWidget;
    QPushButton *exitButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QLabel *logoLabel;
    QLabel *profilePictureLabel;

    // Page 1
    QLineEdit *nameEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QLineEdit *locationEdit;
    QPushButton *mapButton;

    // Page 2
    QComboBox *cropTypeCombo;
    QLineEdit *areaEdit;
    QComboBox *soilTypeCombo;
    QDateEdit *plantDateEdit;

    // Page 3
    QLineEdit *waterNeedsEdit;
    QComboBox *sunlightNeedsCombo;
    QLineEdit *irrigationFreqEdit;
    QComboBox *irrigationTypeCombo;

    // Page 4
    QComboBox *climateConditionCombo;
    QLineEdit *budgetEdit;
    QCheckBox *notificationCheckBox;
    QComboBox *notificationFreqCombo;

    // Page 5
    QPushButton *uploadButton;
    QPushButton *registerButton;
};

#endif // REGISTERWINDOW_H
