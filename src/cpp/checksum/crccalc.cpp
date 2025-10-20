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
#include "crccalc.h"
#include <QJsonObject>

#include <QtConcurrent>
#include <QThread>
#include <QFuture>
#include <QFutureWatcher>

#include <QFile>
#include <QFileInfo>

#include "util/fsutils.h"
#include "stor/fileloader.h"

#include <QDebug>


namespace
{

const QString DefAlgorithmsPath = QStringLiteral(":/resources/other/crc");

//    QJsonObject {
//        {"name", "CRC16/CCIT_ZERO"},
//        {"poly", "0x1021"},
//        {"init", "0x0"},
//        {"xor", "0x0"}
//        {"refin", false},
//        {"refout", false}
//    });
bool checkCrcConfig(const QJsonObject &cfg)
{
    if (cfg.value("refin").isBool() == false || cfg.value("refout").isBool() == false
        || cfg.value("poly").isString() == false || cfg.value("init").isString() == false
        || cfg.value("xor").isString() == false) return false;

    return true;
}

QString toLocalFile(const QString &path)
{
    return QUrl(path).isLocalFile() ? QUrl(path).toLocalFile() : path;
}

}   // namespace



namespace checksum
{

CrcCalc::CrcCalc(QObject *parent)
    : QObject{parent},
    _algorithmsPath(QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppConfigLocation) + "/crc")
{
    util::FsUtils::copyDir(DefAlgorithmsPath, _algorithmsPath);

    init();
    qDebug() << "CrcCalc::CrcCalc: ctor: " << this;
}

CrcCalc::~CrcCalc()
{
    qDebug() << "CrcCalc::~CrcCalc: dtor: " << this;
}

//-----------------------------------------------------------------

QJsonArray CrcCalc::getCrcTypes() const
{
    return _types.value(_mode);
}

//-----------------------------------------------------------------

bool CrcCalc::setCrcMode(CrcMode::Mode mode)
{
    if (_processing == true) return false;

    if (_mode != mode)
    {
        _mode = mode;
        emit crcTypesChanged();
    }

    return true;
}

//-----------------------------------------------------------------

bool CrcCalc::setCrcMode(unsigned int mode)
{
    CrcMode::Mode v = static_cast<CrcMode::Mode>(mode);

    if (v != CrcMode::CRC8 && v != CrcMode::CRC16
        && v != CrcMode::CRC32 && v != CrcMode::CRC64) return false;

    return setCrcMode(v);
}

//-----------------------------------------------------------------

bool CrcCalc::calculate(unsigned int typeIdx, const QByteArray &data)
{
    QJsonArray types = getCrcTypes();
    if (typeIdx >= types.size()) return false;

    return calculate(types.at(typeIdx).toObject(), data);
}

//-----------------------------------------------------------------

bool CrcCalc::calculate(unsigned int typeIdx, const QString &path)
{
    QJsonArray types = getCrcTypes();
    if (typeIdx >= types.size()) return false;

    return calculate(types.at(typeIdx).toObject(), path);
}

//-----------------------------------------------------------------

bool CrcCalc::calculate(const QJsonObject &crcCfg, const QByteArray &data)
{
    if (_processing == true) return false;
    resetResult();

    if (checkCrcConfig(crcCfg) == false) return false;
    if (data.isEmpty()) return false;

    QFutureWatcher<quint64> *watcher = new QFutureWatcher<quint64>(this);
    QObject::connect(watcher, &QFutureWatcher<quint64>::finished, this,
                     [watcher, this] ()
                     {
                         watcher->deleteLater();
                         quint64 res = watcher->result();

                         _processing = false;
                         emit processingChanged();
                         _result = res;
                         emit resultChanged();

                         _hexResult = /*"0x" + */QString("%1").arg(_result, _mode / 4, 16, QLatin1Char( '0' )).toUpper();
                         emit hexResultChanged();
                     });

    _processing = true;
    emit processingChanged();

    watcher->setFuture(QtConcurrent::run([this](const QJsonObject &cfg, const QByteArray &data) {

        quint64 poly = cfg.value("poly").toString().toULongLong(nullptr, 16);
        quint64 init = cfg.value("init").toString().toULongLong(nullptr, 16);
        quint64 xorb = cfg.value("xor").toString().toULongLong(nullptr, 16);
        bool refin = cfg.value("refin").toBool();
        bool refout = cfg.value("refout").toBool();

        CrcEngine eng(_mode, poly, init, refin, refout, xorb);
        quint64 crc = eng.getCrc(data);
        return crc;

    }, crcCfg, data));

    return true;
}

