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
#include <QUrl>
#include "aboutdlg.h"
#include "util.h"


//----------------------------------------------------
#define MIN_WIDTH          814
#define MIN_HEIGHT         764
#define LEFT_BUTTON        5
#define RIGHT_BUTTON       5
#define SCENE_OFFS         20
#define QPHOTO             "QPhoto"
#define MSEC_INI_LOAD      200


//----------------------------------------------------
CPhoto::CPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPhoto)
{
    ui->setupUi(this);
    setModal( false);
    setWindowTitle( QPHOTO);
    m_szFileName    = "" ;
    m_nCurr         = -1 ;
    m_bOrdChanged   = false ;
    m_bFullScreen   = false ;
    m_bShowHelp     = false ;
    m_bShiftPressed = false ;
    m_bCtrlPressed  = false ;
    m_player        = NULL ;
    setMinimumSize( MIN_WIDTH, MIN_HEIGHT);
    m_pConf = new ConfMgr() ;
    m_pColl = new CollectionMgr() ;
    m_cCollDlg.SetMgr( m_pColl) ;
    m_pConf->SetDbMgr( m_pColl) ;
    InitLang() ;
    SetIds();
    CreateActions();
    CreateTimers() ;
    BuildContextMenu();
    BuildSlideShowMenu();
    ui->ImgView->SetConfMgr( m_pConf);
    ui->ImgView->PrepareSlideshowItems() ;
    SetBtnIcons() ;
    setAcceptDrops( true);
    InitPlayer();
    RetranslateDialog();
}

//----------------------------------------------------
CPhoto::~CPhoto()
{
    DeleteAction() ;
    DeleteTimers() ;

    if ( m_pConf != NULL)
        delete m_pConf ;

    if ( m_pColl != NULL)
        delete m_pColl ;

    if ( m_player != NULL)
        delete m_player ;

    delete ui;
}

//----------------------------------------------------
void CPhoto::InitLang()
{
    int     nLang ;
    QString szLangFile ;

    m_pConf->GetIntProp( PROP_INT_LANG, &nLang) ;

    szLangFile = "QPhoto_" ;
    szLangFile += GetLang( nLang) ;

    m_cTranslator.load( szLangFile) ;

    QCoreApplication::installTranslator( &m_cTranslator) ;
}

//----------------------------------------------------
QString CPhoto::GetLang( int nLang)
{
    if ( nLang == ENGLISH)
        return "en" ;
    else
        return "it" ;
}


//----------------------------------------------------
void CPhoto::ChangeLang( int nLang)
{
    QString szLangFile ;

    QCoreApplication::removeTranslator( &m_cTranslator) ;

    szLangFile = "QPhoto_" ;
    szLangFile += GetLang( nLang) ;

    m_cTranslator.load( szLangFile) ;

    QCoreApplication::installTranslator( &m_cTranslator) ;

    RetranslateDialog();
}

//----------------------------------------------------
void CPhoto::InitPlayer()
{
    m_player  = Phonon::createPlayer( Phonon::MusicCategory) ;
}

