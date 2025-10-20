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
#include "appstyle.h"
#include "configjsondbstorage.h"
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDir>
#include "fileloader.h"

#include "util/fsutils.h"

#include <QDebug>


namespace
{
const QString Type = QStringLiteral("app");
const QString Style = QStringLiteral("style");
const QString DefStylesPath = QStringLiteral(":/resources/styles");
}   // namespace



namespace stor
{

AppStyle::AppStyle(QQmlApplicationEngine &engine, const QString &stylesDir, QObject *parent)
    : QObject{parent},
    _engine(engine)
{
    _stylesPath = stylesDir;
    if (_stylesPath.isEmpty())
        _stylesPath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppConfigLocation) + "/styles";
    util::FsUtils::copyDir(DefStylesPath, _stylesPath);

    initStyle();
}

AppStyle::~AppStyle()
{
}

//-----------------------------------------------------------------

bool AppStyle::setStyle(int idx)
{
    if (idx < 0 || idx > _styleList.size() - 1) return false;

    QString name = _styleList.at(idx).toObject().value("name").toString();
    QJsonObject obj {
        { "style", name }
    };
    bool res = ConfigJsonDbStorage::instance().write(Type, Style, obj);
    if (res == true)
    {
        _styleName = name;
        emit styleNameChanged();
    }

    return res;
}

//-----------------------------------------------------------------

void AppStyle::initStyle()
{
    ConfigJsonDbStorage &cfg = ConfigJsonDbStorage::instance();
    if (cfg.isTypeValid(Type) == false)
    {
        cfg.createType(Type);
    } else
    {
        QJsonObject obj = cfg.read(Type, Style);
        if (!obj.isEmpty() && util::FsUtils::isDir(_stylesPath + "/" + obj.value("style").toString()))
            _styleName = obj.value("style").toString();
    }

    QString currentStyleDir = _stylesPath + "/" + _styleName;
    qputenv("QT_QUICK_CONTROLS_CONF", QString(currentStyleDir + "/qtquickcontrols2.conf").toLocal8Bit());

    QString styleCfgPath = currentStyleDir + "/style.json";
    _styleConfig = FileLoader::readFileAsJsonObject(styleCfgPath);
    _styleConfig["theme"] = "file:///" + currentStyleDir + "/theme.jpg";
    _styleConfig["logo"] = "file:///" + currentStyleDir + "/logo.png";
    _engine.rootContext()->setContextProperty("Style", _styleConfig);

    QDir dir(_stylesPath);
    foreach (const QString &name, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QJsonObject obj {
            { "name", name },
            { "screen", "file:///" + _stylesPath + "/" + name + "/screen.jpg" }
        };
        _styleList.append(obj);
    }

}

//-----------------------------------------------------------------

}   // namespace stor

