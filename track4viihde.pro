
# Add more folders to ship with the application, here
folder_01.source = qml/track4viihde
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE3D9A4AB

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

QT += script
QT += network

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    engine/utils.cpp \
    engine/track4viihdeengine.cpp \
    engine/simplecrypt.cpp \
    engine/elisanetworkservice.cpp \
    engine/elisadataparser.cpp \
    engine/elisadatamodel.cpp \
    modelviews/programlistmodel.cpp \
    modelviews/recordinglistmodel.cpp \
    modelviews/folderlistmodel.cpp \
    modelviews/programdetailsmodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    TR4_icon_qml.png

HEADERS += \
    engine/utils.h \
    engine/track4viihdeengine.h \
    engine/simplecrypt.h \
    engine/elisanetworkservice.h \
    engine/elisadataparser.h \
    engine/elisadatamodel.h \
    modelviews/programlistmodel.h \
    modelviews/recordinglistmodel.h \
    modelviews/folderlistmodel.h \
    modelviews/programdetailsmodel.h





































