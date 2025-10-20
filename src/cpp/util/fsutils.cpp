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
#include "fsutils.h"
#include <QGuiApplication>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QClipboard>

#include <QDebug>

namespace
{
QString toLocalFile(const QString &path)
{
    return QUrl(path).isLocalFile() ? QUrl(path).toLocalFile() : path;
}

}   // namespace



namespace util
{

FsUtils::FsUtils()
{
}

//-----------------------------------------------------------------

bool FsUtils::isFile(const QString &path)
{
    QFileInfo info(toLocalFile(path));
    return info.isFile();
}

//-----------------------------------------------------------------

bool FsUtils::createFile(const QString &path)
{
    QFile file(toLocalFile(path));
    if (file.open(QIODevice::WriteOnly) == false)
        return false;

    file.close();
    return true;
}

//-----------------------------------------------------------------

bool FsUtils::createFile(const QString &path, const QByteArray &data)
{
    QFile file(toLocalFile(path));
    if (file.open(QIODevice::WriteOnly) == false)
        return false;

    qint64 len = file.write(data);
    file.close();

    if (len == data.size()) return true;
    else return false;
}

//-----------------------------------------------------------------

bool FsUtils::deleteFile(const QString &path)
{
    QFile file(toLocalFile(path));
    return file.remove();
}

//-----------------------------------------------------------------

bool FsUtils::isDir(const QString &path)
{
    QFileInfo info(toLocalFile(path));
    return info.isDir();
}

//-----------------------------------------------------------------

bool FsUtils::createDir(const QString &path)
{
    QDir dir(toLocalFile(path));
    if (dir.exists()) return true;

    return dir.mkpath(".");
}

//-----------------------------------------------------------------

QStringList FsUtils::dirContent(const QString &path, const QStringList &nameFilters)
{
    QDir dir(toLocalFile(path));
    return dir.entryList(nameFilters, QDir::Files, QDir::Time);
}

//-----------------------------------------------------------------

bool FsUtils::copyDir(const QString &srcPath, const QString &dstPath, bool overWrite)
{
    QDir srcDir(srcPath);
    if (!srcDir.exists()) return false;

    QDir dstDir(dstPath);
    if(dstDir.exists() && !overWrite) return false;
    if(dstDir.exists() && overWrite)
    {
        if (!dstDir.removeRecursively()) return false;
    }
    if (!dstDir.mkpath(dstPath)) return false;

    // Create subdirs
    foreach (QString dirName, srcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString subSrcPath = srcPath + "/" + dirName;
        QString subDstPath = dstPath + "/" + dirName;
        if (!copyDir(subSrcPath, subDstPath, overWrite)) return false;
    }

    // Copy files
    foreach (QString fileName, srcDir.entryList(QDir::Files))
    {
        QString fileSrcPath = srcPath + "/" + fileName;
        QString fileDstPath = dstPath + "/" + fileName;
        if (!QFile::copy(fileSrcPath, fileDstPath)) return false;
    }

    return true;
}

//-----------------------------------------------------------------

void FsUtils::copyToClipboard(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

//-----------------------------------------------------------------

void FsUtils::openWithApp(const QString &path)
{
//    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    QDesktopServices::openUrl(QUrl::fromUserInput(path));
}

//-----------------------------------------------------------------

}    // namespace util

