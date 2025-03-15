# Modules Qt requis
QT += core gui widgets charts mqtt multimedia multimediawidgets quick

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
    registerwindow.cpp \
    signinwindow.cpp

# Fichiers d'en-tête
HEADERS += \
    dashboardwindow.h \
    logowindow.h \
    registerwindow.h \
    signinwindow.h

# Fichiers de formulaire (si vous utilisez Qt Designer)
FORMS +=

# Fichiers de ressources
RESOURCES += \
    resources.qrc

# Fichiers supplémentaires à inclure dans la distribution
DISTFILES += \
    images/logo (2).png

# Règles par défaut pour le déploiement
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
