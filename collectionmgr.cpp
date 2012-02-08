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


#include "collectionmgr.h"
#include "util.h"
#include <QVariant>
#include <QSqlError>
#include <QStringList>


//----------------------------------------------------
#define DATABASE                "Db.db"
#define SQLITE                  "QSQLITE"
#define TABLENAME               "PICTURECOLLECTION"

//----------------------------------------------------
#define ERR_NO                  0
#define ERR_CREATECONNECTION    1
#define ERR_OPENDB              2
#define ERR_POPULATEDB          3

//----------------------------------------------------
#define NAME                    "name"
#define FILES                   "files"

#define NAME_VAL                 0
#define FILES_VAL                1

//----------------------------------------------------
#define CHECK_ST_PARAM( s)      if ( s.isEmpty()) return false ;


//----------------------------------------------------
CollectionMgr::CollectionMgr()
{
    m_nLastErr = ERR_NO ;
    InitDb();
}


//----------------------------------------------------
CollectionMgr::~CollectionMgr()
{
    CloseDb();
}


//----------------------------------------------------
bool CollectionMgr::InitDb()
{
    m_db = QSqlDatabase::addDatabase( SQLITE) ;

    if ( ! m_db.isValid()) {
        m_nLastErr = ERR_CREATECONNECTION ;
        return false ;
    }

    m_db.setDatabaseName( DATABASE);

    if ( ! m_db.open()) {
        m_nLastErr = ERR_OPENDB ;
        return false ;
    }

    QStringList lszTabs = m_db.tables() ;

    if ( lszTabs.count() < 1) {
        if ( ! PopulateDb()) {
            m_nLastErr = ERR_POPULATEDB ;
            return false ;
        }
    }


    return true ;
}

//----------------------------------------------------
bool CollectionMgr::ExecQuery()
{
    CHECK_ST_PARAM( m_szQuery)

    m_qQuery = m_db.exec( m_szQuery) ;

    return m_qQuery.exec() ;
}

//----------------------------------------------------
bool CollectionMgr::PopulateDb()
{
    m_szQuery = QString( "CREATE TABLE %1( %2 STRING, %3 STRING);").arg( TABLENAME).arg( NAME).arg(FILES) ;

    return ExecQuery() ;
}

//----------------------------------------------------
bool CollectionMgr::CloseDb()
{
    m_db.close();

    return true ;
}


//----------------------------------------------------
bool CollectionMgr::InsertItem( const QString& szName, const QStringList& lszFiles)
{
    QString szList ;

    CHECK_ST_PARAM( szName)
    CHECK_ST_PARAM( lszFiles)

    FromStringListToString( lszFiles, &szList) ;

    m_szQuery = FindItem( szName) ? QString( "UPDATE %1 SET %2 = '%3' WHERE %4 = '%5';").arg(TABLENAME).arg( FILES).arg(szList).arg( NAME).arg(szName) :
                                    QString( "INSERT INTO %1 (%2,%3) VALUES ('%4','%5');").arg(TABLENAME).arg( NAME).arg( FILES).arg( szName).arg( szList) ;

    return ExecQuery() ;
}

//----------------------------------------------------
bool CollectionMgr::DeleteItem( const QString& szName)
{
    CHECK_ST_PARAM( szName)

    m_szQuery = QString( "DELETE FROM %1 WHERE %2 = '%3';").arg( TABLENAME).arg( NAME).arg( szName) ;

    return ExecQuery() ;
}

//----------------------------------------------------
bool CollectionMgr::FindItem( const QString& szName)
{
    CHECK_ST_PARAM( szName)

    m_szQuery = QString( "SELECT * FROM %1 WHERE %2 = '%3';").arg( TABLENAME).arg( NAME).arg( szName) ;

    ExecQuery() ;

    return m_qQuery.first() ;
}

//----------------------------------------------------
QStringList CollectionMgr::GetItemsList()
{
    QStringList lszList ;

    m_szQuery = QString( "SELECT * FROM %1;").arg( TABLENAME) ;

    if ( ! ExecQuery())
        return lszList ;

    while ( m_qQuery.next())
        lszList.append( m_qQuery.value( NAME_VAL).toString()) ;

    return lszList ;
}

//----------------------------------------------------
QStringList CollectionMgr::GetItemData( const QString& szName)
{
    QStringList lszList ;

    if ( ! FindItem( szName))
        return lszList ;

    FromStringToStringList( m_qQuery.value( FILES_VAL).toString(), &lszList) ;

    return lszList ;
}

//----------------------------------------------------
QString CollectionMgr::GetLastErr()
{
    QString   szLog ;
    QSqlError qErr ;

    qErr   = m_db.lastError() ;

    szLog  = QString( "%1:").arg( m_nLastErr) ;
    szLog += qErr.text() ;
    szLog += " Executing " ;
    szLog += m_qQuery.lastQuery() ;

    return szLog  ;
}