//----------------------------------------------------
void CPhoto::DeleteTimers()
{

    if ( m_pTimer != NULL)
        delete m_pTimer ;

    if ( m_pLoadIniTimer != NULL)
        delete m_pLoadIniTimer ;

    if ( m_pFullScreenTimer != NULL)
        delete m_pFullScreenTimer ;
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
void CPhoto::SetBtnIcons()
{
    QIcon   icon ;
    QSize   pixSize ;


    GetPixBtnSize( ui->BtnLeft->size(), &pixSize) ;
    icon.addFile( "icons/arrow_left.png", pixSize) ;
    ui->BtnLeft->setIcon( icon);

    GetPixBtnSize( ui->BtnRight->size(), &pixSize) ;
    icon.addFile( "icons/arrow_right.png", pixSize) ;
    ui->BtnRight->setIcon( icon);

    GetPixBtnSize( ui->BtnMinus->size(), &pixSize) ;
    icon.addFile( "icons/zoom_out.png", pixSize) ;
    ui->BtnMinus->setIcon( icon);

    GetPixBtnSize( ui->BtnPlus->size(), &pixSize) ;
    icon.addFile( "icons/zoom_in.png", pixSize) ;
    ui->BtnPlus->setIcon( icon);

    GetPixBtnSize( ui->BtnDel->size(), &pixSize) ;
    icon.addFile( "icons/picture_delete.png", pixSize) ;
    ui->BtnDel->setIcon( icon);

    GetPixBtnSize( ui->BtnSave->size(), &pixSize) ;
    icon.addFile( "icons/picture_save.png", pixSize) ;
    ui->BtnSave->setIcon( icon);

    GetPixBtnSize( ui->BtnConfig->size(), &pixSize) ;
    icon.addFile( "icons/wrench.png", pixSize) ;
    ui->BtnConfig->setIcon( icon);

    GetPixBtnSize( ui->BtnLibrary->size(), &pixSize) ;
    icon.addFile( "icons/database.png", pixSize) ;
    ui->BtnLibrary->setIcon( icon);

    GetPixBtnSize( ui->BtnOpen->size(), &pixSize) ;
    icon.addFile( "icons/picture_add.png", pixSize) ;
    ui->BtnOpen->setIcon( icon);

    GetPixBtnSize( ui->BtnExit->size(), &pixSize) ;
    icon.addFile( "icons/cross.png", pixSize) ;
    ui->BtnExit->setIcon( icon);
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
    ui->LeftButtons->setId( ui->BtnLibrary, 4);
    ui->LeftButtons->setId( ui->BtnConfig, 5);
}

//----------------------------------------------------
void CPhoto::InitLogDlg()
{
    m_cErrDlg.SetMgr( m_pConf) ;
    m_cErrDlg.setModal( false);
}

//----------------------------------------------------
void CPhoto::CreateTimers()
{

    m_pTimer = new QTimer( this) ;
    connect( m_pTimer, SIGNAL( timeout()), this, SLOT( on_BtnRight_clicked())) ;

    m_pLoadIniTimer = new QTimer( this) ;
    connect( m_pLoadIniTimer, SIGNAL( timeout()), this, SLOT( OnIniLoad())) ;

    m_pFullScreenTimer = new QTimer( this) ;
    connect( m_pFullScreenTimer, SIGNAL( timeout()), this, SLOT( OnFullSw())) ;

    m_pLoadIniTimer->setInterval( MSEC_INI_LOAD) ;
    m_pLoadIniTimer->start() ;
}

//----------------------------------------------------
void CPhoto::CreateActions()
{
    QIcon icon ;

    m_pMoveUpAct = new QAction( "", this) ;
    icon.addFile( "icons/arrow_up.png") ;
    m_pMoveUpAct->setIcon( icon);
    connect( m_pMoveUpAct, SIGNAL( triggered()), this, SLOT( OnMoveCurrUp())) ;

    m_pMoveDownAct = new QAction( "", this) ;
    icon.addFile( "icons/arrow_down.png") ;
    m_pMoveDownAct->setIcon( icon);
    connect( m_pMoveDownAct, SIGNAL( triggered()), this, SLOT( OnMoveCurrDown())) ;

    m_pZoomAllAct = new QAction( "", this) ;
    icon.addFile( "icons/zoom.png") ;
    m_pZoomAllAct->setIcon( icon);
    connect( m_pZoomAllAct, SIGNAL( triggered()), this, SLOT( OnZoomAll())) ;

    m_pConfigAct = new QAction( "", this) ;
    icon.addFile( "icons/wrench.png") ;
    m_pConfigAct->setIcon( icon);
    connect( m_pConfigAct, SIGNAL( triggered()), this, SLOT( OnConfig())) ;

    m_pStartSlideShowAct = new QAction( "", this) ;
    icon.addFile( "icons/control_play_blue.png") ;
    m_pStartSlideShowAct->setIcon( icon);
    connect( m_pStartSlideShowAct, SIGNAL( triggered()), this, SLOT( OnStartSlideShow())) ;

    m_pEndSlideShowAct = new QAction( "", this) ;
    icon.addFile( "icons/control_stop_blue.png") ;
    m_pEndSlideShowAct->setIcon( icon);
    connect( m_pEndSlideShowAct, SIGNAL( triggered()), this, SLOT( OnEndSlideShow())) ;

    m_pPauseSlideShowAct = new QAction( "", this) ;
    icon.addFile( "icons/control_pause_blue.png") ;
    m_pPauseSlideShowAct->setIcon( icon);
    connect( m_pPauseSlideShowAct, SIGNAL( triggered()), this, SLOT( OnPauseSlideShow())) ;

    m_pShowFullScreen = new QAction( "", this) ;
    icon.addFile( "icons/monitor.png") ;
    m_pShowFullScreen->setIcon( icon);
    connect( m_pShowFullScreen, SIGNAL( triggered()), this, SLOT( SwitchFullScreen())) ;

    m_pExitFullScreen = new QAction( "", this) ;
    icon.addFile( "icons/monitor_go.png") ;
    m_pExitFullScreen->setIcon( icon);
    connect( m_pExitFullScreen, SIGNAL( triggered()), this, SLOT( SwitchFullScreen())) ;
}

//----------------------------------------------------
void CPhoto::RetranslateDialog()
{
    m_pMoveUpAct->setText( tr( "Move Up"));
    m_pMoveDownAct->setText( tr( "Move Down" ));
    m_pStartSlideShowAct->setText( tr( "Start SlideShow")) ;
    m_pZoomAllAct->setText( tr( "Zoom All"));
    m_pShowFullScreen->setText( tr( "Show Fullscreen"));
    m_pExitFullScreen->setText( tr(" Exit Fullscreen"));
    m_pEndSlideShowAct->setText( tr( "End SlideShow"));
    m_pPauseSlideShowAct->setText( tr( "Pause SlideShow"));
    m_pConfigAct->setText( tr( "Settings"));


    ui->BtnConfig->setToolTip( tr( "Settings"));
    ui->BtnDel->setToolTip( tr( "Delete"));
    ui->BtnExit->setToolTip( tr( "Exit"));
    ui->BtnOpen->setToolTip( tr( "Open"));
    ui->BtnSave->setToolTip( tr( "Save"));
    ui->BtnMinus->setToolTip( tr( "Zoom Out"));
    ui->BtnPlus->setToolTip( tr( "Zoom in"));
    ui->BtnLeft->setToolTip( tr( "See Previous Image"));
    ui->BtnRight->setToolTip( tr( "See Next Image"));

}

//----------------------------------------------------
void CPhoto::on_ImgDropped( const QString& szFile, bool bShow)
{
    m_szFileName = szFile ;
    LoadImage( bShow) ;
}

//----------------------------------------------------
void CPhoto::OnIniLoad()
{
    ShowList();
    m_pLoadIniTimer->stop();
}

//----------------------------------------------------
void CPhoto::OnFullSw()
{
    ui->ImgView->ZoomAll();
    m_pFullScreenTimer->stop();
}

//----------------------------------------------------
void CPhoto::BuildContextMenu()
{
    m_ContextMenu.addAction( m_pMoveUpAct) ;
    m_ContextMenu.addAction( m_pMoveDownAct) ;
    m_ContextMenu.addAction( m_pZoomAllAct) ;
    m_ContextMenu.addAction( m_pConfigAct) ;
    m_ContextMenu.addAction( m_pShowFullScreen) ;
    m_ContextMenu.addAction( m_pStartSlideShowAct) ;
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
        LoadImages();
}

//----------------------------------------------------
void CPhoto::LoadFile()
{
    QString szLastDir ;
    QString szFile ;
    QString szFilters ;

    szFilters = "List (*.txt)" ;
    m_pConf->GetStrProp( PROP_STR_LAST_DIR_LIST, &szLastDir) ;
    szFile    = QFileDialog::getOpenFileName( this, tr( "Open List"), szLastDir, szFilters) ;

    if ( szFile.isEmpty())
        return ;

    ui->ImgView->SetShiftPressed( false);
    m_bShiftPressed = false ;
    DeleteAll();
    ShowList( szFile);
}

//----------------------------------------------------
void CPhoto::LoadImages()
{
    int         n ;
    QString     szLastDir ;
    QString     szFilters ;
    QStringList lszList ;


    szFilters = "Images (*.jpeg *.jpg)" ;
    m_pConf->GetStrProp( PROP_STR_LAST_DIR, &szLastDir) ;
    lszList   = QFileDialog::getOpenFileNames( this, tr( "Open Images"), szLastDir, szFilters) ;

    for( n = 0 ;  n < lszList.count() ;  n ++) {
        m_szFileName = lszList.at(n) ;
        LoadImage( n == lszList.count() - 1);
    }
}

//----------------------------------------------------
void CPhoto::LoadImage( bool bShow)
{
    if( m_szFileName.isEmpty())
       return ;

    ShowPhoto( true, bShow) ;
    m_nCurr = ui->ImgList->count() - 1 ;
    ui->ImgList->setCurrentRow( m_nCurr) ;
}

//----------------------------------------------------
bool CPhoto::ShowPhoto( bool bToAddToList, bool bShow)
{
    if ( m_szFileName.isEmpty())
        return false ;

    if ( ! bShow  ||  ui->ImgView->ShowPhoto( m_szFileName)) {
        QString title ;
        title = QPHOTO ;
        title += ":  " + m_szFileName ;
        setWindowTitle( title);
        if( bToAddToList  &&  ! m_pConf->FindInList( m_szFileName)) {
            m_pConf->AddToList( m_szFileName);
            ui->ImgList->addItem( m_szFileName) ;
        }
    }
    else {
        m_szLog = "Cannot open" + m_szFileName ;
        OnShowLog();
    }

    ui->ImgView->ZoomAll();

    return true ;
}

//----------------------------------------------------
void CPhoto::on_BtnExit_clicked()
{
    m_cErrDlg.close() ;

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
    SeeNextImg();
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
    m_bShiftPressed ? OnMoveCurrUp() : SeePrevImg() ;
}

//----------------------------------------------------
void CPhoto::on_BtnRight_clicked()
{
    m_bShiftPressed ? OnMoveCurrDown() : SeeNextImg() ;
}

//----------------------------------------------------
void CPhoto::DeleteAll()
{
    m_pConf->ClearList() ;
    ui->ImgList->clear();
    m_pConf->WriteList();
    ui->ImgView->ResetView( true);
    setWindowTitle( QPHOTO);
}

//----------------------------------------------------
void CPhoto::DeleteSingle()
{
    QListWidgetItem* pItem ;

    if ( m_nCurr < 0)
        return ;

    pItem = ui->ImgList->takeItem( m_nCurr);
    m_pConf->RemoveFromList( pItem->text());

    if ( m_nCurr == ui->ImgList->count())
        m_nCurr -- ;
    if ( m_nCurr < 0) {
        ui->ImgView->ResetView( true);
        return ;
    }
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

        case Qt::Key_Delete:
        on_BtnDel_clicked() ;
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
        if ( m_bShowHelp)
            OnHelp();
        else if ( m_bFullScreen)
            SwitchFullScreen();
        break ;   

        case Qt::Key_Control :
        m_bCtrlPressed = true ;
        break ;

        case Qt::Key_Q :
        if ( m_bCtrlPressed)
            on_BtnExit_clicked();
        break ;

        case Qt::Key_A :
        OnZoomAll();
        break ;

        case  Qt::Key_L :
        on_BtnOpen_clicked();
        break ;

        case Qt::Key_H :
        OnHelp();
        break ;

        case Qt::Key_S :
        OnStartSlideShow();
        break ;

        case Qt::Key_E :
        OnShowLog() ;
        break ;

        case Qt::Key_Home :
        if ( m_bFullScreen)
            GoToStartEnd( true);
        break ;

        case Qt::Key_End :
        if( m_bFullScreen)
            GoToStartEnd( false);
        break ;

        case Qt::Key_C :
        OnConfig();
        break ;

        case Qt::Key_M :
        if ( m_player->state() == Phonon::PausedState)
            m_player->play();
        else
            m_player->pause();
        break ;

        case Qt::Key_Pause :
        OnPauseSlideShow();
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
void CPhoto::OnHelp() {
    m_bShowHelp = ! m_bShowHelp ;
    ui->ImgView->ShowHelp( m_bShowHelp);
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
    QString szLastDir ;
    QString szFilters ;

    szFilters = "List (*.txt)" ;
    m_pConf->GetStrProp( PROP_STR_LAST_DIR_LIST, &szLastDir) ;
    szFile    = QFileDialog::getSaveFileName( this, "Save File", szLastDir, szFilters) ;

    m_pConf->WriteList( szFile);
}

//----------------------------------------------------
void CPhoto::UpdateLayoutAfterResize( int nXMove, int nYMove)
{
    int     nId ;
    QPoint  cPt ;
    QSize   cSize ;

    for ( nId = 1 ;  nId <= RIGHT_BUTTON ;  nId++) {
        cPt = ui->RightButtons->button( nId)->pos() ;
        cPt.setX( cPt.x() + nXMove);
        cPt.setY( cPt.y() + nYMove);

        ui->RightButtons->button( nId)->move( cPt);
    }

    for ( nId = 1 ;  nId <= LEFT_BUTTON ;  nId++) {
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
        else if ( e->button() == Qt::LeftButton) {
            ui->ImgView->StartZoomRect( pos);
            setCursor( Qt::CrossCursor);
        }
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
void CPhoto::EndDrag( void)
{
    setCursor( Qt::ArrowCursor);
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
    int nOldLang ;
    int nNewLang ;

    m_pConf->GetIntProp( PROP_INT_LANG, &nOldLang) ;

    m_pConf->ShowSettingsDlg() ;

    m_pConf->GetIntProp( PROP_INT_LANG, &nNewLang) ;

    if( nNewLang != nOldLang)
        ChangeLang( nNewLang);
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
        ui->ImgView->SetFullScreen( true);
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
        ui->ImgView->SetFullScreen( false);

        if ( m_pTimer->isActive()) {
            m_pTimer->stop();
            OnEndSlideShow();
        }
    }

    m_bFullScreen = ! m_bFullScreen ;
    m_pFullScreenTimer->setInterval( MSEC_INI_LOAD);
    m_pFullScreenTimer->start();
}

//----------------------------------------------------
void CPhoto::OnStartSlideShow()
{
    QString     szSongs ;
    QStringList lszPlayList ;
    int         nSec ;
    int         n ;

    if ( ! m_bFullScreen)
        SwitchFullScreen();

    m_pStartSlideShowAct->setEnabled( false);
    m_pEndSlideShowAct->setEnabled( true);
    m_pPauseSlideShowAct->setEnabled( true);
    m_pExitFullScreen->setEnabled( false);

    m_nCurr = -1 ;
    on_BtnRight_clicked();
    m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;
    ui->ImgView->SetSlideShow( true);
    m_pTimer->setInterval( nSec * 1000);
    m_pTimer->start();
    ui->ImgView->DrawPlay();

    m_pConf->GetStrProp( PROP_STR_SONGS, &szSongs) ;
    lszPlayList = szSongs.split( ";") ;
    m_player->clearQueue();
    for ( n = 0 ;  n < lszPlayList.count() ;  n ++)
        m_player->enqueue( Phonon::MediaSource( lszPlayList.at(n)));

    m_player->play();
}

//----------------------------------------------------
void CPhoto::OnEndSlideShow()
{
    ui->ImgView->SetSlideShow(        false);
    m_pPauseSlideShowAct->setEnabled( false);
    m_pEndSlideShowAct->setEnabled(   false);
    m_pStartSlideShowAct->setEnabled( true);
    m_pExitFullScreen->setEnabled( true);

    m_pTimer->stop();
    m_player->stop();
}

//----------------------------------------------------
void CPhoto::OnPauseSlideShow()
{
    if ( m_pTimer->isActive()) {
        m_pTimer->stop();
        ui->ImgView->DrawPause();
        m_player->pause();
    }
    else {
        SeeNextImg();
        m_pTimer->start();
        ui->ImgView->DrawPlay();
        m_player->play();
    }

}

//----------------------------------------------------
void CPhoto::OnShowLog()
{
    QPoint pos ;
    QSize  size ;
    QRect  rect ;

    pos  = ui->ImgList->pos() ;
    size = ui->ImgList->size() ;

    rect.setTopLeft( pos);
    rect.setSize( size);

    m_cErrDlg.DoShow( rect, m_szLog) ;

    m_szLog.clear();
}

//----------------------------------------------------
void CPhoto::GoToStartEnd( bool bStart)
{
    if ( bStart)
        m_nCurr = -1 ;
    else
        m_nCurr = ui->ImgList->count() - 2 ;

    on_BtnRight_clicked();
}

//----------------------------------------------------
void CPhoto::on_BtnConfig_clicked()
{
    OnConfig();
}

//----------------------------------------------------
QSize  CPhoto::GetSceneSize()
{
    return ui->ImgView->size() ;
}

//----------------------------------------------------
void CPhoto::on_BtnLibrary_clicked()
{
    m_cCollDlg.show();
}
