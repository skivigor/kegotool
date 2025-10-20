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
#ifndef SCRIPTQMLENVCREATOR_H
#define SCRIPTQMLENVCREATOR_H

#include <QObject>
#include <QMap>

namespace script
{

class QmlEnvCreator : public QObject
{
    Q_OBJECT
public:
    explicit QmlEnvCreator(QObject *parent = nullptr);

    Q_INVOKABLE QObject* createScriptEngine(const QString &name, QObject *client);

signals:

private:
    QMap<QString, QObject*> _map;

};


}   // namespace script


#endif // SCRIPTQMLENVCREATOR_H
