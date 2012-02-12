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
#include "cphoto.h"
#include "util.h"


//----------------------------------------------------
#define BUTTONS     4

//----------------------------------------------------
CollectionDlg::CollectionDlg( QWidget *parent) :
    QDialog( parent),
    ui( new Ui::CollectionDlg)
{
    ui->setupUi( this) ;
    setFixedWidth(  width()) ;
    ui->preView->setAlignment( Qt::AlignCenter) ;
    ui->preView->setText( tr( "Preview")) ;
    m_pCollMgr   = NULL ;
    m_pParent    = NULL ;
    m_bAllowMove = false ;

    SetIds() ;
    SetBtnIcons() ;
    RetranslateDialog() ;

ui->AppendAllBtn->hide() ;
ui->appendBtn->hide() ;
}

//----------------------------------------------------
CollectionDlg::~CollectionDlg()
{
    delete ui ;
}

//----------------------------------------------------
void CollectionDlg::SetIds()
{
    ui->buttons->setId( ui->delBtn,       1) ;
    ui->buttons->setId( ui->loadBtn,      2) ;
    ui->buttons->setId( ui->AppendAllBtn, 3) ;
    ui->buttons->setId( ui->appendBtn,    4) ;
}

//----------------------------------------------------
void CollectionDlg::SetBtnIcons()
{
    QIcon icon ;
    QSize pixSize ;

    GetPixBtnSize( ui->loadBtn->size(), &pixSize) ;
    icon.addFile( "icons/book_go.png", pixSize) ;
    ui->loadBtn->setIcon( icon) ;

    GetPixBtnSize( ui->delBtn->size(), &pixSize) ;
    icon.addFile( "icons/book_delete.png", pixSize) ;
    ui->delBtn->setIcon( icon) ;
}

//----------------------------------------------------
void CollectionDlg::RetranslateDialog()
{
    setWindowTitle( tr( "Collection")) ;
    ui->loadBtn->setToolTip( tr( "Load from library")) ;
    ui->delBtn->setToolTip( tr( "Delete from library")) ;
}

//----------------------------------------------------
void CollectionDlg::UpdateDbView()
{
    if ( m_pCollMgr->HasLastQueryMod())
        PopulateRecordsList() ;
}

//----------------------------------------------------
bool CollectionDlg::Initialize( CollectionMgr* pMgr, QDialog* pParent)
{
    m_pCollMgr = pMgr ;
    m_pParent  = ( CPhoto*) pParent ;

    return m_pCollMgr != NULL  &&  m_pParent != NULL ;
}

//----------------------------------------------------
void CollectionDlg::on_recordsList_itemDoubleClicked( QListWidgetItem *item)
{
    ui->filesList->clear() ;
    ui->filesList->addItems( m_pCollMgr->GetItemData( item->text())) ;
    if ( ui->filesList->count() > 0) {
        QListWidgetItem* first = ui->filesList->item( 0) ;
        ui->filesList->setItemSelected( first, true) ;
        on_filesList_itemDoubleClicked( first) ;
    }
}

//----------------------------------------------------
void CollectionDlg::DoShowHide( bool bShow)
{
    if ( bShow) {
        setVisible( true) ;
        show() ;
        raise() ;
        activateWindow() ;

        PopulateRecordsList() ;
    }
    else {
        hide() ;
        setVisible( false) ;
    }
}

//----------------------------------------------------
void CollectionDlg::PopulateRecordsList()
{
    ui->recordsList->clear() ;
    ui->recordsList->addItems( m_pCollMgr->GetItemsList()) ;
    if ( ui->recordsList->count() > 0) {
        QListWidgetItem* first = ui->recordsList->item( 0) ;
        ui->recordsList->setItemSelected( first, true) ;
        on_recordsList_itemDoubleClicked( first) ;
    }
}

//----------------------------------------------------
void CollectionDlg::on_filesList_itemDoubleClicked( QListWidgetItem *item)
{
    QImage imgTmp ;

    ! imgTmp.load( item->text()) ?
    ui->preView->setText(   tr( "No preview available")) :
    ui->preView->setPixmap( QPixmap::fromImage( imgTmp.scaled( imgTmp.width()  * 0.25,
                                                               imgTmp.height() * 0.25,
                                                               Qt::KeepAspectRatio,
                                                               Qt::FastTransformation))) ;
}

//----------------------------------------------------
void CollectionDlg::moveEvent( QMoveEvent* event)
{
    if ( ! m_bAllowMove)
        move( m_pos) ;
}

//----------------------------------------------------
void CollectionDlg::UpdatePosHeight( const QPoint& pos, int height)
{
    int    i ;
    int    diff ;
    QPoint btnPos ;
    QSize  size ;

    diff         = this->height() - height ;
    m_bAllowMove = true ;
    move( pos) ;
    m_bAllowMove = false ;
    m_pos        = pos ;

    setFixedHeight( height) ;
    for ( i = 1 ;  i <= BUTTONS ;  i ++) {
        btnPos = ui->buttons->button(i)->pos() ;
        ui->buttons->button(i)->move( btnPos.x(), btnPos.y() - diff) ;
    }

    size = ui->preView->size() ;
    ui->preView->resize( size.width(), size.height() - diff) ;
    ui->frame->resize( size.width(), size.height() - diff) ;
}

//----------------------------------------------------
void CollectionDlg::on_loadBtn_clicked()
{
    m_pParent->ShowList( GetCurrTextItem()) ;
}

//----------------------------------------------------
void CollectionDlg::on_delBtn_clicked()
{
    int nCurr = ui->recordsList->currentRow() ;
    int nNext = nCurr == ui->recordsList->count() - 1 ? 0 : nCurr ;

    m_pCollMgr->DeleteItem( GetCurrTextItem()) ;
    ui->recordsList->takeItem( nCurr) ;

    if ( ui->recordsList->count() > 0) {
        QListWidgetItem* pNext = ui->recordsList->item( nNext) ;
        ui->recordsList->setItemSelected( pNext, true) ;
        on_recordsList_itemDoubleClicked( pNext) ;
    }
}

//----------------------------------------------------
QString CollectionDlg::GetCurrTextItem()
{
    return ui->recordsList->currentItem()->text() ;
}
