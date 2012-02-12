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
#include <QMoveEvent>
#include "collectionmgr.h"

class CPhoto ;

namespace Ui {
class CollectionDlg;
}

class CollectionDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CollectionDlg(QWidget *parent = 0);
    ~CollectionDlg();
    bool Initialize(      CollectionMgr* pMgr, QDialog* pParent) ;
    void DoShowHide(      bool bShow) ;
    void UpdatePosHeight( const QPoint& pos, int height) ;
    void UpdateDbView() ;
    void RetranslateDialog() ;
    void SetBtnIcons() ;

private slots:
    void on_recordsList_itemDoubleClicked(QListWidgetItem *item) ;
    void on_filesList_itemDoubleClicked(  QListWidgetItem *item) ;
    void moveEvent( QMoveEvent* event) ;
    void on_loadBtn_clicked() ;
    void on_delBtn_clicked() ;

private :
    void    PopulateRecordsList() ;
    void    SetIds() ;
    QString GetCurrTextItem() ;

private:
    Ui::CollectionDlg* ui;
    CollectionMgr*     m_pCollMgr ;
    CPhoto*            m_pParent ;
    QPixmap            m_pix ;
    QPoint             m_pos ;
    bool               m_bAllowMove ;
};

#endif // COLLECTIONDLG_H
