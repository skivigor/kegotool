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
#include "commandmodel.h"
#include <QJsonArray>

#include <QDebug>

namespace client
{

CommandModel::CommandModel(const QString &storName, QObject *parent)
    : QAbstractListModel(parent), _stor(storName)
{
    init();
}

CommandModel::~CommandModel()
{
    qDebug() << "CommandModel::~CommandModel dtor";
}

//-----------------------------------------------------------------

int CommandModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _modelList.size();
}

//-----------------------------------------------------------------

QHash<int, QByteArray> CommandModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescrRole] = "descr";
    roles[CommandRole] = "cmd";

    return roles;
}

//-----------------------------------------------------------------

QVariant CommandModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > _modelList.size() - 1) return QVariant();

    QJsonObject cmd = _modelList[index.row()];
    if (role == DescrRole) return cmd.value("fdescr").toString();
    if (role == CommandRole) return cmd.value("fcmd").toString();

    return QVariant();
}

//-----------------------------------------------------------------

void CommandModel::addItem(const QString &descr, const QString &cmd)
{
    QJsonObject obj = _stor.write(descr, cmd);
    if (obj.isEmpty()) return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _modelList.append(obj);
    endInsertRows();
}

//-----------------------------------------------------------------

void CommandModel::removeItem(int idx)
{
    if (idx < 0 || idx > _modelList.size() - 1) return;
    if (_stor.remove(_modelList.at(idx).value("fid").toInt()) == false) return;

    beginRemoveRows(QModelIndex(), idx, idx);
    _modelList.removeAt(idx);
    endRemoveRows();
}

//-----------------------------------------------------------------

QJsonObject CommandModel::getCommand(int idx) const
{
    if (idx < 0 || idx > _modelList.size() - 1) return QJsonObject();
    return _modelList.at(idx);
}

//-----------------------------------------------------------------

void CommandModel::moveUp(int idx)
{
    if (idx < 1 || idx > _modelList.size() - 1) return;

    int pos = _modelList[idx]["fposition"].toInt();
    _modelList[idx]["fposition"] = _modelList[idx - 1]["fposition"].toInt();
    _modelList[idx - 1]["fposition"] = pos;

    bool ret = _stor.updatePosition(_modelList[idx]["fid"].toInt(), _modelList[idx]["fposition"].toInt());
    if(ret == false) return;
    ret = _stor.updatePosition(_modelList[idx - 1]["fid"].toInt(), _modelList[idx - 1]["fposition"].toInt());
    if(ret == false) return;

    beginMoveRows(QModelIndex(), idx, idx, QModelIndex(), idx - 1);
    _modelList.swapItemsAt(idx, idx - 1);
    endMoveRows();
}

//-----------------------------------------------------------------

void CommandModel::moveDown(int idx)
{
    if (idx < 0 || idx > _modelList.size() - 2) return;

    moveUp(idx + 1);
}

//-----------------------------------------------------------------

void CommandModel::clear()
{
    if (_modelList.isEmpty()) return;
    if (_stor.clear() == false) return;

    beginRemoveRows(QModelIndex(), 0, _modelList.size() - 1);
    _modelList.clear();
    endRemoveRows();
}

//-----------------------------------------------------------------

void CommandModel::init()
{
    QJsonArray data = _stor.readAll();
    if (data.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, data.size() - 1);
    foreach (QJsonValue val, data)
    {
        QJsonObject obj = val.toObject();
        _modelList.append(obj);

    }
    endInsertRows();
}

//-----------------------------------------------------------------

}   // namespace client

