#ifndef PESTDISPLAYAPP_H
#define PESTDISPLAYAPP_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QFrame>

class PestDisplayApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit PestDisplayApp(QWidget *parent = nullptr);
    ~PestDisplayApp();

private slots:
    void exitApplication();

private:
    void initializeUI();
    void setupPestData();
    void createPestDisplays();
    bool loadImageSafely(QLabel *label, const QString &path);


    QWidget *centralWidget;
    QLabel *headerLabel;
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    QVBoxLayout *contentLayout;
    QPushButton *exitButton;

    // Data storage
    struct PestInfo {
        QString name;          // Arabic name
        QString imagePath;
        QString description;   // Arabic description
    };
    QList<PestInfo> pestData;
};

#endif // PESTDISPLAYAPP_H
