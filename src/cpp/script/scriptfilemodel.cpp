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
#include "scriptfilemodel.h"
#include <QStandardPaths>
#include "util/fsutils.h"
#include "stor/fileloader.h"

#include <QDebug>

namespace
{
const QString DefScriptsPath = QStringLiteral(":/resources/other/scripts/examples");
const QString TemplateScriptPath = QStringLiteral(":/resources/other/scripts/template/template.js");
}   // namespace

namespace script
{

ScriptFileModel::ScriptFileModel(const QString &subDir, QObject *parent)
    : QAbstractListModel{parent}
{
    _scriptDirPath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppConfigLocation) + "/scripts";
    if (subDir.isEmpty() == false) _scriptDirPath += ("/" + subDir);

    util::FsUtils::copyDir(DefScriptsPath, _scriptDirPath);
    updateModel();
}

ScriptFileModel::~ScriptFileModel()
{
}

//-----------------------------------------------------------------

ScriptFileModel::Roles ScriptFileModel::roleNames() const
{
    Roles roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";

    return roles;
}

//-----------------------------------------------------------------

int ScriptFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _modelList.size();
}

//-----------------------------------------------------------------

QVariant ScriptFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > _modelList.size() - 1) return QVariant();

    ScriptFileItem itm = _modelList[index.row()];
    if (role == FileNameRole) return itm.FileName;
    if (role == FilePathRole) return itm.FilePath;

    return QVariant();
}

//-----------------------------------------------------------------

ScriptFileItem ScriptFileModel::getItem(int idx) const
{
    if (idx < 0 || idx > _modelList.size() - 1)
        return ScriptFileItem();

    return _modelList.at(idx);
}

//-----------------------------------------------------------------

void ScriptFileModel::updateModel()
{
    clear();

    QStringList list = util::FsUtils::dirContent(_scriptDirPath, QStringList() << "*.js");
    beginInsertRows(QModelIndex(), 0, list.size() - 1);
    foreach(const QString &name, list)
    {
        ScriptFileItem itm(name, _scriptDirPath + "/" + name);
        _modelList.append(itm);
    }
    endInsertRows();
}

//-----------------------------------------------------------------

QString ScriptFileModel::addItem(const QString &fileName)
{
    foreach (const ScriptFileItem &itm, _modelList)
        if (itm.FileName == fileName) return QString();

    QString path = _scriptDirPath + "/" + fileName;
    if (!util::FsUtils::createFile(path, stor::FileLoader::readFile(TemplateScriptPath))) return QString();

    beginInsertRows(QModelIndex(), 0, 0);
    ScriptFileItem itm(fileName, path);
    _modelList.prepend(itm);
    endInsertRows();

    return path;
}

//-----------------------------------------------------------------

bool ScriptFileModel::delItem(const QString &fileName)
{
    int idx = -1;
    for (int i = 0; i < _modelList.size(); ++i)
    {
        if (_modelList.at(i).FileName == fileName)
        {
            idx = i;
            break;
        }
    }

    if (idx >= 0) return delItem(idx);
    return false;
}

//-----------------------------------------------------------------

bool ScriptFileModel::delItem(int idx)
{
    if (idx < 0 || idx > _modelList.size() - 1) return false;

    QString path = _modelList.at(idx).FilePath;
    if (!util::FsUtils::deleteFile(path)) return false;

    beginRemoveRows(QModelIndex(), idx, idx);
    _modelList.removeAt(idx);
    endRemoveRows();

    return true;
}

//-----------------------------------------------------------------

void ScriptFileModel::clear()
{
    if (_modelList.isEmpty()) return;

    beginRemoveRows(QModelIndex(), 0, _modelList.size() - 1);
    _modelList.clear();
    endRemoveRows();
}

//-----------------------------------------------------------------

}   // namespace script

