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


#ifndef CONFMGR_H
#define CONFMGR_H

#include "collectionmgr.h"
#include <QString>
#include <QSettings>
#include <QStringList>
#include <QColor>
#include <QFont>

//----------------------------------------------------
#define NUM_STR_PROP 5
#define NUM_INT_PROP 3


//----------------------------------------------------
#define PROP_STR_COLOR         0
#define PROP_STR_FONT          1
#define PROP_STR_SONGS         2
#define PROP_STR_LAST_DIR      3
#define PROP_STR_LAST_DIR_LIST 4

//----------------------------------------------------
#define PROP_INT_SEC   0
#define PROP_INT_FADE  1
#define PROP_INT_LANG  2

//----------------------------------------------------
#define FADE_NONE        0
#define FADE_ONSLIDESHOW 1
#define FADE_ALWAYS      2

//----------------------------------------------------
#define ENGLISH 0
#define ITALIAN 1



class ConfMgr
{
public:
    ConfMgr();
    ~ConfMgr();
    bool            SetDbMgr( CollectionMgr* pMgr) ;
    void            WriteList(      const QString& szFile = "", bool bSaveDir = false) ;
    void            LoadList(       const QString& szFile = "") ;
    bool            FindInList(     const QString& szFile, int* pnIdx = NULL) ;
    void            AddToList(      const QString& szFile) ;
    void            RemoveFromList( const QString& szFile) ;
    void            ClearList() ;
    QString         GetListItem(  int n) ;
    int             GetItemCount( void) ;
    void            ShowSettingsDlg() ;
    bool            GetStrProp( int nProp, QString* pVal) ;
    bool            GetIntProp( int nProp, int* pVal) ;
    bool            GetHelpFromFile( QString* pszHelp) ;
    QString         GetLog() ;
    bool            WriteLog( const QString szLog) ;
    bool            ResetLog() ;

private :
    void            LoadSettings() ;
    void            WriteSettings() ;

private :
    CollectionMgr*  m_pDbMgr ;
    QStringList     m_lszList ;
    QString         m_aStrProp[NUM_STR_PROP] ;
    int             m_aIntProp[NUM_INT_PROP] ;
};

#endif // CONFMGR_H
