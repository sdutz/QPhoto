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

#ifndef COLLECTIONDLG_H
#define COLLECTIONDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "collectionmgr.h"

namespace Ui {
class CollectionDlg;
}

class CollectionDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CollectionDlg(QWidget *parent = 0);
    ~CollectionDlg();
    void SetMgr( CollectionMgr* pMgr) ;
    void DoShow( const QPoint& pos, double dHeight) ;
    void DoHide() ;


private slots:
    void on_recordsList_itemDoubleClicked(QListWidgetItem *item);

private :
    void PopulateRecordsList() ;

private:
    Ui::CollectionDlg* ui;
    CollectionMgr*     m_pCollMgr ;
};

#endif // COLLECTIONDLG_H
