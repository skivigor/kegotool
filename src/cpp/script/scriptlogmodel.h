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
#ifndef SCRIPTLOGMODEL_H
#define SCRIPTLOGMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QTime>
#include <QJsonArray>

namespace script
{

struct ScriptLogItem
{
    ScriptLogItem(const QString &mes)
        : Time(QTime::currentTime().toString("hh:mm:ss.zzz")),
          Message(mes) {}

    QString Time;
    QString Message;
    bool    Selected = false;
};


class ScriptLogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ScriptLogModel(QObject *parent = nullptr);
    ~ScriptLogModel();

    using Roles = QHash<int, QByteArray>;

    // QAbstractListModel impl
    Roles    roleNames() const override;
    int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = ModelRoles::MessageRole) const override;

    void addItem(const ScriptLogItem &itm);
    void addItem(const QString &message);
    int  size() const { return _modelList.size(); }

    Q_INVOKABLE QJsonArray getData(int idx);
    Q_INVOKABLE void       select(int idx);
    Q_INVOKABLE void       unselect();
    Q_INVOKABLE void       clear();

signals:

private:
    enum ModelRoles
    {
        TimeRole = Qt::UserRole + 40,
        MessageRole,
        SelectRole
    };

private:
    QList<ScriptLogItem> _modelList;

    int _startSelectionIdx = -1;
    int _endSelectionIdx = -1;

};


}   // namespace script


#endif // SCRIPTLOGMODEL_H
