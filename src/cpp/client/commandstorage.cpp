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
#include "commandstorage.h"
#include "stor/jdatabase.h"

#include <QDebug>

namespace client
{

CommandStorage::CommandStorage(const QString &storName)
    : _db(stor::JDataBase::instance()),
    _storName(storName)
{
    init();
}

//-----------------------------------------------------------------

QJsonArray CommandStorage::readAll() const
{
    QString req = "SELECT * FROM " + _storName + " ORDER BY fposition ASC;";
    QVariantMap resp = _db.sendQuery(req);
    if (resp.value("error").toBool() == true)
    {
        qDebug() << "CommandStorage::readAll: error: " << resp.value("errorString").toString();
        return QJsonArray();
    }

    return resp.value("data").toJsonArray();
}

//-----------------------------------------------------------------

QJsonObject CommandStorage::write(const QString &descr, const QString &cmd) const
{
    // Create record
    QString req = "INSERT INTO " + _storName + " (fdescr, fcmd, fposition) \
                   VALUES(?, ?, (SELECT IFNULL(MAX(fposition), 0) + 1 FROM " + _storName + "));";
    QVariantList args;
    args << descr << cmd;
    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true)
    {
        qDebug() << "CommandStorage::write: insert error: " << resp.value("errorString").toString();
        return QJsonObject();
    }

    // Select last record
    req = "SELECT * FROM " + _storName + " ORDER BY fid DESC LIMIT 1;";
    resp = _db.sendQuery(req);
    if (resp.value("error").toBool() == true || resp.value("data").toJsonArray().size() != 1)
    {
        qDebug() << "CommandStorage::write: select last error: " << resp.value("errorString").toString();
        return QJsonObject();
    }
    return resp.value("data").toJsonArray().at(0).toObject();
}

//-----------------------------------------------------------------

bool CommandStorage::updatePosition(int id, int newPosition) const
{
    QString req = "UPDATE " + _storName + " SET fposition = ? WHERE fid = ?;";
    QVariantList args;
    args << newPosition << id;
    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true)
    {
        qDebug() << "CommandStorage::updatePosition: error: " << resp.value("errorString").toString();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------

bool CommandStorage::remove(int id) const
{
    QString req = "DELETE FROM " + _storName + " WHERE fid = ?;";
    QVariantList args;
    args << id;
    QVariantMap resp = _db.sendQuery(req, args);
    if (resp.value("error").toBool() == true)
    {
        qDebug() << "CommandStorage::remove: error: " << resp.value("errorString").toString();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------

bool CommandStorage::clear() const
{
    QString req = "DELETE FROM " + _storName + ";";
    QVariantMap resp = _db.sendQuery(req);
    if (resp.value("error").toBool() == true)
    {
        qDebug() << "CommandStorage::clear: error: " << resp.value("errorString").toString();
        return false;
    }

    return true;
}

//-----------------------------------------------------------------

void CommandStorage::init() const
{
    if (_db.isTableExists(_storName) == true) return;

    QString req = "CREATE TABLE "+ _storName +" ( \
                   fid        INTEGER PRIMARY KEY AUTOINCREMENT, \
                   fdescr     varchar(100) NOT NULL, \
                   fcmd       varchar(99999) NOT NULL, \
                   fposition  INTEGER NOT NULL \
                   );";
    _db.sendQuery(req);
}

//-----------------------------------------------------------------

}   // namespace client

