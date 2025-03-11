QT       += core gui \
    quick
    QT += charts
    QT += mqtt
    QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -L$$[QT_INSTALL_LIBS] -lQt6Charts


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dashboardwindow.cpp \
    logowindow.cpp \
    main.cpp \
    registerwindow.cpp \
    signinwindow.cpp

HEADERS += \
    dashboardwindow.h \
    logowindow.h \
    registerwindow.h \
    signinwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/logo (2).png \
    images/logo (2).png

RESOURCES += \
    resources.qrc
