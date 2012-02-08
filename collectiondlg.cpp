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

#include "collectiondlg.h"
#include "ui_collectiondlg.h"


//----------------------------------------------------
CollectionDlg::CollectionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CollectionDlg)
{
    ui->setupUi(this);

    setWindowTitle( tr( "Collection"));
    setFixedWidth( width());
    m_pCollMgr = NULL ;
}

//----------------------------------------------------
CollectionDlg::~CollectionDlg()
{
    delete ui;
}

//----------------------------------------------------
void CollectionDlg::SetMgr( CollectionMgr* pMgr)
{
    if ( pMgr != NULL)
        m_pCollMgr = pMgr ;
}

//----------------------------------------------------
void CollectionDlg::on_recordsList_itemDoubleClicked(QListWidgetItem *item)
{
    ui->filesList->clear();
    ui->filesList->addItems( m_pCollMgr->GetItemData( item->text()));
}

//----------------------------------------------------
void CollectionDlg::DoHide()
{
    hide() ;
    setVisible( false);
}

//----------------------------------------------------
void CollectionDlg::DoShow( const QPoint& pos, double dHeight)
{

    move( pos) ;
    setFixedHeight( dHeight);
    setVisible( true);
    show() ;
    raise();
    activateWindow();

    PopulateRecordsList() ;
}

//----------------------------------------------------
void CollectionDlg::PopulateRecordsList()
{
    ui->recordsList->clear();
    ui->recordsList->addItems( m_pCollMgr->GetItemsList());
    on_recordsList_itemDoubleClicked( ui->recordsList->item(0)) ;
}
