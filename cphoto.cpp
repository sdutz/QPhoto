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

#include "cphoto.h"
#include "ui_cphoto.h"
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include "aboutdlg.h"


//----------------------------------------------------
#define MIN_WIDTH          814
#define MIN_HEIGHT         764
#define LEFT_BUTTON        3
#define RIGHT_BUTTON       5
#define SCENE_OFFS         20


//----------------------------------------------------
CPhoto::CPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPhoto)
{
    ui->setupUi(this);
    m_szFileName    = "" ;
    m_nCurr         = -1 ;
    m_bOrdChanged   = false ;
    m_bFullScreen   = false ;
   // TODO recuperare la cartella dei documenti!
    m_bShiftPressed = false ;
    m_bCtrlPressed  = false ;
    setMinimumSize( MIN_WIDTH, MIN_HEIGHT);
    m_pConf = new ConfMgr() ;
    SetIds();
    CreateActions();
    BuildContextMenu();
    BuildSlideShowMenu();
    ShowList();
    SetToolTips() ;
    ui->ImgView->SetConfMgr( m_pConf);
}

//----------------------------------------------------
CPhoto::~CPhoto()
{
    DeleteAction() ;

    if ( m_pConf != NULL)
        delete m_pConf ;

    if ( m_pTimer != NULL)
        delete m_pTimer ;

    delete ui;
}

//----------------------------------------------------
void CPhoto::DeleteAction()
{
    if ( m_pMoveUpAct != NULL)
        delete m_pMoveUpAct ;
    if ( m_pMoveDownAct != NULL)
        delete m_pMoveDownAct ;
    if ( m_pZoomAllAct != NULL)
        delete m_pZoomAllAct ;
    if ( m_pConfigAct != NULL)
        delete m_pConfigAct ;
    if ( m_pStartSlideShowAct != NULL)
        delete m_pStartSlideShowAct ;
    if ( m_pEndSlideShowAct != NULL)
        delete m_pEndSlideShowAct ;
    if ( m_pPauseSlideShowAct != NULL)
        delete m_pPauseSlideShowAct ;
    if ( m_pShowFullScreen != NULL)
        delete m_pShowFullScreen ;
    if ( m_pExitFullScreen != NULL)
        delete m_pExitFullScreen ;
}

//----------------------------------------------------
void CPhoto::SetIds()
{
    ui->RightButtons->setId( ui->BtnPlus, 1) ;
    ui->RightButtons->setId( ui->BtnMinus, 2);
    ui->RightButtons->setId( ui->BtnLeft, 3);
    ui->RightButtons->setId( ui->BtnRight, 4);
    ui->RightButtons->setId( ui->BtnExit, 5);

    ui->LeftButtons->setId( ui->BtnSave, 1);
    ui->LeftButtons->setId( ui->BtnDel, 2);
    ui->LeftButtons->setId( ui->BtnOpen,  3);
}

//----------------------------------------------------
void CPhoto::SetToolTips()
{
   // Todo Completare i tooltip con tutti gli elementi del dialogo
    ui->BtnExit->setToolTip( "Exit");
    ui->BtnOpen->setToolTip( "Open");
}

//----------------------------------------------------
void CPhoto::CreateActions()
{
    m_pMoveUpAct = new QAction( "MoveUp", this) ;
    connect( m_pMoveUpAct, SIGNAL( triggered()), this, SLOT( OnMoveCurrUp())) ;

    m_pMoveDownAct = new QAction( "MoveDown", this) ;
    connect( m_pMoveDownAct, SIGNAL( triggered()), this, SLOT( OnMoveCurrDown())) ;

    m_pZoomAllAct = new QAction( "ZoomAll", this) ;
    connect( m_pZoomAllAct, SIGNAL( triggered()), this, SLOT( OnZoomAll())) ;

    m_pConfigAct = new QAction( "Config", this) ;
    connect( m_pConfigAct, SIGNAL( triggered()), this, SLOT( OnConfig())) ;

    m_pStartSlideShowAct = new QAction( "Start SlideShow", this) ;
    connect( m_pStartSlideShowAct, SIGNAL( triggered()), this, SLOT( OnStartSlideShow())) ;

    m_pEndSlideShowAct = new QAction( "End SlideShow", this) ;
    connect( m_pEndSlideShowAct, SIGNAL( triggered()), this, SLOT( OnEndSlideShow())) ;

    m_pPauseSlideShowAct = new QAction( "Pause SlideShow", this) ;
    connect( m_pPauseSlideShowAct, SIGNAL( triggered()), this, SLOT( OnPauseSlideShow())) ;

    m_pShowFullScreen = new QAction( "Show Fullscreen", this) ;
    connect( m_pShowFullScreen, SIGNAL( triggered()), this, SLOT( SwitchFullScreen())) ;

    m_pExitFullScreen = new QAction( "Exit Fullscreen", this) ;
    connect( m_pExitFullScreen, SIGNAL( triggered()), this, SLOT( SwitchFullScreen())) ;

    m_pTimer = new QTimer( this) ;
    connect( m_pTimer, SIGNAL( timeout()), this, SLOT( on_BtnRight_clicked())) ;
}

