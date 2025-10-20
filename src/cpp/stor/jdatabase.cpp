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
#include "jdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QFile>
#include <QStandardPaths>
#include "util/fsutils.h"

#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

namespace stor
{

JDataBase::JDataBase(const QString &path, const QString &dbName, QObject *parent)
    : QObject(parent),
    _pDb(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")))
{
    QString dbPath = path.isEmpty() ?
                     QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppConfigLocation) + "/kdb" :
                     path;

    util::FsUtils::createDir(dbPath);

    _pDb->setHostName("kego");
    _pDb->setDatabaseName(dbPath + "/" + dbName);
}

JDataBase::~JDataBase()
{
}

//-----------------------------------------------------------------

JDataBase &JDataBase::instance(const QString &path, const QString &dbName)
{
    static JDataBase jdb(path, dbName);
    return jdb;
}

//-----------------------------------------------------------------

bool JDataBase::connectToDatabase()
{
    bool res = _pDb->open();
    if (res == false)
        qDebug() << "JDataBase::connectToDatabase: connection Error: " << _pDb->lastError();

    _connected = res;
    emit connectedChanged();
    return res;
}

//-----------------------------------------------------------------

void JDataBase::disconnectFromDatabase()
{
    if (_pDb->isOpen()) _pDb->close();

    _connected = false;
    emit connectedChanged();
}

//-----------------------------------------------------------------

bool JDataBase::isTableExists(const QString &tname) const
{
    QString req = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?;";
    QVariantList args;
    args << tname;

    QVariantMap ans = sendQuery(req, args);
    if (ans.size() < 2) return false;

    QVariant error = ans.value("error");
    if (error.toBool() == true) return false;

    QJsonArray data = ans.value("data").toJsonArray();
    if (data.size() != 1) return false;

    QJsonObject obj = data.at(0).toObject();
    int ret = obj.value("count(*)").toInt();

    return ret == 1 ? true : false;
}

//-----------------------------------------------------------------

QVariantMap JDataBase::sendQuery(const QString &req, const QVariantList &args) const
{
    if (req.isEmpty() || !_pDb->isOpen()) return QVariantMap();

    QSqlQuery query(*_pDb);
    query.prepare(req);
    for (int i = 0; i < args.size(); ++i) query.addBindValue(args.at(i));
    query.exec();

    // Parse query
    QJsonObject resp;
    QJsonArray arr;

    QSqlError err = query.lastError();
    if (err.isValid())
    {
        qDebug() << "JDataBase::sendQuery: error: " << err.databaseText();
        resp.insert("error", true);
        resp.insert("errorString", err.databaseText());
        return resp.toVariantMap();
    }

    while(query.next())
    {
        QJsonObject obj;
        for(int i = 0; i < query.record().count(); ++i) obj.insert(query.record().fieldName(i), QJsonValue::fromVariant(query.value(i)));
        arr.push_back(obj);
    }

    resp.insert("error", false);
    resp.insert("data", arr);
    return resp.toVariantMap();
}

//-----------------------------------------------------------------

}    // namespace stor