//-----------------------------------------------------------------

bool CrcCalc::calculate(const QJsonObject &crcCfg, const QString &path)
{
    if (_processing == true) return false;
    resetResult();

    if (checkCrcConfig(crcCfg) == false) return false;
    QString p = toLocalFile(path);
    QFileInfo info(p);
    if (info.isFile() == false || info.isReadable() == false) return false;

    QFutureWatcher<quint64> *watcher = new QFutureWatcher<quint64>(this);
    QObject::connect(watcher, &QFutureWatcher<quint64>::finished, this,
                     [watcher, this] ()
                     {
                         watcher->deleteLater();
                         quint64 res = watcher->result();

                         _processing = false;
                         emit processingChanged();
                         _result = res;
                         emit resultChanged();

                         _hexResult = /*"0x" +*/ QString("%1").arg(_result, _mode / 4, 16, QLatin1Char( '0' )).toUpper();
                         emit hexResultChanged();
                     });

    _processing = true;
    emit processingChanged();

    watcher->setFuture(QtConcurrent::run([this](const QJsonObject &cfg, const QString &path) {

        quint64 poly = cfg.value("poly").toString().toULongLong(nullptr, 16);
        quint64 init = cfg.value("init").toString().toULongLong(nullptr, 16);
        quint64 xorb = cfg.value("xor").toString().toULongLong(nullptr, 16);
        bool refin = cfg.value("refin").toBool();
        bool refout = cfg.value("refout").toBool();
        quint64 crc = 0;

        CrcEngine eng(_mode, poly, init, refin, refout, xorb);

        QFile file(path);
        if (file.open(QIODevice::ReadOnly) == false) return crc;

        int blockSize = 1048576;  // 1Mb
        int count = 1;
        while (file.atEnd() == false)
        {
            QByteArray ba = file.read(blockSize);

            if (count == 1) crc = eng.crcChunkStart(ba);
            else crc = eng.crcChunkProcess(ba, crc);

            count++;
        }
        file.close();

        crc = eng.crcChunkFinish(crc);
        return crc;

    }, crcCfg, p));

    return true;
}

//-----------------------------------------------------------------

void CrcCalc::init()
{
    // CRC8 algorithms
    QJsonArray types = stor::FileLoader::readFileAsJsonArray(_algorithmsPath + "/crc8.json");
    _types.insert(CrcMode::CRC8, types);

    // CRC16 algorithms
    types = stor::FileLoader::readFileAsJsonArray(_algorithmsPath + "/crc16.json");
    _types.insert(CrcMode::CRC16, types);

    // CRC32 algorithms
    types = stor::FileLoader::readFileAsJsonArray(_algorithmsPath + "/crc32.json");
    _types.insert(CrcMode::CRC32, types);

    // CRC64 algorithms
    types = stor::FileLoader::readFileAsJsonArray(_algorithmsPath + "/crc64.json");
    _types.insert(CrcMode::CRC64, types);
}

//-----------------------------------------------------------------

void CrcCalc::resetResult()
{
    _result = 0;
    emit resultChanged();
    _hexResult = "0x0";
    emit hexResultChanged();
}

//-----------------------------------------------------------------

}   // namespace checksum

