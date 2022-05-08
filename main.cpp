#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controller.h"
#include "liveimageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<double>();
    qRegisterMetaType<QImage*>();
    qmlRegisterType<Controller>("controller", 1, 0, "Controller");

    LiveImageProvider* liveImageProvider(new LiveImageProvider());
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("liveImageProvider", liveImageProvider);
    engine.addImageProvider("live", liveImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
