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
#include "scriptengine.h"
#include "scriptexecutor.h"
#include "client/iclient.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QUrl>

#include <QDebug>

namespace script
{

ScriptEngine::ScriptEngine(client::IClient *ptr, const QString &subDir, QObject *parent)
    : QObject{parent},
    _pClient(ptr),
    _files(subDir)
{
    QObject::connect(&_thread, &QThread::started, this, &ScriptEngine::onStarted);
    QObject::connect(&_thread, &QThread::finished, this, &ScriptEngine::onFinished);

    _watchdog.setSingleShot(true);
    _watchdog.setInterval(5000);
    QObject::connect(&_watchdog, &QTimer::timeout, this, &ScriptEngine::onTimeout);

    qDebug() << "ScriptEngine::ScriptEngine: ctor: " << this;

}

ScriptEngine::~ScriptEngine()
{
    if (_thread.isRunning()) _thread.terminate();
    if (_pExec != nullptr) delete _pExec;

    qDebug() << "ScriptEngine::~ScriptEngine: dtor";
}

//-----------------------------------------------------------------

void ScriptEngine::executeScript(const QString &path)
{
    qDebug() << "ScriptEngine::executeScript: thread: " << QThread::currentThread();
    if (_pExec != nullptr || path.isEmpty()) return;

    QString p = path;
    if (QUrl(p).isLocalFile()) p = QUrl(p).toLocalFile();

    QFileInfo info(p);
    if (info.exists() == false)
    {
        _log.addItem("File " + info.fileName() + " does not exist");
        return;
    }
    _scriptName = info.fileName();

    using namespace client;

    _pExec = new ScriptExecutor();
    QObject::connect(this, &ScriptEngine::execute, _pExec, &ScriptExecutor::execute);
    QObject::connect(_pExec, &ScriptExecutor::complete, this, &ScriptEngine::onComplete);
    QObject::connect(_pExec, &ScriptExecutor::error, this, &ScriptEngine::onError);
    QObject::connect(_pExec, SIGNAL(send(QByteArray)), dynamic_cast<QObject *>(_pClient), SLOT(send(QByteArray)));
    QObject::connect(dynamic_cast<QObject *>(_pClient), SIGNAL(received(QByteArray)), _pExec, SLOT(onReceived(QByteArray)));
    QObject::connect(_pExec, &ScriptExecutor::message, this, &ScriptEngine::onMessage);

    _pExec->moveToThread(&_thread);
    _thread.start();
    emit execute(p);
}

//-----------------------------------------------------------------

void ScriptEngine::executeScript(int idx)
{
    executeScript(_files.getItem(idx).FilePath);
}

//-----------------------------------------------------------------

void ScriptEngine::stopScript()
{
    if (_pExec == nullptr) return;

    _thread.requestInterruption();
    _watchdog.start();

    _log.addItem("Script " + _scriptName + " interrupt");
}

//-----------------------------------------------------------------

void ScriptEngine::onStarted()
{
    _processing = true;
    emit processingChanged();

    _log.addItem("Script " + _scriptName + " started");
}

//-----------------------------------------------------------------

void ScriptEngine::onFinished()
{
    _watchdog.stop();

    _pExec->disconnect();
    delete _pExec;
    _pExec = nullptr;

    _processing = false;
    emit processingChanged();
}

//-----------------------------------------------------------------

void ScriptEngine::onTimeout()
{
    if (_thread.isRunning() == false) return;
    _thread.terminate();

    _log.addItem("Script " + _scriptName + " terminated");
}

//-----------------------------------------------------------------

void ScriptEngine::onComplete()
{
    qDebug() << "ScriptEngine::onComplete: " << QThread::currentThread();
    _thread.quit();

    _log.addItem("Script " + _scriptName + " finished");
}

//-----------------------------------------------------------------

void ScriptEngine::onError()
{
    qDebug() << "ScriptEngine::onError: " << QThread::currentThread();
    _thread.quit();
}

//-----------------------------------------------------------------

void ScriptEngine::onSendData(const QString &data)
{
    qDebug() << "ScriptEngine::onSendData: " << QThread::currentThread();
    qDebug() << "ScriptEngine::onSendData: " << data;

}

//-----------------------------------------------------------------

void ScriptEngine::onSendBytes(const QByteArray &data)
{
    qDebug() << "ScriptEngine::onSendBytes: " << QThread::currentThread();
    qDebug() << "ScriptEngine::onSendBytes: " << data.toHex();
}

//-----------------------------------------------------------------

void ScriptEngine::onMessage(const QString &mes)
{
//    qDebug() << "ScriptEngine::onMessage: " << mes;
    _log.addItem(mes);
}

//-----------------------------------------------------------------

}   // namespace script

