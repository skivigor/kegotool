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
#ifndef JDATABASE_H
#define JDATABASE_H

#include <QObject>
#include <QVariant>

class QSqlDatabase;

namespace stor
{

class JDataBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString connected READ isConnected NOTIFY connectedChanged)

public:
    static JDataBase& instance(const QString &dbPath = QString(), const QString &dbName = "kegotool.db");

signals:
    void connectedChanged();

public:
    Q_INVOKABLE bool connectToDatabase();
    Q_INVOKABLE void disconnectFromDatabase();
    Q_INVOKABLE bool isConnected() const { return _connected; }

    Q_INVOKABLE QVariantMap sendQuery(const QString &req, const QVariantList &args = QVariantList()) const;
    Q_INVOKABLE bool        isTableExists(const QString &tname) const;

private:
    explicit JDataBase(const QString &path, const QString &dbName, QObject *parent = nullptr);
    ~JDataBase();

private:
    QScopedPointer<QSqlDatabase> _pDb;
    bool  _connected = false;

};


}    // namespace stor


#endif // JDATABASE_H
