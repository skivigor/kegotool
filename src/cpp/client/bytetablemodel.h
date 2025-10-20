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
#ifndef BYTETABLEMODEL_H
#define BYTETABLEMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QAbstractTableModel>
#include <QList>
#include <QElapsedTimer>
#include <QJsonObject>


namespace client
{

class TByte
{
public:
    explicit TByte() {}
    explicit TByte(quint8 byte, int msec = 0);

    quint8      byte() const        { return _byte; }
    QString     toChar() const      { return _char; }
    QString     toHex() const       { return _hex; }
    QString     toBin() const       { return _bin; }
    QJsonObject toJson() const;
    bool        isValid() const     { return _valid; }
    bool        isSelected() const  { return _selected; }
    int         getMSec() const     { return _msec; }

    void select()   { _selected = true; }
    void unselect() { _selected = false; }

private:
    quint8  _byte = 0;
    QString _char;
    QString _hex;
    QString _bin;
    bool    _valid = false;
    bool    _selected = false;

    int     _msec = 0;
};


class ByteTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList newLineModes READ getNewLineModes CONSTANT)

    Q_PROPERTY(int timeout     READ getTimeout     WRITE setTimeout     NOTIFY timeoutChanged)
    Q_PROPERTY(int columnCount READ getColumnCount WRITE setColumnCount NOTIFY columnCountChanged)
    Q_PROPERTY(int byteCount   READ getByteCount   NOTIFY byteCountChanged)

public:
    enum class NewLineMode
    {
        None = 0,
        CR,
        LF,
        CRLF,
        Null,
        Timeout
    };
    Q_ENUM(NewLineMode)

    static void registerQml()
    {
        qmlRegisterUncreatableType<ByteTableModel>("NewLineMode", 1, 0, "NewLineMode", "");
    }

    using Roles = QHash<int, QByteArray>;

public:
    explicit ByteTableModel(QObject *parent = nullptr);

    // QAbstractTableModel impl
    Roles    roleNames() const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role) override;
    int      rowCount( const QModelIndex& parent ) const override;
    int      columnCount( const QModelIndex& parent ) const override;

    QStringList getNewLineModes() const { return _newLineModes; }
    void        setNewLineMode(NewLineMode mode);
    void        addByte(const TByte &byte);

    Q_INVOKABLE void addByte(quint8 byte);
    Q_INVOKABLE void addBytes(const QByteArray &data);
    Q_INVOKABLE void setColumnCount(int count);
    Q_INVOKABLE int  getColumnCount() const { return _columnCount; }
    Q_INVOKABLE int  getByteCount() const   { return _byteCount; }
    Q_INVOKABLE void setNewLineMode(int mode);
    Q_INVOKABLE void setTimeout(int tim);
    Q_INVOKABLE int  getTimeout() const { return _timeout; }
    Q_INVOKABLE void clear();

    Q_INVOKABLE void select(int row, int col);
    Q_INVOKABLE void unselect();

    Q_INVOKABLE QJsonObject getData(int row, int col) const;

signals:
    void timeoutChanged();
    void columnCountChanged();
    void byteCountChanged();

private:
    enum ByteRoles
    {
        ChRole = Qt::UserRole + 20,
        HxRole,
        BnRole,
        ValidRole,
        MSecRole,
        SelectRole
    };

private:
    void checkRowCount();
    void append(const TByte &byte);
    void compose();
    void transform(quint8 f);
    void transform(quint8 f1, quint8 f2);
    void transform();
    void reset();

private:
    QList<TByte> _modelList;
    int          _columnCount = 32;
    int          _rowCount = 0;
    int          _byteCount = 0;

    QStringList  _newLineModes;
    NewLineMode  _mode = NewLineMode::None;
    int          _timeout = 100;

    int _startSelectionIdx = -1;
    int _endSelectionIdx = -1;

    QElapsedTimer _timer;

};


}   // namespace client


#endif // BYTETABLEMODEL_H
