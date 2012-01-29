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
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

//----------------------------------------------------
#define DATABASE        "Db.db"
#define SQLITE          "QSQLITE"
#define TABLENAME       "PICTURECOLLECTION"

//----------------------------------------------------
#define ERR_NO                  0
#define ERR_CREATECONNECTION    1
#define ERR_OPENDB              2
#define ERR_POPULATEDB          3


//----------------------------------------------------
CollectionMgr::CollectionMgr() {

    m_nLastErr = ERR_NO ;
    InitDb();

}


//----------------------------------------------------
CollectionMgr::~CollectionMgr() {
    CloseDb();
}


//----------------------------------------------------
bool CollectionMgr::InitDb() {
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
bool CollectionMgr::PopulateDb() {

    QSqlQuery qQuery ;

    m_szQuery = QString( "create table %s( STRING name, STRING files)").arg( TABLENAME) ;
    qQuery    = m_db.exec( m_szQuery) ;

    return true ;
}

//----------------------------------------------------
bool CollectionMgr::CloseDb() {
    m_db.close();

    return true ;
}

//----------------------------------------------------
QString CollectionMgr::GetLastErr() {
    QString   szLog ;
    QSqlError qErr ;

    qErr = m_db.lastError() ;

    szLog = QString( "%d:").arg( m_nLastErr) ;
    szLog += qErr.text() ;

    return szLog  ;
}
