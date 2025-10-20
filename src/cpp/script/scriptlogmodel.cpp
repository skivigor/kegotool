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
#include "scriptlogmodel.h"
#include <QJsonObject>
//#include <QDebug>

namespace script
{

ScriptLogModel::ScriptLogModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

ScriptLogModel::~ScriptLogModel()
{
}

//-----------------------------------------------------------------

ScriptLogModel::Roles ScriptLogModel::roleNames() const
{
    Roles roles;
    roles[TimeRole] = "time";
    roles[MessageRole] = "message";
    roles[SelectRole] = "sel";

    return roles;
}

//-----------------------------------------------------------------

int ScriptLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _modelList.size();
}

//-----------------------------------------------------------------

QVariant ScriptLogModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > _modelList.size() - 1) return QVariant();

    ScriptLogItem itm = _modelList[index.row()];
    if (role == TimeRole) return itm.Time;
    if (role == MessageRole) return itm.Message;
    if (role == SelectRole) return itm.Selected;

    return QVariant();
}

//-----------------------------------------------------------------

void ScriptLogModel::addItem(const ScriptLogItem &itm)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _modelList.append(itm);
    endInsertRows();
}

//-----------------------------------------------------------------

void ScriptLogModel::addItem(const QString &message)
{
    addItem(ScriptLogItem(message));
}

//-----------------------------------------------------------------

QJsonArray ScriptLogModel::getData(int idx)
{
    if (idx < 0 || idx > _modelList.size() - 1) return QJsonArray();

    int startIdx = -1;
    int endIdx = -1;

    if (_startSelectionIdx >= 0)
        startIdx = _startSelectionIdx;
    if (_endSelectionIdx >= 0)
        endIdx = _endSelectionIdx;

    if (startIdx < 0 || endIdx < 0 || idx < startIdx || idx > endIdx)
    {
        startIdx = idx;
        endIdx = idx;
    }

    QJsonArray arr;
    for (int i = startIdx; i <= endIdx; ++i)
    {
        QJsonObject obj {
            { "time", _modelList.at(i).Time },
            { "message", _modelList.at(i).Message }
        };
        arr.append(obj);
    }

    return arr;
}

//-----------------------------------------------------------------

void ScriptLogModel::select(int idx)
{
    if (idx < 0 || idx > _modelList.size() - 1) return;

    if ((_startSelectionIdx >= 0) && (_endSelectionIdx >= 0) && (_startSelectionIdx != _endSelectionIdx))
        unselect();

    if (_startSelectionIdx < 0)
    {
        _startSelectionIdx = _endSelectionIdx = idx;
    } else
    {
        if (idx < _startSelectionIdx)
        {
            _endSelectionIdx = _startSelectionIdx;
            _startSelectionIdx = idx;
        } else
        {
            _endSelectionIdx = idx;
        }
    }

    for (int i = _startSelectionIdx; i <= _endSelectionIdx; ++i)
    {
        _modelList[i].Selected = true;
    }

    QModelIndex startSelection = index(_startSelectionIdx, 0);
    QModelIndex endSelection = index(_endSelectionIdx, 0);

    emit dataChanged(startSelection, endSelection);
}

//-----------------------------------------------------------------

void ScriptLogModel::unselect()
{
    if (_startSelectionIdx < 0) return;
    if (_endSelectionIdx < 0) _endSelectionIdx = _startSelectionIdx;

    for (int i = _startSelectionIdx; i <= _endSelectionIdx; ++i)
    {
        _modelList[i].Selected = false;
    }

    QModelIndex startSelection = index(_startSelectionIdx, 0);
    QModelIndex endSelection = index(_endSelectionIdx, 0);

    emit dataChanged(startSelection, endSelection);

    _startSelectionIdx = -1;
    _endSelectionIdx = -1;
}

//-----------------------------------------------------------------

void ScriptLogModel::clear()
{
    if (_modelList.isEmpty()) return;

    beginRemoveRows(QModelIndex(), 0, _modelList.size() - 1);
    _modelList.clear();
    endRemoveRows();
}

//-----------------------------------------------------------------

}   // namespace script

