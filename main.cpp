#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include "qmlapplicationviewer.h"
#include "engine/track4viihdeengine.h"

#include <QDebug>
#include <QLibrary>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

#if !defined(QT_NO_LIBRARY)
    qDebug() << "Try to load testability";
    QLibrary testLib("qttestability");
    if(testLib.load()){
        qDebug() << "lib loaded";
        typedef void (*TasInitialize)(void);
        TasInitialize initFunction = (TasInitialize)testLib.resolve("qt_testability_init");
#ifdef Q_OS_SYMBIAN
        //not found so use ordinal
        if(!initFunction){
            initFunction = (TasInitialize)testLib.resolve("1");
        }
#endif
        if(initFunction){
            qDebug() << "init";
            initFunction();
        }
    }
#endif



    Track4ViihdeEngine* engine = Track4ViihdeEngine::instance();

    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    viewer->rootContext()->setContextProperty("track4Engine", engine);
    viewer->rootContext()->setContextProperty("epgModel", engine->epgModel());
    viewer->rootContext()->setContextProperty("recordingModel", engine->recordingModel());    
    viewer->rootContext()->setContextProperty("recordedShowsModel", engine->recordedShowsModel());
    viewer->rootContext()->setContextProperty("foldersModel", engine->folderListModel());
    viewer->rootContext()->setContextProperty("channelModel",  engine->channelModel());
    viewer->rootContext()->setContextProperty("programDateModel",  engine->programDatesModel());
    viewer->rootContext()->setContextProperty("programDetailsModel",  engine->programDetailsModel());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/track4viihde/main.qml"));
    viewer->showExpanded();

/*
    QApplication app(argc, argv);
    QDeclarativeView view;
    view.rootContext()->setContextProperty("track4engine", engine);
    view.setSource(QUrl::fromLocalFile("qml/track4viihde/main.qml"));
    view.show();
*/
    return app->exec();
}
