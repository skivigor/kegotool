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
#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class QQmlApplicationEngine;

namespace stor
{

class AppStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonArray  styleList READ getStyleList CONSTANT)
    Q_PROPERTY(QString     styleName READ getStyleName NOTIFY styleNameChanged)
    Q_PROPERTY(QJsonObject style     READ getStyle     CONSTANT)

public:
    explicit AppStyle(QQmlApplicationEngine &engine,
                      const QString &stylesDir = QString(),
                      QObject *parent = nullptr);
    ~AppStyle();

    Q_INVOKABLE QJsonArray  getStyleList() const  { return _styleList; }
    Q_INVOKABLE QString     getStyleName() const  { return _styleName; }
    Q_INVOKABLE QJsonObject getStyle() const      { return _styleConfig; }
    Q_INVOKABLE bool        setStyle(int idx);

signals:
    void styleNameChanged();

private:
    void initStyle();

private:
    QQmlApplicationEngine &_engine;
    QString                _stylesPath;
    QString                _styleName = "Dark";
    QJsonObject            _styleConfig;
    QJsonArray             _styleList;

};


}   // namespace stor


#endif // APPSTYLE_H
