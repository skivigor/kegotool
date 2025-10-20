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
#ifndef HASHCALC_H
#define HASHCALC_H

#include <QObject>
#include <QStringList>
#include <QCryptographicHash>
#include <QByteArray>

namespace checksum
{

class HashCalc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool        processing READ isProcessing NOTIFY processingChanged)
    Q_PROPERTY(QStringList hashTypes  READ getHashTypes CONSTANT)
    Q_PROPERTY(QByteArray  result     READ getResult    NOTIFY resultChanged)
    Q_PROPERTY(QString     hexResult  READ getHexResult NOTIFY hexResultChanged)

public:
    explicit HashCalc(QObject *parent = nullptr);
    ~HashCalc();

    Q_INVOKABLE bool        isProcessing() const { return _processing; }
    Q_INVOKABLE QStringList getHashTypes() const { return _types; }
    Q_INVOKABLE QByteArray  getResult() const    { return _result; }
    Q_INVOKABLE QString     getHexResult() const { return _hexResult; }
    Q_INVOKABLE void        resetResult();

signals:
    void processingChanged();
    void resultChanged();
    void hexResultChanged();

public slots:
    bool calculate(const QString &algorithmName, const QByteArray &data);
    bool calculate(const QString &algorithmName, const QString &path);
    bool calculate(QCryptographicHash::Algorithm algorithm, const QByteArray &data);
    bool calculate(QCryptographicHash::Algorithm algorithm, const QString &path);

private:
    void init();

private:
    QStringList  _types;
    bool         _processing = false;
    QByteArray   _result;
    QString      _hexResult = "0x0";

};


}   // namespace checksum


#endif // HASHCALC_H
