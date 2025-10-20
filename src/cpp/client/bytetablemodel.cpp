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
#include "bytetablemodel.h"
#include <QtMath>
#include <QThread>
#include <QMetaEnum>

#include <QDebug>

namespace client
{

TByte::TByte(quint8 byte, int msec) :
    _byte(byte),
    _char(QString::fromUtf8(reinterpret_cast<const char *>(&_byte), 1)),
    _hex(QString("%1").arg(_byte, 2, 16, QLatin1Char( '0' )).toUpper()),
    _bin(QString("%1").arg(_byte, 8, 2, QLatin1Char( '0' )).toUpper()),
    _valid(true),
    _msec(msec)
{
    if (byte == 0x09) _char = "\\t";
    if (byte == 0x0a) _char = "\\n";
    if (byte == 0x0c) _char = "\\f";
    if (byte == 0x0d) _char = "\\r";
    if (byte == 0x00) _char = "\\0";
}

//-----------------------------------------------------------------

QJsonObject TByte::toJson() const
{
    QJsonObject js {
        { "byte", _byte },
        { "char", _char },
        { "hex",  _hex },
        { "bin",  _bin },
        { "msec", _msec }
    };

    return js;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------


ByteTableModel::ByteTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    QMetaEnum nlEnum = QMetaEnum::fromType<NewLineMode>();
    for (int i = 0, count = nlEnum.keyCount(); i < count; i++) _newLineModes.append(nlEnum.key(i));
}

//-----------------------------------------------------------------

int ByteTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _rowCount;
}

//-----------------------------------------------------------------

int ByteTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _columnCount;
}

//-----------------------------------------------------------------

QVariant ByteTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > _rowCount) return QVariant();
    if (index.column() < 0 || index.column() > _columnCount) return QVariant();

    int idx = index.row() * _columnCount + index.column();
    if (idx > _modelList.size() - 1) return QVariant();

    TByte ch = _modelList.at(idx);

    if (role == ChRole) return ch.toChar();
    if (role == HxRole) return ch.toHex();
    if (role == BnRole) return ch.toBin();
    if (role == ValidRole) return ch.isValid();
    if (role == MSecRole) return ch.getMSec();
    if (role == SelectRole) return ch.isSelected();

    return QVariant();
}

//-----------------------------------------------------------------

bool ByteTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() == false) return false;

    int idx = index.row() * _columnCount + index.column();
    if (idx > _modelList.size() - 1) return false;

    TByte ch = _modelList.at(idx);
    ch.select();
    _modelList[idx].select();
    emit dataChanged(index, index);

    return true;
}

//-----------------------------------------------------------------

QHash<int, QByteArray> ByteTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ChRole] = "chdata";
    roles[HxRole] = "hxdata";
    roles[BnRole] = "bndata";
    roles[ValidRole] = "valid";
    roles[MSecRole] = "msec";
    roles[SelectRole] = "sel";
    return roles;
}

//-----------------------------------------------------------------

void ByteTableModel::addByte(const TByte &byte)
{
    append(byte);
    checkRowCount();
    beginInsertRows(QModelIndex(), _rowCount - 1, _rowCount - 1);
    endInsertRows();

    _byteCount++;
    emit byteCountChanged();
}

//-----------------------------------------------------------------

void ByteTableModel::addByte(quint8 byte)
{
    if (_timer.isValid() == false) _timer.start();
    addByte(TByte(byte, _timer.restart()));
}

//-----------------------------------------------------------------

void ByteTableModel::addBytes(const QByteArray &data)
{
    if (_timer.isValid() == false) _timer.start();

    foreach (quint8 byte, data)
        append(TByte(byte, _timer.restart()));

    int rowCount = _rowCount;
    checkRowCount();
    beginInsertRows(QModelIndex(), rowCount - 1, _rowCount - 1);
    endInsertRows();

    _byteCount += data.size();
    emit byteCountChanged();
}

//-----------------------------------------------------------------

void ByteTableModel::setColumnCount(int count)
{
    if (count <= 0 || count == _columnCount) return;

    unselect();
    _columnCount = count;
    emit columnCountChanged();

    checkRowCount();
    compose();
}

//-----------------------------------------------------------------

