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


#ifndef COLLECTIONMGR_H
#define COLLECTIONMGR_H


#include "QtSql/QSqlDatabase"


class CollectionMgr {

public :
    CollectionMgr() ;
    ~CollectionMgr() ;
    QString GetLastErr() ;

private :
    bool InitDb() ;
    bool CloseDb() ;
    bool PopulateDb() ;


private :
    int          m_nLastErr ;
    QString      m_szQuery ;
    QSqlDatabase m_db ;

};


#endif // COLLECTIONMGR_H
