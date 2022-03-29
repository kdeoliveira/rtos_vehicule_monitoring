#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <rtos_common.hpp>

#include "inputdata.h"

int main(int argc, char *argv[])
{


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<InputData>("qnx.rtos", 1, 0, "InputData");

    QQmlApplicationEngine engine;
    QFontDatabase::addApplicationFont("qrc:/fonts/DejaVuSans.ttf");

    app.setFont(QFont("DejaVu Sans"));

    const QUrl url(QStringLiteral("qrc:/qml/dashboard.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        InputData *data = obj->findChild<InputData*>();
        data->init("m_buffer_input");

        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);

    }, Qt::QueuedConnection);


    engine.load(url);

    return app.exec();
}