void ByteTableModel::setNewLineMode(NewLineMode mode)
{
    if (mode == _mode) return;

    unselect();
    _mode = mode;
    compose();
}

//-----------------------------------------------------------------

void ByteTableModel::setNewLineMode(int mode)
{
    NewLineMode m = static_cast<NewLineMode>(mode);
    if (m < NewLineMode::None || m > NewLineMode::Timeout) return;

    setNewLineMode(m);
}

//-----------------------------------------------------------------

void ByteTableModel::setTimeout(int tim)
{
    if (tim < 0) return;

    unselect();
    _timeout = tim;
    emit timeoutChanged();

    if (_timeout > 0) compose();
}

//-----------------------------------------------------------------

void ByteTableModel::clear()
{
    _timer.invalidate();

    beginRemoveRows(QModelIndex(), 0, _rowCount - 1);
    _modelList.clear();
    checkRowCount();
    endRemoveRows();

    _byteCount = 0;
    emit byteCountChanged();
}

//-----------------------------------------------------------------

void ByteTableModel::select(int row, int col)
{
    if (row < 0 || col < 0 || row > _rowCount - 1 || col > _columnCount - 1)
        return;

    if ((_startSelectionIdx >= 0) && (_endSelectionIdx >= 0) && (_startSelectionIdx != _endSelectionIdx))
        unselect();

    int idx = row * _columnCount + col;

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
        if (_modelList[i].isValid()) _modelList[i].select();
    }

    QModelIndex startSelection = index((_startSelectionIdx / _columnCount), (_startSelectionIdx % _columnCount));
    QModelIndex endSelection = index((_endSelectionIdx / _columnCount), (_endSelectionIdx % _columnCount));

    if (_startSelectionIdx == _endSelectionIdx)
        emit dataChanged(startSelection, startSelection);
    else
        emit dataChanged(index(startSelection.row(), 0), index(endSelection.row(), _columnCount - 1));

}

//-----------------------------------------------------------------

void ByteTableModel::unselect()
{
    if (_startSelectionIdx < 0) return;
    if (_endSelectionIdx < 0) _endSelectionIdx = _startSelectionIdx;

    for (int i = _startSelectionIdx; i <= _endSelectionIdx; ++i)
    {
        if (_modelList[i].isValid()) _modelList[i].unselect();
    }

    QModelIndex startSelection = index((_startSelectionIdx / _columnCount), (_startSelectionIdx % _columnCount));
    QModelIndex endSelection = index((_endSelectionIdx / _columnCount), (_endSelectionIdx % _columnCount));

    if (_startSelectionIdx == _endSelectionIdx)
        emit dataChanged(startSelection, startSelection);
    else
        emit dataChanged(index(startSelection.row(), 0), index(endSelection.row(), _columnCount - 1));

    _startSelectionIdx = -1;
    _endSelectionIdx = -1;
}

//-----------------------------------------------------------------

QJsonObject ByteTableModel::getData(int row, int col) const
{
    int pos = row * _columnCount + col;
    if (pos < 0 || pos > _modelList.size() - 1) return QJsonObject();

    int startIdx = -1;
    int endIdx = -1;

    if (_startSelectionIdx >= 0)
        startIdx = _startSelectionIdx;
    if (_endSelectionIdx >= 0)
        endIdx = _endSelectionIdx;

    if (startIdx < 0 || endIdx < 0 || pos < startIdx || pos > endIdx)
    {
        startIdx = pos;
        endIdx = pos;
    }

    QString ascii;
    QString hex;
    QString bin;
    int size = 0;

    for (int i = startIdx; i <= endIdx; ++i)
    {
        if (_modelList[i].isValid() == false) continue;

        ascii += _modelList[i].toChar();
        hex += _modelList[i].toHex() + " ";
        bin += _modelList[i].toBin() + " ";
        size++;
    }

    QJsonObject obj {
        { "ascii", ascii },
        { "hex", hex.trimmed() },
        { "bin", bin.trimmed() },
        { "size", size }
    };

    return obj;
}

//-----------------------------------------------------------------

void ByteTableModel::checkRowCount()
{
    _rowCount = qCeil(static_cast<float>(_modelList.size()) / _columnCount);
}

