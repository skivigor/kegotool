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
#include "configjsondbstorage.h"
#include "jdatabase.h"
#include <QJsonArray>
#include <QJsonDocument>

#include <QDebug>

namespace
{
const QString typeTable = QStringLiteral("ttype");
const QString configTable = QStringLiteral("tconfig");
}   // namespace


namespace stor
{

ConfigJsonDbStorage::ConfigJsonDbStorage(QObject *parent)
    : QObject{parent},
    _db(stor::JDataBase::instance())
{
    init();
}

//-----------------------------------------------------------------

ConfigJsonDbStorage &ConfigJsonDbStorage::instance()
{
    static ConfigJsonDbStorage cfg;
    return cfg;
}

//-----------------------------------------------------------------

bool ConfigJsonDbStorage::createType(const QString &type) const
{
    QString req = "INSERT INTO " + typeTable + " (fname) VALUES(?);";
    QVariantList args;
    args << type;

    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true) return false;

    return true;
}

//-----------------------------------------------------------------

bool ConfigJsonDbStorage::isTypeValid(const QString &type) const
{
    QString req = "SELECT * FROM " + typeTable + " WHERE fname = ?;";
    QVariantList args;
    args << type;
    QVariantMap resp = _db.sendQuery(req, args);

    if (resp.value("error").toBool() == true) return false;
    if (resp.value("data").toJsonArray().size() != 1) return false;

    return true;
}

//-----------------------------------------------------------------

QJsonObject ConfigJsonDbStorage::read(const QString &type, const QString &key) const
{
    QString req = "SELECT CFG.fid, TYP.fname, CFG.fkey, CFG.fdata FROM tconfig CFG \
                   JOIN ttype TYP ON TYP.fid = CFG.freftype \
                   WHERE TYP.fname = ? AND CFG.fkey = ?;";
    QVariantList args;
    args << type << key;

    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true) return QJsonObject();
    if (resp.value("data").toJsonArray().size() != 1) return QJsonObject();

    QJsonObject obj = resp.value("data").toJsonArray().at(0).toObject();
    if (obj.value("fdata").isUndefined()) return QJsonObject();

    if (QJsonDocument::fromJson(obj.value("fdata").toString().toLocal8Bit()).isObject() == false)
    {
        remove(type, key);
        return QJsonObject();
    }

    return QJsonDocument::fromJson(obj.value("fdata").toString().toLocal8Bit()).object();;
}

//-----------------------------------------------------------------

QJsonObject ConfigJsonDbStorage::read(const QString &type) const
{
    QString req = "SELECT CFG.fid, TYP.fname, CFG.fkey, CFG.fdata FROM tconfig CFG \
                   JOIN ttype TYP ON TYP.fid = CFG.freftype \
                   WHERE TYP.fname = ?;";
    QVariantList args;
    args << type;

    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true) return QJsonObject();

    QJsonArray arr = resp.value("data").toJsonArray();
    if (arr.isEmpty()) return QJsonObject();

    QJsonObject res;

    for (int  i = 0; i < arr.size(); ++i)
    {
        QJsonObject obj = arr.at(i).toObject();
        if (obj.value("fdata").isUndefined() || obj.value("fkey").isString() == false) return QJsonObject();

        QString key = obj.value("fkey").toString();
        QJsonObject val = QJsonDocument::fromJson(obj.value("fdata").toString().toLocal8Bit()).object();

        res.insert(key, val);
    }

    return res;
}

//-----------------------------------------------------------------

bool ConfigJsonDbStorage::write(const QString &type, const QString &key, const QJsonObject &rec) const
{
    QJsonObject obj = read(type, key);
    if (obj == rec) return true;

    if (obj.isEmpty())
    {
        QString req = "INSERT INTO " + configTable + "  (freftype, fkey, fdata) \
                       VALUES ((SELECT fid FROM " + typeTable + " WHERE fname = ?), ?, ?);";
        QVariantList args;
        args << type << key << QJsonDocument(rec).toJson(QJsonDocument::Compact);

        QVariantMap resp = _db.sendQuery(req, args);
        if (resp.value("error").toBool() == true) return false;
    } else
    {
        QString req = "UPDATE " + configTable + " SET fdata = ?  \
                       WHERE freftype = (SELECT fid FROM " + typeTable + " WHERE fname = ?) AND fkey = ?;";
        QVariantList args;
        args << QJsonDocument(rec).toJson(QJsonDocument::Compact) << type << key;

        QVariantMap resp = _db.sendQuery(req, args);
        if (resp.value("error").toBool() == true) return false;
    }

    return true;
}

//-----------------------------------------------------------------

bool ConfigJsonDbStorage::remove(const QString &type, const QString &key) const
{
    QString req = "DELETE FROM " + configTable + " WHERE freftype = (SELECT fid FROM " +
                  typeTable + " WHERE fname = ?) AND fkey = ?;";
    QVariantList args;
    args << type << key;

    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true) return false;

    return true;
}

//-----------------------------------------------------------------

bool ConfigJsonDbStorage::remove(const QString &type) const
{
    QString req = "DELETE FROM " + configTable + " WHERE freftype = (SELECT fid FROM " +
                  typeTable + " WHERE fname = ?);";
    QVariantList args;
    args << type;

    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true) return false;

    return true;
}

//-----------------------------------------------------------------

void ConfigJsonDbStorage::init()
{
    if (_db.isTableExists(typeTable) == false)
    {
        QString req = "CREATE TABLE " + typeTable + " ( \
                       fid        INTEGER PRIMARY KEY AUTOINCREMENT, \
                       fname      varchar(100) NOT NULL, \
                       UNIQUE (fname) \
                       );";
        _db.sendQuery(req);
    }
    if (_db.isTableExists(configTable) == false)
    {
        QString req = "CREATE TABLE " + configTable + " ( \
                       fid        INTEGER PRIMARY KEY AUTOINCREMENT, \
                       freftype   INTEGER NOT NULL REFERENCES " + typeTable + "(fid), \
                       fkey       varchar(100) NOT NULL, \
                       fdata      varchar(99999) NOT NULL, \
                       UNIQUE (freftype, fkey) \
                       );";
        _db.sendQuery(req);
    }
}

//-----------------------------------------------------------------



}   // namespace stor

