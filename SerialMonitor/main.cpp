#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "GraphData.h"
#include "SerialPortWrapper.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));


    qmlRegisterSingletonType<SerialPortWrapper>("SerialComponents", 1, 0, "SerialPortWrapper",
                                        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        SerialPortWrapper *pInstance = new SerialPortWrapper();
        return pInstance;
    });

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);






    return app.exec();
}
