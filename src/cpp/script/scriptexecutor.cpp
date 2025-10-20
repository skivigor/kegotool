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
#include "scriptexecutor.h"
#include <QThread>
#include <QQmlEngine>

#include <QTime>
#include <QCoreApplication>

#include <QFile>

#include <QDebug>

namespace script
{

ScriptExecutor::ScriptExecutor(QObject *parent)
    : QObject{parent}
{
    qDebug() << "ScriptExecutor::ScriptExecutor: ctor";
    _engine.installExtensions(QJSEngine::ConsoleExtension);

    scrObj = _engine.newQObject(this);
    _engine.globalObject().setProperty("scr", scrObj);
    _engine.evaluate(
        "function wait(ms) {"
        "    scr.wait(ms)"
        "}");
    _engine.evaluate(
        "function write(data) {"
        "    scr.write(data)"
        "}");

    _engine.evaluate(
        "function read() {"
        "    return scr.read()"
        "}");
    _engine.evaluate(
        "function log(mes) {"
        "    scr.log(mes)"
        "}");

}

ScriptExecutor::~ScriptExecutor()
{
    QQmlEngine::setObjectOwnership(scrObj.toQObject(), QQmlEngine::CppOwnership);
    qDebug() << "ScriptExecutor::~ScriptExecutor: dtor";
}

//-----------------------------------------------------------------

void ScriptExecutor::execute(const QString &path)
{
    qDebug() << "ScriptExecutor::execute: " << QThread::currentThread();

    QString scr;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly) == false)
    {
        qDebug() << "Open file ERROR";
        emit error();
        return;
    }
    scr = file.readAll();
    file.close();

    QJSValue res = _engine.evaluate(scr);
    if (res.isError())
    {
        QString err = "Exception at line " + res.property("lineNumber").toString() + ": " + res.toString();
        log(err);
        qDebug() << "ScriptExecutor::execute: " << err;
        emit error();
        return;
    }

    emit complete();
}

//-----------------------------------------------------------------

void ScriptExecutor::onReceived(const QByteArray &data)
{
    qDebug() << "!!!!!!!! ScriptExecutor::onReceived: " << data.toHex();
    _resp.append(data);
}

//-----------------------------------------------------------------
void ScriptExecutor::wait(int ms)
{
    QTime time = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < time)
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            _engine.setInterrupted(true);
            return;
        }
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        QThread::msleep(10);
    }
}

//-----------------------------------------------------------------

void ScriptExecutor::log(const QVariant &mes)
{
    if (QThread::currentThread()->isInterruptionRequested())
    {
        _engine.setInterrupted(true);
        return;
    }

    emit message(mes.toString());
}

//-----------------------------------------------------------------

void ScriptExecutor::write(const QByteArray &data)
{
    if (QThread::currentThread()->isInterruptionRequested())
    {
        _engine.setInterrupted(true);
        return;
    }

    emit send(data);
}

//-----------------------------------------------------------------

QByteArray ScriptExecutor::read()
{
    if (QThread::currentThread()->isInterruptionRequested())
    {
        _engine.setInterrupted(true);
        return QByteArray();
    }

    QByteArray ba(_resp);
    _resp.clear();

    return ba;
}

//-----------------------------------------------------------------

}   // namespace script

