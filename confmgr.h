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

#include <QString>
#include <QSettings>
#include <QStringList>
#include <QColor>

class ConfMgr
{
public:
    ConfMgr();
    ~ConfMgr();
    void            WriteList(      const QString& szFile = "") ;
    void            LoadList(       const QString& szFile = "") ;
    bool            FindInList(     const QString& szFile, int* pnIdx = NULL) ;
    void            AddToList(      const QString& szFile) ;
    void            RemoveFromList( const QString& szFile) ;
    inline QString  GetLastDir() {  return m_szLastDir ; } ;
    void            ClearList() ;
    QString         GetListItem(  int n) ;
    int             GetItemCount( void) ;
    void            ShowSettingsDlg() ;
    QColor          GetColor( void)   { return m_Color ; } ;
    int             GetSeconds( void) { return m_nSec ; } ;
    bool            GetHelpFromFile( QString* pszHelp) ;

private :
    void            LoadSettings() ;
    void            WriteSettings() ;

private :
    QString         m_szLastDir ;
    QStringList     m_lszList ;
    QColor          m_Color ;
    int             m_nSec ;
};

#endif // CONFMGR_H
