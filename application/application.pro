# Modules Qt requis
QT += core gui widgets charts mqtt multimedia multimediawidgets quick network
QT += charts multimedia multimediawidgets
# Configuration pour C++17
CONFIG += c++17

# Vous pouvez faire échouer la compilation si des API dépréciées sont utilisées.
# Pour ce faire, décommentez la ligne suivante.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000  # Désactive toutes les API dépréciées avant Qt 6.0.0

# Fichiers sources
SOURCES += \
    dashboardwindow.cpp \
    logowindow.cpp \
    main.cpp \
    pestdisplayapp.cpp \
    registerwindow.cpp \
    signinwindow.cpp \
    valuemqtt.cpp

# Fichiers d'en-tête
HEADERS += \
    dashboardwindow.h \
    logowindow.h \
    pestdisplayapp.h \
    registerwindow.h \
    signinwindow.h \
    valuemqtt.h

# Fichiers de formulaire (si vous utilisez Qt Designer)
FORMS +=

# Fichiers de ressources
RESOURCES += \
    resources.qrc

# Fichiers supplémentaires à inclure dans la distribution
DISTFILES += \
    images/logo (2).png \
    pest_detection.py \
    pest_detection.py
    # Add Python include path
    INCLUDEPATH += C:/Users/mustapha/AppData/Local/Programs/Python/Python312/include
    # Add Python library path
    LIBS += -LC:/Users/mustapha/AppData/Local/Programs/Python/Python312/libs -lpython312
# Règles par défaut pour le déploiement
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# Copie automatique du script Python dans le dossier de build
python_scripts.path = $$OUT_PWD
python_scripts.files = $$PWD/pest_detection.py  # Chemin vers votre fichier source
INSTALLS += python_scripts

# Configuration des chemins
win32 {
    # Chemin de destination (dossier debug)
    DESTDIR = $$OUT_PWD

    # Copie automatique du fichier Python
    python_scripts.files = $$PWD/pest_detection.py
    python_scripts.path = $$DESTDIR
    INSTALLS += python_scripts

    # Vérification de Python
    !system(python --version > NUL 2>&1) {
        error("Python n'est pas installé ou n'est pas dans le PATH")
    }
}
