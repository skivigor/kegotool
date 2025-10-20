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
#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QObject>
#include <QThread>
#include <QTimer>

//#include "client/iclient.h"
#include "scriptfilemodel.h"
#include "scriptlogmodel.h"


namespace client
{
class IClient;
}   // namespace client


namespace script
{

class ScriptExecutor;

class ScriptEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    explicit ScriptEngine(client::IClient *ptr, const QString &subDir = QString(), QObject *parent = nullptr);
    ~ScriptEngine();

    Q_INVOKABLE void executeScript(const QString &path);
    Q_INVOKABLE void executeScript(int idx);
    Q_INVOKABLE void stopScript();
    Q_INVOKABLE bool isProcessing() const { return _processing; }

    Q_INVOKABLE QObject* getFileModel() { return &_files; }
    Q_INVOKABLE QObject* getLogModel()  { return &_log; }
    Q_INVOKABLE void     clearLog()     { _log.clear(); }

signals:
    void execute(const QString &path);
    void processingChanged();

private slots:
    void onStarted();
    void onFinished();
    void onTimeout();
    void onComplete();
    void onError();
    void onSendData(const QString &data);
    void onSendBytes(const QByteArray &data);
    void onMessage(const QString &mes);

private:
    QThread          _thread;
    client::IClient *_pClient = nullptr;
    ScriptExecutor  *_pExec = nullptr;
    QString          _scriptName;
    ScriptFileModel  _files;
    ScriptLogModel   _log;
    QTimer           _watchdog;
    bool             _processing = false;

};


}   // namespace script


#endif // SCRIPTENGINE_H
