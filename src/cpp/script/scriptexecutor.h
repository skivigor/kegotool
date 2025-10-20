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
#ifndef SCRIPTEXECUTOR_H
#define SCRIPTEXECUTOR_H

#include <QObject>
#include <QJSEngine>
#include <QByteArray>

namespace script
{

class ScriptExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ScriptExecutor(QObject *parent = nullptr);
    ~ScriptExecutor();

    Q_INVOKABLE void wait(int msec);
    Q_INVOKABLE void log(const QVariant &mes);
    Q_INVOKABLE void write(const QByteArray &data);
    Q_INVOKABLE QByteArray read();

public slots:
    void execute(const QString &path);
    void onReceived(const QByteArray &data);

signals:
    void complete();
    void message(const QString &mes);
    void error();
    void send(const QByteArray &data);

private:
    QJSEngine _engine;
    QJSValue scrObj;

    QByteArray _resp;

};


}   // namespace script


#endif // SCRIPTEXECUTOR_H