//----------------------------------------------------
void CPhoto::BuildContextMenu()
{
    m_ContextMenu.addAction( m_pMoveUpAct) ;
    m_ContextMenu.addAction( m_pMoveDownAct) ;
    m_ContextMenu.addAction( m_pZoomAllAct) ;
    m_ContextMenu.addAction( m_pConfigAct) ;
    m_ContextMenu.addAction( m_pShowFullScreen) ;
}

//----------------------------------------------------
void CPhoto::BuildSlideShowMenu()
{
    m_SlideShowMenu.addAction( m_pStartSlideShowAct) ;
    m_SlideShowMenu.addAction( m_pPauseSlideShowAct) ;
    m_SlideShowMenu.addAction( m_pEndSlideShowAct) ;
    m_SlideShowMenu.addAction( m_pZoomAllAct) ;
    m_SlideShowMenu.addAction( m_pExitFullScreen) ;

    m_pPauseSlideShowAct->setEnabled( false);
    m_pEndSlideShowAct->setEnabled( false);
}

//----------------------------------------------------
void CPhoto::on_BtnOpen_clicked()
{
    if ( m_bShiftPressed)
        LoadFile() ;
    else
        LoadImage() ;
}

//----------------------------------------------------
void CPhoto::LoadFile()
{
    QString szFile ;
    QString szFilters ;

    szFilters       = "List (*.txt)" ;
    szFile          = QFileDialog::getOpenFileName( this, "Open File",
                      m_pConf->GetLastDir(), szFilters) ;

    if ( szFile.isEmpty())
        return ;

    ui->ImgView->SetShiftPressed( false);
    m_bShiftPressed = false ;
    DeleteAll();
    ShowList( szFile);
}

//----------------------------------------------------
void CPhoto::LoadImage()
{
    QString szFilters ;


    szFilters    = "Images (*.jpeg *.jpg)" ;
    m_szFileName = QFileDialog::getOpenFileName( this, "Open File",
                   m_pConf->GetLastDir(), szFilters) ;

    if( m_szFileName.isEmpty())
       return ;

    ShowPhoto( true) ;
    m_nCurr = ui->ImgList->count() - 1 ;
    ui->ImgList->setCurrentRow( m_nCurr);
}

//----------------------------------------------------
bool CPhoto::ShowPhoto( bool bToAddToList)
{
    if ( m_szFileName.isEmpty())
        return false ;

    if ( ui->ImgView->ShowPhoto( m_szFileName)) {
        setWindowTitle( m_szFileName);
        if( bToAddToList  &&  ! m_pConf->FindInList( m_szFileName)) {
            m_pConf->AddToList( m_szFileName);
            ui->ImgList->addItem( m_szFileName) ;
        }
    }

    return true ;
}

//----------------------------------------------------
void CPhoto::on_BtnExit_clicked()
{
    if ( m_bOrdChanged)
        RefreshList() ;
    m_pConf->WriteList();
    QDialog::close() ;
}

//----------------------------------------------------
void CPhoto::on_ImgList_itemDoubleClicked( QListWidgetItem* item)
{
    QString szTmp ;

    szTmp = item->text() ;

    if( ! szTmp.isEmpty()) {
        m_szFileName = szTmp ;
        ShowPhoto( false) ;
    }

    m_nCurr = ui->ImgList->row( item) ;
}

