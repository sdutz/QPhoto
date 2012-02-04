/*
    QPhoto: a small gallery generator
    Copyright (C) <Lorenzo Zambelli>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_H
#define UTIL_H

#include <QSize>
#include <QStringList>


void GetPixBtnSize(          const QSize& btnSize, QSize* pPixSize) ;
void DoDebug(                const QString& szDebug) ;
bool FromStringToStringList( const QString& szList, QStringList* plszRes) ;
bool FromStringListToString( const QStringList& lszList, QString* pszRes) ;

#endif // UTIL_H
