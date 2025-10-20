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
#include "hashcalc.h"
#include "crccalc.h"
//#include <QQmlEngine>

namespace checksum
{

QmlEnvCreator::QmlEnvCreator(QObject *parent)
    : QObject{parent}
{
}

//-----------------------------------------------------------------

QObject* QmlEnvCreator::createCrcCalc(const QString &name)
{
    if (_map.contains(name))
    {
        return _map.value(name);
    } else
    {
        CrcCalc *ptr = new CrcCalc(this);
//        QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
        _map.insert(name, ptr);

        return ptr;
    }

}

//-----------------------------------------------------------------

QObject* QmlEnvCreator::createHashCalc(const QString &name)
{
    if (_map.contains(name))
    {
        return _map.value(name);
    } else
    {
        HashCalc *ptr = new HashCalc(this);
//        QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
        _map.insert(name, ptr);

        return ptr;
    }

}

//-----------------------------------------------------------------

}   // namespace checksum