//----------------------------------------------------
void CPhoto::on_BtnMinus_clicked()
{
    ui->ImgView->ZoomOut();
}

//----------------------------------------------------
void CPhoto::on_BtnPlus_clicked()
{
    ui->ImgView->ZoomIn();
}

//----------------------------------------------------
void CPhoto::DoDebug( const QString& szDebug)
{
    QMessageBox box ;
    box.setText( szDebug);
    box.exec() ;
}

//----------------------------------------------------
void CPhoto::ShowList(  const QString& szFile)
{
    int     n ;
    QString szIt ;

    m_pConf->LoadList( szFile);

    for ( n = 0 ; n < m_pConf->GetItemCount() ; n ++) {
        szIt = m_pConf->GetListItem( n) ;
        ui->ImgList->addItem( szIt);
    }

    m_nCurr = -1 ;
    on_BtnRight_clicked();
}

//----------------------------------------------------
void CPhoto::SeePrevImg()
{
    if ( m_nCurr == 0)
        return ;

    m_nCurr -- ;
    m_szFileName = ui->ImgList->item(m_nCurr)->text() ;
    ui->ImgList->setCurrentRow( m_nCurr);
    ShowPhoto( false) ;
}

//----------------------------------------------------
void CPhoto::SeeNextImg()
{
    if ( m_nCurr == ui->ImgList->count() - 1)
        return ;

    m_nCurr ++ ;
    m_szFileName = ui->ImgList->item(m_nCurr)->text() ;
    ui->ImgList->setCurrentRow( m_nCurr);

    ShowPhoto( false) ;

    if ( m_pTimer->isActive()) {
        m_pTimer->stop();
        m_pTimer->start();
    }
}

//----------------------------------------------------
void CPhoto::on_BtnLeft_clicked()
{
    if ( m_bShiftPressed)
        OnMoveCurrUp() ;
    else
        SeePrevImg() ;
}

//----------------------------------------------------
void CPhoto::on_BtnRight_clicked()
{
    if ( m_bShiftPressed)
        OnMoveCurrDown();
    else
        SeeNextImg();
}

//----------------------------------------------------
void CPhoto::DeleteAll()
{
    m_pConf->ClearList() ;
    ui->ImgList->clear();
    m_pConf->WriteList();
    ui->ImgView->ResetView();
}

//----------------------------------------------------
void CPhoto::DeleteSingle()
{
    QListWidgetItem* pItem ;

    pItem = ui->ImgList->takeItem( m_nCurr);
    m_pConf->RemoveFromList( pItem->text());

    if ( m_nCurr == ui->ImgList->count())
        m_nCurr -- ;
    ui->ImgList->setCurrentRow( m_nCurr);
    m_szFileName = ui->ImgList->item( m_nCurr)->text() ;
    if( ! m_szFileName.isEmpty())
        ShowPhoto( false) ;
}

//----------------------------------------------------
void CPhoto::keyPressEvent ( QKeyEvent* e)
{
    switch ( e->key()) {

        case Qt::Key_Space :
        SeeNextImg();
        break ;

        case Qt::Key_Backspace:
        SeePrevImg();
        break ;

        case Qt::Key_Plus:
        on_BtnPlus_clicked(); ;
        break ;

        case Qt::Key_Minus:
        on_BtnMinus_clicked(); ;
        break ;
        on_BtnDel_clicked() ;
        case Qt::Key_Delete:

        break ;

        case Qt::Key_Shift:
        m_bShiftPressed = true ;
        ui->ImgView->SetShiftPressed( true);
        break ;

        case Qt::Key_F1 :
        ShowAboutDlg() ;
        break ;

        case Qt::Key_F :
        SwitchFullScreen() ;
        break ;

        case Qt::Key_Escape :
        if ( m_bFullScreen)
            SwitchFullScreen();
        break ;

        case Qt::Key_Control :
        m_bCtrlPressed = true ;
        break ;

        case Qt::Key_Q :
        if ( m_bCtrlPressed)
            on_BtnExit_clicked();
        break ;
    }
}

//----------------------------------------------------
void CPhoto::keyReleaseEvent( QKeyEvent* e)
{
    if ( e->key() == Qt::Key_Shift) {
        m_bShiftPressed = false ;
        ui->ImgView->SetShiftPressed( false);
    }
    else if ( e->key() == Qt::Key_Control)
        m_bCtrlPressed = false ;
}

