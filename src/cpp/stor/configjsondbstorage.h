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
#ifndef CONFIGJSONDBSTORAGE_H
#define CONFIGJSONDBSTORAGE_H

#include <QObject>
#include <QJsonObject>

namespace stor
{

class JDataBase;

class ConfigJsonDbStorage : public QObject
{
    Q_OBJECT

public:
    static ConfigJsonDbStorage& instance();

public:
    // Config types
    Q_INVOKABLE bool createType(const QString &type) const;
    Q_INVOKABLE bool isTypeValid(const QString &type) const;

    // Config records
    Q_INVOKABLE QJsonObject read(const QString &type, const QString &key) const;
    Q_INVOKABLE QJsonObject read(const QString &type) const;
    Q_INVOKABLE bool        write(const QString &type, const QString &key, const QJsonObject &rec) const;
    Q_INVOKABLE bool        remove(const QString &type, const QString &key) const;
    Q_INVOKABLE bool        remove(const QString &type) const;

private:
    explicit ConfigJsonDbStorage(QObject *parent = nullptr);
    ~ConfigJsonDbStorage() {}
    ConfigJsonDbStorage(const ConfigJsonDbStorage&);
    ConfigJsonDbStorage& operator=(const ConfigJsonDbStorage&);

    void init();

private:
    JDataBase &_db;

};


}   // namespace stor


#endif // CONFIGJSONDBSTORAGE_H
