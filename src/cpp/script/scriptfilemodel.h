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
#ifndef SCRIPTFILEMODEL_H
#define SCRIPTFILEMODEL_H

#include <QAbstractListModel>
#include <QList>

namespace script
{

struct ScriptFileItem
{
    ScriptFileItem(const QString &fileName = QString(), const QString &filePath = QString())
        : FileName(fileName),
          FilePath(filePath) {}

    QString  FileName;
    QString  FilePath;
};

class ScriptFileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ScriptFileModel(const QString &subDir = QString(), QObject *parent = nullptr);
    ~ScriptFileModel();

    using Roles = QHash<int, QByteArray>;

    // QAbstractListModel impl
    Roles    roleNames() const override;
    int      rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = ModelRoles::FileNameRole) const override;

    ScriptFileItem getItem(int idx) const;

    Q_INVOKABLE void    updateModel();
    Q_INVOKABLE QString addItem(const QString &fileName);
    Q_INVOKABLE bool    delItem(const QString &fileName);
    Q_INVOKABLE bool    delItem(int idx);
    Q_INVOKABLE int     size() const { return _modelList.size(); }
    Q_INVOKABLE void    clear();

signals:

private:
    enum ModelRoles
    {
        FileNameRole = Qt::UserRole + 50,
        FilePathRole
    };

private:
    QList<ScriptFileItem> _modelList;
    QString               _scriptDirPath;


};


}   // namespace script


#endif // SCRIPTFILEMODEL_H
