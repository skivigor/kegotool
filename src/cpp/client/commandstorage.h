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
#ifndef COMMANDSTORAGE_H
#define COMMANDSTORAGE_H

#include <QJsonArray>
#include <QJsonObject>

namespace stor
{
class JDataBase;
}   // namespace stor


namespace client
{

class CommandStorage
{
public:
    explicit CommandStorage(const QString &storName);

    QJsonArray  readAll() const;
    QJsonObject write(const QString &descr, const QString &cmd) const;
    bool        updatePosition(int id, int newPosition) const;
    bool        remove(int id) const;
    bool        clear() const;

private:
    void init() const;

private:
    stor::JDataBase &_db;
    QString          _storName;

};


}   // namespace client


#endif // COMMANDSTORAGE_H
