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
#include <QDir>


//----------------------------------------------------
#define PIX_OFFS   1


//----------------------------------------------------
bool GetPixBtnSize( const QSize& btnSize, QSize* pPixSize)
{
    CHECK_PTR_PARAM( pPixSize)

    int nMin ;

    nMin = qMin( btnSize.width(), btnSize.height()) - PIX_OFFS ;
    pPixSize->setHeight( nMin) ;
    pPixSize->setWidth(  nMin) ;

    return true ;
}

//----------------------------------------------------
void DoDebug( const QString& szDebug)
{
    QMessageBox box ;

    box.setText( szDebug);
    box.exec() ;
}

//----------------------------------------------------
bool DirToFileList( const QString& szDir, const QString& szValExt, QStringList* plszFiles)
{
    CHECK_ST_PARAM( szDir)
    CHECK_PTR_PARAM( plszFiles)

    int           n ;
    int           nTot ;
    QString       szExt ;
    QDir          dir( szDir) ;
    QFileInfo     fileInfo ;
    QFileInfoList list ;

    dir.setFilter(  QDir::Files | QDir::NoSymLinks) ;
    dir.setSorting( QDir::Type) ;

    list = dir.entryInfoList() ;
    nTot = list.count() ;

    for ( n = 0 ;  n < nTot ;  n ++) {
        fileInfo = list.at( n) ;
        szExt    = fileInfo.suffix().toLower() ;
        if ( szValExt.isEmpty()  ||  szValExt.indexOf( szExt) >= 0)
            plszFiles->append( fileInfo.filePath()) ;
    }

    return true ;
}

