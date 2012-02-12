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
#define LOG_FILE           "log.txt"
#define LAST_DIR           "LastDir"
#define LAST_DIR_LIST      "LastDirList"
#define COLOR              "Color"
#define FONT               "Font"
#define SONGS              "Songs"
#define LANG               "Lang"
#define SLIDESHOWSEC       "SlideShowSec"
#define FADETYPE           "Fade Type"


//----------------------------------------------------
ConfMgr::ConfMgr()
{
    LoadSettings() ;
}

//----------------------------------------------------
ConfMgr::~ConfMgr()
{
    WriteSettings() ;
}

//----------------------------------------------------
bool  ConfMgr::SetDbMgr( CollectionMgr* pMgr)
{
    m_pDbMgr = pMgr ;

    return m_pDbMgr != NULL ;
}

//----------------------------------------------------
void ConfMgr::WriteList( const QString& szFile, bool bSaveDir)
{
    int     nIdx ;
    int     nItems ;
    QString szMyFile ;

    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;
    if ( bSaveDir) {
        nIdx = szFile.lastIndexOf( "/") ;
        m_aStrProp[ PROP_STR_LAST_DIR_LIST] = nIdx > 0 ? szFile.left( nIdx) : "" ;
    }

    m_pDbMgr->InsertItem( szFile, m_lszList) ;

    QFile cFile( szMyFile) ;

    nItems = m_lszList.count() ;

    if( ! cFile.open( QIODevice::WriteOnly | QIODevice::Text))
        return ;

    QTextStream out( &cFile) ;

    for( nIdx = 0 ;  nIdx < nItems ;  nIdx ++)
        out<<m_lszList.at( nIdx)<<endl ;

    cFile.close();
}

//----------------------------------------------------
void ConfMgr::LoadList( const QString& szFile)
{
    QString szTmp ;
    QString szMyFile ;

    ClearList() ;
    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;

    QFile cFile( szMyFile) ;

    if ( ! cFile.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

    QTextStream in( &cFile) ;

    while ( ! in.atEnd()) {
         szTmp = in.readLine() ;
         m_lszList.append( szTmp) ;
    }

    cFile.close() ;

    m_pDbMgr->InsertItem( szFile, m_lszList) ;
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

    nIdx = szFile.lastIndexOf( "\\") ;
    m_aStrProp[ PROP_STR_LAST_DIR] = szFile.left( nIdx) ;
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
void  ConfMgr::ClearList()
{
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
    QColor    DefColor ;
    QFont     DefFont ;

    DefColor  = Qt::black ;
    szBlack   = DefColor.name() ;

    m_aIntProp[ PROP_INT_SEC]           = cSet.value( SLIDESHOWSEC, 5).toInt() ;
    m_aIntProp[ PROP_INT_FADE]          = cSet.value( FADETYPE, 0).toInt() ;
    m_aIntProp[ PROP_INT_LANG]          = cSet.value( LANG, ENGLISH).toInt() ;
    m_aStrProp[ PROP_STR_COLOR]         = cSet.value( COLOR, szBlack).toString() ;
    m_aStrProp[ PROP_STR_FONT]          = cSet.value( FONT, DefFont.toString()).toString() ;
    m_aStrProp[ PROP_STR_SONGS]         = cSet.value( SONGS, "").toString() ;
    m_aStrProp[ PROP_STR_LAST_DIR]      = cSet.value( LAST_DIR, "").toString() ;
    m_aStrProp[ PROP_STR_LAST_DIR_LIST] = cSet.value( LAST_DIR_LIST, "").toString() ;
}

//----------------------------------------------------
void ConfMgr::WriteSettings()
{
    QSettings cSet ;

    cSet.setValue( SLIDESHOWSEC, m_aIntProp[ PROP_INT_SEC]) ;
    cSet.setValue( FADETYPE, m_aIntProp[ PROP_INT_FADE]) ;
    cSet.setValue( LANG, m_aIntProp[ PROP_INT_LANG]) ;
    cSet.setValue( COLOR, m_aStrProp[PROP_STR_COLOR])  ;
    cSet.setValue( FONT, m_aStrProp[PROP_STR_FONT]) ;
    cSet.setValue( SONGS, m_aStrProp[PROP_STR_SONGS]) ;
    cSet.setValue( LAST_DIR, m_aStrProp[ PROP_STR_LAST_DIR]) ;
    cSet.setValue( LAST_DIR_LIST, m_aStrProp[ PROP_STR_LAST_DIR_LIST]) ;
}

//----------------------------------------------------
void ConfMgr::ShowSettingsDlg()
{
    SettingsDlg    cDlg ;
    QPhotoSettings sets ;

    sets.nSec      = m_aIntProp[ PROP_INT_SEC] ;
    sets.nFadeType = m_aIntProp[ PROP_INT_FADE] ;
    sets.nLang     = m_aIntProp[ PROP_INT_LANG] ;
    sets.szColor   = m_aStrProp[ PROP_STR_COLOR] ;
    sets.szFont    = m_aStrProp[ PROP_STR_FONT] ;
    sets.szSongs   = m_aStrProp[ PROP_STR_SONGS] ;

    cDlg.SetInitSettings( sets) ;

    if ( cDlg.exec() == QDialog::Accepted) {
        sets = cDlg.GetSettings() ;
        m_aIntProp[ PROP_INT_SEC]   = sets.nSec ;
        m_aIntProp[ PROP_INT_FADE]  = sets.nFadeType ;
        m_aIntProp[ PROP_INT_LANG]  = sets.nLang ;
        m_aStrProp[ PROP_STR_COLOR] = sets.szColor ;
        m_aStrProp[ PROP_STR_FONT]  = sets.szFont ;
        m_aStrProp[ PROP_STR_SONGS] = sets.szSongs ;
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

//----------------------------------------------------
QString ConfMgr::GetLog( )
{
    QFile cFile( LOG_FILE) ;

    if ( ! cFile.open( QIODevice::ReadOnly | QIODevice::Text))
        return "" ;

    QByteArray  pByte ;

    pByte = cFile.readAll() ;

    QTextStream stream( pByte, QIODevice::ReadOnly) ;

    QString szRet = stream.readAll() ;
    cFile.close();

    return szRet ;
}

//----------------------------------------------------
bool ConfMgr::WriteLog( const QString szLog)
{
    QFile cFile( LOG_FILE) ;

    if ( ! cFile.open( QIODevice::WriteOnly | QIODevice::Text))
        return false ;

    if ( szLog.isEmpty())
        cFile.write( "") ;

    else {
        QByteArray pByte ;
        pByte.append( szLog) ;
        cFile.write( pByte) ;
    }

    cFile.close() ;

    return true ;
}

//----------------------------------------------------
bool ConfMgr::ResetLog()
{
    return WriteLog( "") ;
}

//----------------------------------------------------
bool ConfMgr::GetStrProp( int nProp, QString* pVal)
{
    if ( nProp < 0  ||  nProp >= NUM_STR_PROP)
        return false ;

    if ( pVal == NULL)
        return false ;

    *pVal = m_aStrProp[nProp] ;

    return true ;
}

//----------------------------------------------------
bool ConfMgr::GetIntProp( int nProp, int* pVal)
{
    if ( nProp < 0  ||  nProp >= NUM_INT_PROP)
        return false ;

    if ( pVal == NULL)
        return false ;

    *pVal = m_aIntProp[nProp] ;

    return true ;
}

