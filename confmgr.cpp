/*
    QPhoto: a smart gallery generator
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

#include "confmgr.h"
#include <QFile>
#include <QTextStream>
#include "settingsdlg.h"


//----------------------------------------------------
#define HISTORY_FILE       "history.txt"
#define HELP_FILE_ENG      "help-eng.txt"
#define LAST_DIR           "LastDir"
#define COLOR              "Color"
#define SLIDESHOWSEC       "SlideShowSec"


//----------------------------------------------------
ConfMgr::ConfMgr()
{
    LoadSettings() ;
}

ConfMgr::~ConfMgr()
{
    WriteSettings();
}

//----------------------------------------------------
void ConfMgr::WriteList( const QString& szFile)
{
    int     n ;
    int     nItems ;
    QString szMyFile ;

    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;

    QFile cFile( szMyFile) ;

    nItems = m_lszList.count() ;

    if( ! cFile.open( QIODevice::WriteOnly | QIODevice::Text))
        return ;

    QTextStream out( &cFile) ;

    for( n = 0 ;  n < nItems ;  n ++)
        out<<m_lszList.at(n)<<endl ;

    cFile.close();
}

//----------------------------------------------------
void ConfMgr::LoadList( const QString& szFile)
{
    QString szTmp ;
    QString szMyFile ;

    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;

    QFile   cFile( szMyFile) ;

    if ( ! cFile.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

    QTextStream in( &cFile) ;

    while ( ! in.atEnd()) {
         szTmp = in.readLine() ;
         m_lszList.append( szTmp) ;
    }

    cFile.close();
}

//----------------------------------------------------
bool ConfMgr::FindInList( const QString& szFile, int* pnIdx)
{
    int     n ;
    QString szCurr ;

    for ( n = 0 ;  n < m_lszList.count() ;  n ++) {
        szCurr = m_lszList.at( n) ;
        if ( szCurr.compare( szFile) == 0) {
            if ( pnIdx != NULL)
                *pnIdx = n ;
            return true ;
        }
    }

    return false ;
}


//----------------------------------------------------
void ConfMgr::AddToList( const QString& szFile)
{
    int nIdx ;

    nIdx         = szFile.lastIndexOf( "\\") ;
    m_szLastDir  = szFile.left( nIdx) ;
    m_lszList.append( szFile) ;
}

//----------------------------------------------------
void ConfMgr::RemoveFromList( const QString& szFile)
{
    int nToErase ;

    if ( FindInList( szFile, &nToErase))
        m_lszList.takeAt( nToErase) ;
}


//----------------------------------------------------
void  ConfMgr::ClearList() {
    m_lszList.clear();
}


//----------------------------------------------------
QString ConfMgr::GetListItem( int n)
{
    return m_lszList.at(n) ;
}

//----------------------------------------------------
int ConfMgr::GetItemCount( void)
{
    return m_lszList.count() ;
}

//----------------------------------------------------
void ConfMgr::LoadSettings()
{
    QSettings cSet ;
    QString   szBlack ;
    QString   szColor ;
    QColor    DefColor ;

    DefColor = Qt::black ;
    szBlack  = DefColor.name() ;

    m_szLastDir = cSet.value( LAST_DIR, "").toString() ;
    szColor     = cSet.value( COLOR, szBlack).toString() ;
    m_Color.setNamedColor( szColor);
    m_nSec      = cSet.value( SLIDESHOWSEC, 5).toInt() ;
}

//----------------------------------------------------
void ConfMgr::WriteSettings()
{
    QSettings cSet ;

    cSet.setValue( LAST_DIR, m_szLastDir);
    cSet.setValue( COLOR, m_Color.name()) ;
    cSet.setValue( SLIDESHOWSEC, m_nSec);
}

//----------------------------------------------------
void ConfMgr::ShowSettingsDlg()
{
    SettingsDlg cDlg ;

    cDlg.SetInitColor( m_Color) ;
    cDlg.SetInitSeconds( m_nSec) ;

    if ( cDlg.exec() == QDialog::Accepted) {
        m_Color = cDlg.GetColor() ;
        m_nSec  = cDlg.GetSeconds() ;
    }
}

//----------------------------------------------------
bool ConfMgr::GetHelpFromFile( QString* pszHelp)
{
    QFile       file( HELP_FILE_ENG) ;
    QByteArray  pByte ;

    if ( pszHelp == NULL)
        return false ;

    if ( ! file.open( QIODevice::ReadOnly | QIODevice::Text))
        return false ;

    pByte = file.readAll() ;

    QTextStream stream( pByte, QIODevice::ReadOnly) ;

    *pszHelp = stream.readAll() ;
    file.close();

    return true ;
}
