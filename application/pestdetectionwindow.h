// pestdetectionwindow.h
#ifndef PESTDETECTIONWINDOW_H
#define PESTDETECTIONWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QCamera>
#include <QVideoWidget>
#include <QImageCapture>
#include <QTimer>
#include <QImage>
#include <Python.h>

class PestDetectionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PestDetectionWindow(QWidget *parent = nullptr);
    ~PestDetectionWindow();

private slots:
    void startCamera();
    void stopCamera();
    void captureAndDetect();
    void loadImage();
    void updateCameraFrame();
    void imageCaptured(int id, const QImage &preview);

private:
    // UI elements
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLabel *cameraView;
    QHBoxLayout *buttonsLayout;
    QPushButton *startCameraButton;
    QPushButton *captureButton;
    QPushButton *loadImageButton;
    QPushButton *stopCameraButton;
    QGroupBox *resultsGroup;
    QVBoxLayout *resultsLayout;
    QLabel *resultLabel;
    QLabel *confidenceLabel;

    // Camera components
    QCamera *camera;
    QVideoWidget *viewfinder;
    QImageCapture *imageCapture;
    QTimer *timer;
    QImage currentFrame;

    // Python interpreter state
    PyObject *pModule;
    PyObject *pDetectFunc;

    void createUiElements();
    bool initPythonInterpreter();
    void cleanupPythonInterpreter();
    bool detectPests(const QImage &image, QString &className, double &confidence);
};

#endif // PESTDETECTIONWINDOW_H
