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
#ifndef FILELOADER_H
#define FILELOADER_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace stor
{

class FileLoader
{
    Q_GADGET

public:
     explicit FileLoader();

public:
    Q_INVOKABLE static QByteArray readFile(const QString &path) noexcept;
    Q_INVOKABLE static QJsonDocument readFileAsJsonDoc(const QString &path) noexcept;
    Q_INVOKABLE static QJsonObject readFileAsJsonObject(const QString &path) noexcept;
    Q_INVOKABLE static QJsonArray readFileAsJsonArray(const QString &path) noexcept;

    Q_INVOKABLE static bool saveFile(const QString &path, const QByteArray &data) noexcept;
    Q_INVOKABLE static bool saveFile(const QString &path, const QString &data) noexcept;
    Q_INVOKABLE static bool saveFile(const QString &path, const QJsonDocument &data) noexcept;
    Q_INVOKABLE static bool saveFile(const QString &path, const QJsonObject &data) noexcept;
    Q_INVOKABLE static bool saveFile(const QString &path, const QJsonArray &data) noexcept;

};

}    // namespace stor


#endif // FILELOADER_H