//----------------------------------------------------
void CPhoto::ShowAboutDlg()
{
    AboutDlg dlg ;

    dlg.exec() ;
}

//----------------------------------------------------
void CPhoto::on_BtnDel_clicked()
{
    if ( ! m_bShiftPressed)
        DeleteSingle();
    else
        DeleteAll();
}

//----------------------------------------------------
void CPhoto::on_BtnSave_clicked()
{
    QString szFile ;
    QString szFilters ;

    szFilters = "List (*.txt)" ;
    szFile    = QFileDialog::getSaveFileName( this, "Save File",
                             m_pConf->GetLastDir(), szFilters) ;

    m_pConf->WriteList( szFile);
}

//----------------------------------------------------
void CPhoto::UpdateLayoutAfterResize( int nXMove, int nYMove)
{
    int     nId ;
    QPoint  cPt ;
    QSize   cSize ;

    for ( nId = 1 ;  nId < 6 ;  nId++) {
        cPt = ui->RightButtons->button( nId)->pos() ;
        cPt.setX( cPt.x() + nXMove);
        cPt.setY( cPt.y() + nYMove);

        ui->RightButtons->button( nId)->move( cPt);
    }

    for ( nId = 1 ;  nId < 4 ;  nId++) {
        cPt = ui->LeftButtons->button( nId)->pos() ;
        cPt.setY( cPt.y() + nYMove);
        ui->LeftButtons->button( nId)->move( cPt);
    }

    cPt = ui->ImgList->pos() ;
    cPt.setY( cPt.y() + nYMove);
    ui->ImgList->move( cPt);
    cSize  = ui->ImgList->size() ;
    cSize.setWidth( cSize .width() + nXMove);
    ui->ImgList->resize( cSize );

    cSize  = ui->ImgView->size() ;
    cSize.setWidth( cSize .width() + nXMove);
    cSize.setHeight( cSize .height() + nYMove);
    ui->ImgView->resize( cSize );
}

//----------------------------------------------------
void CPhoto::resizeEvent( QResizeEvent* event)
{
    int     nXMove ;
    int     nYMove ;
    QSize   cOldSize ;
    QSize   cNewSize ;


    cOldSize = event->oldSize() ;
    cNewSize = event->size() ;

    if ( cOldSize.width() == -1  ||
         cOldSize.height() == -1)
        return ;

    nXMove = cNewSize.width() - cOldSize.width() ;
    nYMove = cNewSize.height() - cOldSize.height() ;

    UpdateLayoutAfterResize( nXMove, nYMove);
}

//----------------------------------------------------
void CPhoto::ShowContextMenu( const QPoint& pos)
{
    m_ContextMenu.exec( pos) ;
}

//----------------------------------------------------
void CPhoto::ShowSlideShowMenu( const QPoint& pos)
{
    m_SlideShowMenu.exec( pos) ;
}

//----------------------------------------------------
bool CPhoto::IsPosOnView( const QPoint& pos)
{
    QRect  cViewRect ;
    QRect  cPosRect ;

    cPosRect.setBottomLeft( pos);
    cPosRect.setTopRight( pos);
    cViewRect = ui->ImgView->rect() ;

    return cViewRect.intersects( cPosRect) ;
}

//----------------------------------------------------
void CPhoto::mousePressEvent( QMouseEvent* e)
{
    QPoint pos ;

    pos = e->pos() ;

    if ( ! IsPosOnView( pos))
        return ;

    if ( e->type() == QEvent::MouseButtonPress) {
        if ( e->button() == Qt::RightButton) {
            QPoint ptGlob ;
            ptGlob.setX( e->globalX());
            ptGlob.setY( e->globalY());
            if ( ! m_bFullScreen)
                ShowContextMenu( ptGlob);
            else
                ShowSlideShowMenu( ptGlob) ;
        }
        else if ( e->button() == Qt::LeftButton)
            ui->ImgView->StartZoomRect( pos);
    }
}

