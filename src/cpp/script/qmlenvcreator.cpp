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
#include "qmlenvcreator.h"
#include "scriptengine.h"

#include "client/iclient.h"

#include <QDebug>

namespace script
{

QmlEnvCreator::QmlEnvCreator(QObject *parent)
    : QObject{parent}
{
}

//-----------------------------------------------------------------

QObject *QmlEnvCreator::createScriptEngine(const QString &name, QObject *client)
{
    client::IClient *pClient = dynamic_cast<client::IClient*>(client);
    if (pClient == nullptr)
    {
        qDebug() << "ScriptQmlEnvCreator::createScriptEngine: pClient NULL";
        return nullptr;
    }

    ScriptEngine *ptr = nullptr;

    if (_map.contains(name))
    {
        ptr = qobject_cast<ScriptEngine*>(_map.value(name));
    } else
    {
        ptr = new ScriptEngine(pClient, name, this);
        _map.insert(name, ptr);
    }

    return ptr;

}

//-----------------------------------------------------------------

}   // namespace script

