// pestdetectionwindow.cpp
#include "pestdetectionwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QDebug>
#include <QFont>

PestDetectionWindow::PestDetectionWindow(QWidget *parent) : QMainWindow(parent),
    camera(nullptr),
    viewfinder(nullptr),
    imageCapture(nullptr),
    timer(nullptr),
    pModule(nullptr),
    pDetectFunc(nullptr)
{
    // Set window properties
    setWindowTitle("Pest Detection System");
    setMinimumSize(800, 600);

    // Create UI elements
    createUiElements();

    // Initialize Python interpreter
    if (!initPythonInterpreter()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to initialize Python interpreter"));
    }

    // Setup timer for frame updates
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PestDetectionWindow::updateCameraFrame);

    // Initial status message
    statusBar()->showMessage("Ready");
}

PestDetectionWindow::~PestDetectionWindow()
{
    if (camera) {
        camera->stop();
        delete camera;
    }

    if (timer) {
        timer->stop();
        delete timer;
    }

    cleanupPythonInterpreter();
}

void PestDetectionWindow::createUiElements()
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Title label
    titleLabel = new QLabel("Fruit Tree Pest Detection", centralWidget);
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Camera view
    cameraView = new QLabel(centralWidget);
    cameraView->setMinimumSize(640, 480);
    cameraView->setText("Camera Feed Will Appear Here");
    cameraView->setAlignment(Qt::AlignCenter);
    cameraView->setFrameShape(QFrame::Box);
    mainLayout->addWidget(cameraView);

    // Buttons layout
    buttonsLayout = new QHBoxLayout();

    // Start Camera Button
    startCameraButton = new QPushButton("Start Camera", centralWidget);
    connect(startCameraButton, &QPushButton::clicked, this, &PestDetectionWindow::startCamera);
    buttonsLayout->addWidget(startCameraButton);

    // Capture Button
    captureButton = new QPushButton("Capture and Detect", centralWidget);
    connect(captureButton, &QPushButton::clicked, this, &PestDetectionWindow::captureAndDetect);
    captureButton->setEnabled(false);
    buttonsLayout->addWidget(captureButton);

    // Load Image Button
    loadImageButton = new QPushButton("Load Image", centralWidget);
    connect(loadImageButton, &QPushButton::clicked, this, &PestDetectionWindow::loadImage);
    buttonsLayout->addWidget(loadImageButton);

    // Stop Camera Button
    stopCameraButton = new QPushButton("Stop Camera", centralWidget);
    connect(stopCameraButton, &QPushButton::clicked, this, &PestDetectionWindow::stopCamera);
    stopCameraButton->setEnabled(false);
    buttonsLayout->addWidget(stopCameraButton);

    mainLayout->addLayout(buttonsLayout);

    // Results Group Box
    resultsGroup = new QGroupBox("Detection Results", centralWidget);
    resultsLayout = new QVBoxLayout(resultsGroup);

    // Detection result label
    resultLabel = new QLabel("No detection performed yet", resultsGroup);
    resultLabel->setAlignment(Qt::AlignCenter);
    QFont resultFont;
    resultFont.setPointSize(12);
    resultLabel->setFont(resultFont);
    resultsLayout->addWidget(resultLabel);

    // Confidence score label
    confidenceLabel = new QLabel("Confidence: N/A", resultsGroup);
    confidenceLabel->setAlignment(Qt::AlignCenter);
    resultsLayout->addWidget(confidenceLabel);

    mainLayout->addWidget(resultsGroup);

    // Create status bar
    statusBar();
}

bool PestDetectionWindow::initPythonInterpreter()
{
    try {
        Py_Initialize();

        // Add current directory to Python path
        PyObject *sys = PyImport_ImportModule("sys");
        PyObject *sys_path = PyObject_GetAttrString(sys, "path");
        PyObject *path = PyUnicode_FromString(".");
        PyList_Append(sys_path, path);
        Py_DECREF(path);
        Py_DECREF(sys_path);
        Py_DECREF(sys);

        // Import our apple_pest_detector module
        pModule = PyImport_ImportModule("apple_pest_detector");
        if (!pModule) {
            PyErr_Print();
            return false;
        }

        // Get the detect_pest function
        pDetectFunc = PyObject_GetAttrString(pModule, "detect_pest");
        if (!pDetectFunc || !PyCallable_Check(pDetectFunc)) {
            PyErr_Print();
            return false;
        }

        return true;
    } catch (...) {
        return false;
    }
}

void PestDetectionWindow::cleanupPythonInterpreter()
{
    if (pDetectFunc) {
        Py_DECREF(pDetectFunc);
    }

    if (pModule) {
        Py_DECREF(pModule);
    }

    Py_Finalize();
}

