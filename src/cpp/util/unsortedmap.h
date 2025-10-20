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
#ifndef UNSORTEDMAP_H
#define UNSORTEDMAP_H

#include <QList>
#include <QPair>

namespace util
{

template<typename K, typename V>
class UnsortedMap
{
public:
    UnsortedMap() {}

    int  size() const    { return _list.size(); }
    bool isEmpty() const { return _list.isEmpty(); }
    void clear()         { _list.clear(); }

    bool contains(const K &key) const
    {
        for (int i = 0; i < _list.size(); ++i)
            if (_list.at(i).first == key) return true;

        return false;
    }

    void remove(const K &key)
    {
        for (int i = 0; i < _list.size(); ++i)
            if (_list.at(i).first == key) { _list.removeAt(i); return; }
    }

    bool append(const K &key, const V &val)
    {
        if (contains(key) == true) return false;

        _list.append(QPair(key, val));
        return true;
    }

    V value(const K &key) const
    {
        for (int i = 0; i < _list.size(); ++i)
            if (_list.at(i).first == key) return _list.at(i).second;

        return V();
    }

    QList<K> keys() const
    {
        QList<K> list;
        for (int i = 0; i < _list.size(); ++i) list.append(_list.at(i).first);

        return list;
    }

private:
    QList<QPair<K, V>> _list;
};

}   // namespace util

#endif // UNSORTEDMAP_H
