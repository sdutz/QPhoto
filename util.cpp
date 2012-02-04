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

#include "util.h"
#include <QMessageBox>


//----------------------------------------------------
#define PIX_OFFS   1

//----------------------------------------------------
#define CHECK_PTR_PARAM( p)   if ( p == NULL) return false ;

//----------------------------------------------------
void GetPixBtnSize( const QSize& btnSize, QSize* pPixSize)
{
    int nMin ;

    nMin = qMin( btnSize.width(), btnSize.height()) - PIX_OFFS ;
    pPixSize->setHeight( nMin);
    pPixSize->setWidth(  nMin);
}


//----------------------------------------------------
void DoDebug( const QString& szDebug)
{
    QMessageBox box ;

    box.setText( szDebug);
    box.exec() ;
}

//----------------------------------------------------
bool FromStringListToString( const QStringList& lszList, QString* pszRes) {

    CHECK_PTR_PARAM( pszRes)

    pszRes->clear();

    for ( int n = 0 ;  n < lszList.count()  ; n ++)
        pszRes->append( "%1,").arg( lszList.at(n)) ;

    return true ;
}


//----------------------------------------------------
bool FromStringToStringList( const QString& szList, QStringList* plszRes)
{
    int nPos ;
    int nCurr ;

    CHECK_PTR_PARAM( plszRes)

    plszRes->clear();

    nPos = 0 ;
    while ( nPos < szList.count()) {

        nCurr = szList.indexOf( ",", nPos) ;
        plszRes->append( szList.mid( nPos, nCurr - nPos));
        nPos  = nCurr ;
    }

    return true ;
}
