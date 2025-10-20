/*
 * KegoTool using Qt and Qml.
 * Copyright (C) 2025  Skiv <skivigor@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QGuiApplication>
#include <QIcon>
#include <QWindow>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QStandardPaths>
#include <QSysInfo>
#include <QJsonObject>

#include "projectconf.h"

#include "checksum/qmlenvcreator.h"
#include "checksum/crcengine.h"

#include "client/qmlenvcreator.h"
#include "client/bytetablemodel.h"

#include "script/qmlenvcreator.h"

#include "stor/appstyle.h"
#include "stor/jdatabase.h"
#include "stor/configjsondbstorage.h"

#include "util/fsutils.h"
#include "util/byteutils.h"


// TODO delete
//#include "util/testclass.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QCoreApplication::setOrganizationName(KEGO_ORG);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(KEGO_ICON));

    // Info
    QJsonObject sysinfo {
        { "productType", QSysInfo::productType() },
        { "productVersion", QSysInfo::productVersion() },
        { "prettyProductName", QSysInfo::prettyProductName() },
        { "kernelType", QSysInfo::kernelType() },
        { "kernelVersion", QSysInfo::kernelVersion() },
        { "appOrg", KEGO_ORG },
        { "appName", PROJECT_NAME },
        { "appVersion", PROJECT_VERSION },
        { "webProj", KEGO_WEB_PROJ },
        { "webBuilds", KEGO_WEB_BUILDS },
        { "email", KEGO_EMAIL }
    };

    checksum::CrcMode::registerQml();
    client::ByteTableModel::registerQml();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("SysInfo", sysinfo);

    util::ByteUtils byteutils;
    engine.rootContext()->setContextProperty("ByteUtils", QVariant::fromValue(byteutils));

    util::FsUtils fsutils;
    engine.rootContext()->setContextProperty("FsUtils", QVariant::fromValue(fsutils));

    stor::JDataBase &jdb = stor::JDataBase::instance();
    jdb.connectToDatabase();

    stor::AppStyle appStyle(engine);
    engine.rootContext()->setContextProperty("appStyle", &appStyle);
    
    client::QmlEnvCreator clientCreator;
    engine.rootContext()->setContextProperty("client", &clientCreator);

    checksum::QmlEnvCreator chkCreator;
    engine.rootContext()->setContextProperty("checksum", &chkCreator);

    script::QmlEnvCreator scrCreator;
    engine.rootContext()->setContextProperty("script", &scrCreator);

    stor::ConfigJsonDbStorage &cfgstor = stor::ConfigJsonDbStorage::instance();
    engine.rootContext()->setContextProperty("cfgstor", &cfgstor);

    //--------------------------------------------------
    // TODO delete
//    util::TestClass ttt;
//    ttt.show();
    //--------------------------------------------------

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}