void PestDetectionWindow::startCamera()
{
    // Initialize camera if not already done
    if (!camera) {
        camera = new QCamera(this);
        viewfinder = new QVideoWidget(this);
        viewfinder->resize(cameraView->size());
        viewfinder->setParent(cameraView);
        viewfinder->show();

        camera->setViewfinder(viewfinder);

        // Create QImageCapture with parent only
        imageCapture = new QImageCapture(this);
        // Set the image capture for the camera
        camera->setImageCapture(imageCapture);

        connect(imageCapture, &QImageCapture::imageCaptured,
                this, &PestDetectionWindow::imageCaptured);
    }

    // Start the camera
    camera->start();

    // Update UI state
    startCameraButton->setEnabled(false);
    stopCameraButton->setEnabled(true);
    captureButton->setEnabled(true);
    loadImageButton->setEnabled(false);

    // Start timer to update status
    timer->start(1000); // Check camera state every second

    statusBar()->showMessage(tr("Camera started"), 3000);
}

void PestDetectionWindow::stopCamera()
{
    if (camera) {
        timer->stop();
        camera->stop();

        // Update UI state
        startCameraButton->setEnabled(true);
        stopCameraButton->setEnabled(false);
        captureButton->setEnabled(false);
        loadImageButton->setEnabled(true);

        // Hide viewfinder
        if (viewfinder) {
            viewfinder->hide();
        }

        statusBar()->showMessage(tr("Camera stopped"), 3000);
    }
}

void PestDetectionWindow::updateCameraFrame()
{
    // This is just to check camera status periodically
    // Real frame updates are handled by QCameraViewfinder
    if (camera && camera->status() != QCamera::ActiveStatus) {
        statusBar()->showMessage(tr("Camera disconnected"), 3000);
        stopCamera();
    }
}

void PestDetectionWindow::captureAndDetect()
{
    if (camera && imageCapture) {
        imageCapture->capture();
        statusBar()->showMessage(tr("Capturing image..."), 1000);
    }
}

void PestDetectionWindow::imageCaptured(int id, const QImage &preview)
{
    // Store the captured frame
    currentFrame = preview;

    // Display the captured image
    QPixmap pixmap = QPixmap::fromImage(preview);
    cameraView->setPixmap(pixmap.scaled(
        cameraView->width(),
        cameraView->height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));

    // Detect pests in the image
    QString className;
    double confidence;

    if (detectPests(preview, className, confidence)) {
        resultLabel->setText(tr("Detected: %1").arg(className));
        confidenceLabel->setText(tr("Confidence: %1").arg(confidence, 0, 'f', 2));

        // Color-code the result based on confidence
        if (confidence > 0.7) {
            resultLabel->setStyleSheet("color: green");
        } else if (confidence > 0.5) {
            resultLabel->setStyleSheet("color: orange");
        } else {
            resultLabel->setStyleSheet("color: red");
        }
    } else {
        resultLabel->setText(tr("Detection failed"));
        confidenceLabel->setText(tr("Confidence: N/A"));
        resultLabel->setStyleSheet("");
    }
}

void PestDetectionWindow::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.jpeg *.bmp)")
        );

    if (!filePath.isEmpty()) {
        QImage image(filePath);
        if (!image.isNull()) {
            // Store and display the image
            currentFrame = image;
            QPixmap pixmap = QPixmap::fromImage(image);
            cameraView->setPixmap(pixmap.scaled(
                cameraView->width(),
                cameraView->height(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                ));

            // Detect pests in the image
            QString className;
            double confidence;

            if (detectPests(image, className, confidence)) {
                resultLabel->setText(tr("Detected: %1").arg(className));
                confidenceLabel->setText(tr("Confidence: %1").arg(confidence, 0, 'f', 2));

                // Color-code the result
                if (confidence > 0.7) {
                    resultLabel->setStyleSheet("color: green");
                } else if (confidence > 0.5) {
                    resultLabel->setStyleSheet("color: orange");
                } else {
                    resultLabel->setStyleSheet("color: red");
                }
            } else {
                resultLabel->setText(tr("Detection failed"));
                confidenceLabel->setText(tr("Confidence: N/A"));
                resultLabel->setStyleSheet("");
            }

            statusBar()->showMessage(tr("Processed image: %1").arg(filePath), 3000);
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Could not load image"));
        }
    }
}

bool PestDetectionWindow::detectPests(const QImage &image, QString &className, double &confidence)
{
    if (!pDetectFunc) {
        return false;
    }

    try {
        // Convert QImage to bytes for Python
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Create Python bytes object
        PyObject *pImageData = PyBytes_FromStringAndSize(
            byteArray.constData(), byteArray.size()
            );

        // Call Python function
        PyObject *pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, pImageData); // pArgs takes ownership of pImageData

        PyObject *pResult = PyObject_CallObject(pDetectFunc, pArgs);
        Py_DECREF(pArgs);

        if (pResult && PyTuple_Check(pResult) && PyTuple_Size(pResult) == 2) {
            // Extract results
            PyObject *pClassName = PyTuple_GetItem(pResult, 0); // Borrowed reference
            PyObject *pConfidence = PyTuple_GetItem(pResult, 1); // Borrowed reference

            className = QString::fromUtf8(PyUnicode_AsUTF8(pClassName));
            confidence = PyFloat_AsDouble(pConfidence);

            Py_DECREF(pResult);
            return true;
        } else {
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
            if (pResult) {
                Py_DECREF(pResult);
            }
            return false;
        }
    } catch (...) {
        return false;
    }
}
