#include "cphoto.h"
#include "ui_cphoto.h"
#include <QFileDialog>
#include <QTextStream>
#include <QKeyEvent>
#include <QMessageBox>
#include "aboutdlg.h"


//----------------------------------------------------
#define HISTORY_FILE       "history.txt"
#define MIN_WIDTH          814
#define MIN_HEIGHT         764


//----------------------------------------------------
CPhoto::CPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPhoto)
{
    ui->setupUi(this);
    m_szFileName    = "" ;
    m_nCurr         = -1 ;
   // TODO recuperare la cartella dei documenti!
    m_szLastDir     = "/home/sdutz" ;
    m_bShiftPressed = false ;
    setMinimumSize( MIN_WIDTH, MIN_HEIGHT);

    SetIds();
    CreateActions();
    BuildMenu();
    LoadList();
    SetToolTips() ;
}

//----------------------------------------------------
CPhoto::~CPhoto()
{
    delete ui;
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
    connect( m_pMoveUpAct, SIGNAL( triggered()), this, SLOT( MoveCurrUp())) ;

    m_pMoveDownAct = new QAction( "MoveDown", this) ;
    connect( m_pMoveDownAct, SIGNAL( triggered()), this, SLOT( MoveCurrDown())) ;

    m_pZoomAllAct = new QAction( "ZoomAll", this) ;
    connect( m_pZoomAllAct, SIGNAL( triggered()), this, SLOT( ZoomAll())) ;
}

//----------------------------------------------------
void CPhoto::BuildMenu()
{
    m_ContextMenu.addAction( m_pMoveUpAct) ;
    m_ContextMenu.addAction( m_pMoveDownAct) ;
    m_ContextMenu.addAction( m_pZoomAllAct) ;
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

    szFilters = "List (*.txt)" ;
    szFile    = QFileDialog::getOpenFileName( this, "Open File",
                             m_szLastDir, szFilters) ;

    DeleteAll();
    LoadList( szFile);
}

//----------------------------------------------------
void CPhoto::LoadImage()
{
    int     nIdx ;
    QString szFilters ;
    QString szHomePath ;


    szFilters    = "Images (*.jpeg *.jpg)" ;
    m_szFileName = QFileDialog::getOpenFileName( this, "Open File",
                             m_szLastDir, szFilters) ;

    nIdx         = m_szFileName.lastIndexOf( "\\") ;
    m_szLastDir  = m_szFileName.left( nIdx) ;

    if( ! m_szFileName.isEmpty())
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
        if( bToAddToList  &&  ! FindInList( m_szFileName))
            ui->ImgList->addItem( m_szFileName) ;
    }

    return true ;
}

//----------------------------------------------------
bool CPhoto::FindInList( const QString& szFile)
{
    int     n ;
    QString szCurr ;

    for ( n = 0 ;  n < m_lszList.count() ;  n ++) {
        szCurr = m_lszList.at( n) ;
        if ( szCurr.compare( szFile) == 0)
            return true ;
    }

    return false ;
}

//----------------------------------------------------
void CPhoto::on_BtnExit_clicked()
{
    WriteList();
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
void CPhoto::WriteList( const QString& szFile)
{
    int     n ;
    int     nItems ;
    QString szMyFile ;

    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;

    QFile cFile( szMyFile) ;

    nItems = ui->ImgList->count() ;

    if( ! cFile.open( QIODevice::WriteOnly | QIODevice::Text))
        return ;

    QTextStream out( &cFile) ;

    for( n = 0 ;  n < nItems ;  n ++)
        out<<ui->ImgList->item(n)->text()<<endl ;

    cFile.close();
}

//----------------------------------------------------
void CPhoto::DoDebug( const QString& szDebug)
{
    QMessageBox box ;
    box.setText( szDebug);
    box.exec() ;
}


//----------------------------------------------------
void CPhoto::LoadList(  const QString& szFile)
{
    QString szTmp ;
    QString szMyFile ;

    szMyFile = szFile.isEmpty() ? HISTORY_FILE : szFile ;

    QFile   cFile( szMyFile) ;

    if ( ! cFile.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in( &cFile) ;

     while ( ! in.atEnd()) {
         szTmp = in.readLine() ;
         ui->ImgList->addItem( szTmp) ;
         m_lszList.append( szTmp) ;
     }

     cFile.close();

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
}

//----------------------------------------------------
void CPhoto::on_BtnLeft_clicked()
{
    if ( m_bShiftPressed)
        MoveCurrUp() ;
    else
        SeePrevImg() ;
}

//----------------------------------------------------
void CPhoto::on_BtnRight_clicked()
{
    if ( m_bShiftPressed)
        MoveCurrDown();
    else
        SeeNextImg();
}

//----------------------------------------------------
void CPhoto::DeleteAll()
{
    m_lszList.clear();
    ui->ImgList->clear();
    WriteList();
    ui->ImgView->ResetView();
}

//----------------------------------------------------
void CPhoto::DeleteSingle()
{
    ui->ImgList->takeItem( m_nCurr);

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
    }
}

//----------------------------------------------------
void CPhoto::keyReleaseEvent( QKeyEvent* e)
{
    if ( e->key() != Qt::Key_Shift)
        return ;
    m_bShiftPressed = false ;
    ui->ImgView->SetShiftPressed( false);

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
                             m_szLastDir, szFilters) ;

    WriteList( szFile);
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
        if ( e->button() == Qt::RightButton)
            ShowContextMenu( QPoint( e->globalX(), e->globalY()));
        else if ( e->button() == Qt::LeftButton)
            ui->ImgView->StartZoomRect( pos);
    }
}


//----------------------------------------------------
void CPhoto::MoveCurrUp()
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
}

//----------------------------------------------------
void CPhoto::MoveCurrDown()
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
}

//----------------------------------------------------
void CPhoto::ZoomAll()
{
    ui->ImgView->ZoomAll();
}

