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
#include "fileloader.h"
#include <QFile>
#include <QUrl>
#include <QJsonParseError>

namespace stor
{

FileLoader::FileLoader()
{
}

//-----------------------------------------------------------------

bool FileLoader::saveFile(const QString &path, const QByteArray &data) noexcept
{
    QString p(path);
    if (QUrl(path).isLocalFile()) p = QUrl(path).toLocalFile();

    QFile file(p);
    if (!file.open(QIODevice::WriteOnly)) return false;

    qint64 len = file.write(data);
    file.close();

    return len == data.size() ? true : false;
}

//-----------------------------------------------------------------

bool FileLoader::saveFile(const QString &path, const QString &data) noexcept
{
    return saveFile(path, data.toLocal8Bit());
}

//-----------------------------------------------------------------

bool FileLoader::saveFile(const QString &path, const QJsonDocument &data) noexcept
{
    return saveFile(path, data.toJson());
}

//-----------------------------------------------------------------

bool FileLoader::saveFile(const QString &path, const QJsonObject &data) noexcept
{
    return saveFile(path, QJsonDocument(data));
}

//-----------------------------------------------------------------

bool FileLoader::saveFile(const QString &path, const QJsonArray &data) noexcept
{
    return saveFile(path, QJsonDocument(data));
}

//-----------------------------------------------------------------

QByteArray FileLoader::readFile(const QString &path) noexcept
{
    QString p(path);
    if (QUrl(path).isLocalFile()) p = QUrl(path).toLocalFile();

    QFile file(p);
    if (!file.open(QIODevice::ReadOnly)) return QByteArray();

    QByteArray ba = file.readAll();
    file.close();

    return ba;
}

//-----------------------------------------------------------------

QJsonDocument FileLoader::readFileAsJsonDoc(const QString &path) noexcept
{
    QByteArray ba = readFile(path);

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) return QJsonDocument();

    return doc;
}

//-----------------------------------------------------------------

QJsonObject FileLoader::readFileAsJsonObject(const QString &path) noexcept
{
    QJsonDocument doc = readFileAsJsonDoc(path);
    if (doc.isObject() == false) return  QJsonObject();

    return doc.object();
}

//-----------------------------------------------------------------

QJsonArray FileLoader::readFileAsJsonArray(const QString &path) noexcept
{
    QJsonDocument doc = readFileAsJsonDoc(path);
    if (doc.isArray() == false) return  QJsonArray();

    return doc.array();
}

//-----------------------------------------------------------------

}    // namespace stor