//-----------------------------------------------------------------

void ByteTableModel::append(const TByte &byte)
{
    auto fillEmpty {
        [this]()
        {
            int len = _columnCount - (_modelList.size() % _columnCount);
            if (len < _columnCount)
                for (int i = 0; i < len; ++i)
                    _modelList.append(TByte());

        }
    };

    if (_mode == NewLineMode::CRLF && byte.byte() == '\n' && _modelList.size() > 1
        && _modelList.last().byte() == '\r')
    {
        _modelList << byte;
        fillEmpty();

        return;
    }

    if ((_mode == NewLineMode::LF && byte.byte() == '\n')
        || (_mode == NewLineMode::CR && byte.byte() == '\r' )
        || (_mode == NewLineMode::Null && byte.byte() == '\0'))
    {
        _modelList << byte;
        fillEmpty();

        return;
    }

    if (_mode == NewLineMode::Timeout && _timeout > 0 && byte.getMSec() >= _timeout)
    {
        fillEmpty();
        _modelList << byte;

        return;
    }

    _modelList << byte;
}

//-----------------------------------------------------------------

void ByteTableModel::compose()
{
    reset();
    if (_mode == NewLineMode::None) return;
    if (_mode == NewLineMode::CR) transform('\r');
    if (_mode == NewLineMode::LF) transform('\n');
    if (_mode == NewLineMode::Null) transform('\0');
    if (_mode == NewLineMode::CRLF) transform('\r', '\n');
    if (_mode == NewLineMode::Timeout) transform();

    beginRemoveRows(QModelIndex(), 0, _rowCount - 1);
    endRemoveRows();

    checkRowCount();
    beginInsertRows(QModelIndex(), 0, _rowCount - 1);
    endInsertRows();
}

//-----------------------------------------------------------------

void ByteTableModel::transform(quint8 f)
{
    if (_modelList.isEmpty()) return;

    QList<TByte>::iterator iter = _modelList.begin();
    while (iter != _modelList.end())
    {
        if (iter->byte() == f && iter->isValid())
        {
            iter++;
            int pos = iter - _modelList.begin();
            int len = _columnCount - (pos % _columnCount);
            if (len == _columnCount) { iter++; continue; }
            for (int i = 0; i < len; ++i) iter = _modelList.insert(iter, TByte());
        } else
        {
            iter++;
        }
    }

}

//-----------------------------------------------------------------

void ByteTableModel::transform(quint8 f1, quint8 f2)
{
    if (_modelList.size() < 2) return;

    QList<TByte>::iterator iter = _modelList.begin() + 1;
    while (iter != _modelList.end())
    {
        if (iter->byte() == f2 && iter->isValid() && (iter - 1)->byte() == f1 && (iter - 1)->isValid())
        {
            iter++;
            int pos = iter - _modelList.begin();
            int len = _columnCount - (pos % _columnCount);
            if (len == _columnCount) { iter++; continue; }
            for (int i = 0; i < len; ++i) iter = _modelList.insert(iter, TByte());
        } else
        {
            iter++;
        }
    }
}

//-----------------------------------------------------------------

void ByteTableModel::transform()
{
    if (_timeout <= 0 || _modelList.size() < 2) return;

    QList<TByte>::iterator iter = _modelList.begin() + 1;
    while (iter != _modelList.end())
    {
        if (iter->getMSec() >= _timeout && iter->isValid())
        {
            int pos = iter - _modelList.begin();
            int len = _columnCount - (pos % _columnCount);
            if (len == _columnCount) { iter++; continue; }
            for (int i = 0; i < len; ++i) iter = _modelList.insert(iter, TByte());
        }
        iter++;
    }
}

//-----------------------------------------------------------------

void ByteTableModel::reset()
{
    QList<TByte>::iterator iter = _modelList.begin();
    while (iter != _modelList.end())
    {
        if (iter->isValid() == false) iter = _modelList.erase(iter);
        else iter++;
    }

    beginRemoveRows(QModelIndex(), 0, _rowCount - 1);
    endRemoveRows();

    checkRowCount();
    beginInsertRows(QModelIndex(), 0, _rowCount - 1);
    endInsertRows();
}


//-----------------------------------------------------------------

}   // namespace client

