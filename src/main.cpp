#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>
#include <QItemSelectionModel>
#include "tagsmodel.h"
#include "tagsfilteredmodel.h"
#include "tagsfilteredmodel.h"
#include "tagsselectedmodel.h"
#include <QStandardItemModel>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    //api and models
    qmlRegisterType<TagsModel>("com.synaps.tagsmodel", 1, 0, "TagsModel");
    qmlRegisterType<TagsFilteredModel>("com.synaps.tagsfilteredmodel", 1, 0, "TagsFilteredModel");
    qmlRegisterType<TagsSelectedModel>("com.synaps.tagsselectedmodel", 1, 0, "TagsSelectedModel");
    qmlRegisterType<QItemSelectionModel>("com.synaps.tagsselectionmodel", 1, 0, "TagsSelectionModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
