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
#ifndef COMMANDMODEL_H
#define COMMANDMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QJsonObject>
#include "commandstorage.h"

namespace client
{

class CommandModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CommandModel(const QString &storName, QObject *parent = nullptr);
    ~CommandModel();

    using Roles = QHash<int, QByteArray>;

    // QAbstractListModel impl
    Roles    roleNames() const override;
    int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = ModelRoles::CommandRole) const override;

    Q_INVOKABLE void addItem(const QString &descr, const QString &cmd);
    Q_INVOKABLE void removeItem(int idx);
    Q_INVOKABLE void moveUp(int idx);
    Q_INVOKABLE void moveDown(int idx);
    Q_INVOKABLE int  size() const { return _modelList.size(); }
    Q_INVOKABLE void clear();
    Q_INVOKABLE QJsonObject getCommand(int idx) const;

signals:

private:
    enum ModelRoles
    {
        DescrRole = Qt::UserRole + 30,
        CommandRole
    };

private:
    void init();

private:
    QList<QJsonObject> _modelList;
    CommandStorage     _stor;

};


}    // namespace client


#endif // COMMANDMODEL_H