//----------------------------------------------------
void CPhoto::OnMoveCurrUp()
{
    int              nCurrRow ;
    QListWidgetItem* pItemCurr ;

    nCurrRow  = ui->ImgList->currentRow() ;
    if ( nCurrRow < 1)
        return ;

    pItemCurr = ui->ImgList->takeItem( nCurrRow) ;
    nCurrRow -- ;
    ui->ImgList->insertItem( nCurrRow, pItemCurr) ;

    m_nCurr = nCurrRow ;
    ui->ImgList->setCurrentRow( m_nCurr);
    m_bOrdChanged = true ;
}

//----------------------------------------------------
void CPhoto::OnMoveCurrDown()
{
    int              nCurrRow ;
    QListWidgetItem* pItemCurr ;

    nCurrRow  = ui->ImgList->currentRow() ;
    if ( nCurrRow > ui->ImgList->count() - 1)
        return ;

    pItemCurr = ui->ImgList->takeItem( nCurrRow) ;
    nCurrRow ++ ;
    ui->ImgList->insertItem( nCurrRow, pItemCurr) ;

    m_nCurr = nCurrRow ;
    ui->ImgList->setCurrentRow( m_nCurr);
    m_bOrdChanged = true ;
}

//----------------------------------------------------
void CPhoto::OnZoomAll()
{
    ui->ImgView->ZoomAll();
}

//----------------------------------------------------
void CPhoto::RefreshList()
{
    int n ;

    m_pConf->ClearList();

    for ( n = 0 ;  n < ui->ImgList->count() ; n ++ )
        m_pConf->AddToList( ui->ImgList->item( n)->text());
}

//----------------------------------------------------
void CPhoto::OnConfig()
{
    m_pConf->ShowSettingsDlg() ;
}

//----------------------------------------------------
void CPhoto::SwitchFullScreen()
{
    int   nIdx ;
    QSize DlgSize ;
    QSize SceneSize ;

    if ( ! m_bFullScreen) {


        for ( nIdx = 1 ;  nIdx <= RIGHT_BUTTON ;  nIdx ++)
            ui->RightButtons->button( nIdx)->hide();

        for ( nIdx = 1 ;  nIdx <= LEFT_BUTTON ;  nIdx ++)
            ui->LeftButtons->button( nIdx)->hide();

        ui->ImgList->hide();

        DlgSize   = size() ;
        SceneSize = ui->ImgView->size() ;
        m_DiffSize.setWidth( DlgSize.width() - SceneSize.width());
        m_DiffSize.setHeight( DlgSize.height() - SceneSize.height());

        showFullScreen();

        ui->ImgView->move( 0, 0);
        ui->ImgView->resize( size()) ;
    }
    else {

        for ( nIdx = 1 ;  nIdx <= RIGHT_BUTTON ;  nIdx ++)
            ui->RightButtons->button( nIdx)->show();

        for ( nIdx = 1 ;  nIdx <= LEFT_BUTTON ;  nIdx ++)
            ui->LeftButtons->button( nIdx)->show();


        showNormal();

        ui->ImgList->show();

        DlgSize = size() ;
        SceneSize.setWidth( DlgSize.width() - m_DiffSize.width());
        SceneSize.setHeight( DlgSize.height() - m_DiffSize.height());
        ui->ImgView->move( SCENE_OFFS, SCENE_OFFS) ;
        ui->ImgView->resize( SceneSize);

        if ( m_pTimer->isActive())
            m_pTimer->stop();
    }

    m_bFullScreen = ! m_bFullScreen ;
}

//----------------------------------------------------
void CPhoto::OnStartSlideShow()
{
    m_pStartSlideShowAct->setEnabled( false);
    m_pEndSlideShowAct->setEnabled( true);
    m_pPauseSlideShowAct->setEnabled( true);

    m_nCurr = -1 ;
    on_BtnRight_clicked();
    m_pTimer->setInterval( m_pConf->GetSeconds() * 1000);
    m_pTimer->start();
}

//----------------------------------------------------
void CPhoto::OnEndSlideShow()
{
    m_pPauseSlideShowAct->setEnabled( false);
    m_pEndSlideShowAct->setEnabled( false);
    m_pStartSlideShowAct->setEnabled( true);
    m_pTimer->stop();
}

//----------------------------------------------------
void CPhoto::OnPauseSlideShow()
{
    if ( m_pTimer->isActive())
        m_pTimer->stop();
    else
        m_pTimer->start();
}