#include "musicmgr.h"
#include "ui_musicmgr.h"
#include "util.h"
#include <QFileDialog>
#include <QKeyEvent>


//----------------------------------------------------
MusicMgr::MusicMgr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicMgr)
{
    ui->setupUi(this);
    setMinimumSize( width(), height());
    setMaximumSize( width(), height());
    setWindowTitle( "Music Manager");
    m_bShiftPressed = false ;
    setAcceptDrops( true);
    m_player = NULL ;
    SetBtnIcons();
    SetToolTips();
    InitPlayer() ;
}

//----------------------------------------------------
MusicMgr::~MusicMgr()
{
    if( m_player != NULL)
        delete m_player ;

    delete ui;
}

//----------------------------------------------------
void MusicMgr::SetToolTips()
{
    ui->BtnAdd->setToolTip( "Add Music");
}

//----------------------------------------------------
void MusicMgr::SetBtnIcons( void)
{
    QIcon   icon ;
    QSize   pixSize ;


    GetPixBtnSize( ui->BtnAdd->size(), &pixSize) ;
    icon.addFile( "icons/sound_add.png", pixSize) ;
    ui->BtnAdd->setIcon( icon);

    GetPixBtnSize( ui->BtnRemove->size(), &pixSize) ;
    icon.addFile( "icons/sound_delete.png", pixSize) ;
    ui->BtnRemove->setIcon( icon);

    GetPixBtnSize( ui->BtnPreviewStart->size(), &pixSize) ;
    icon.addFile( "icons/control_play_blue.png", pixSize) ;
    ui->BtnPreviewStart->setIcon( icon);

    GetPixBtnSize( ui->BtnPreviewStop->size(), &pixSize) ;
    icon.addFile( "icons/control_stop_blue.png", pixSize) ;
    ui->BtnPreviewStop->setIcon( icon);

    GetPixBtnSize( ui->BtnCancel->size(), &pixSize) ;
    icon.addFile( "icons/cross.png", pixSize) ;
    ui->BtnCancel->setIcon( icon);

    GetPixBtnSize( ui->BtnOk->size(), &pixSize) ;
    icon.addFile( "icons/accept.png", pixSize) ;
    ui->BtnOk->setIcon( icon);
}


//----------------------------------------------------
void MusicMgr::InitPlayer()
{
  m_player  = Phonon::createPlayer( Phonon::MusicCategory) ;
}

//----------------------------------------------------
void MusicMgr::on_BtnAdd_clicked()
{
    int         n ;
    QString     szFilters ;
    QString     szCurr ;
    QStringList lszList ;


    szFilters    = "Songs (*.ogg *.mp3)" ;
    lszList      = QFileDialog::getOpenFileNames( this, "Open Files",
                   "", szFilters) ;

    for( n = 0 ;  n < lszList.count() ;  n ++) {
        szCurr = lszList.at(n) ;
        ui->SongsList->addItem( szCurr);
        m_lFilesList.append( szCurr);
    }

}

//----------------------------------------------------
void MusicMgr::keyPressEvent ( QKeyEvent* e)
{
    if ( e->key() == Qt::Key_Shift)
        m_bShiftPressed = true ;
}

//----------------------------------------------------
void MusicMgr::keyReleaseEvent(  QKeyEvent * e )
{
    if ( e->key() == Qt::Key_Shift)
        m_bShiftPressed = false ;
}

//----------------------------------------------------
void MusicMgr::on_BtnRemove_clicked()
{
    int nCurr ;

    if ( ! m_bShiftPressed) {
        nCurr = ui->SongsList->currentRow() ;
        ui->SongsList->takeItem( nCurr) ;
        m_lFilesList.takeAt( nCurr) ;
    }
    else {
        ui->SongsList->clear();
        m_lFilesList.clear();
    }
}

//----------------------------------------------------
void MusicMgr::on_BtnPreviewStart_clicked()
{
    QString szCurr ;

    szCurr = ui->SongsList->currentItem()->text() ;

    if ( szCurr.isEmpty())
        return ;

    m_player->setCurrentSource( Phonon::MediaSource( szCurr));
    m_player->play();
}

//----------------------------------------------------
void MusicMgr::on_BtnPreviewStop_clicked()
{
    m_player->stop();
}

//----------------------------------------------------
void MusicMgr::on_BtnCancel_clicked()
{
    QDialog::reject() ;
}

//----------------------------------------------------
void MusicMgr::on_BtnOk_clicked()
{
    QDialog::accept() ;
}

//----------------------------------------------------
void MusicMgr::SetInitList( const QString& szFiles)
{
    int     n ;
    QString szCurr ;

    m_lFilesList.append( szFiles.split( ";"));

    for( n = 0 ;  n < m_lFilesList.count() ;  n ++) {
        szCurr = m_lFilesList.at(n) ;
        ui->SongsList->addItem( szCurr);
    }
}

//----------------------------------------------------
void MusicMgr::GetList( QString* pszFiles)
{

    if ( pszFiles == NULL)
        return ;

    *pszFiles = m_lFilesList.join( ";") ;
}

//----------------------------------------------------
void MusicMgr::on_SongsList_itemDoubleClicked(QListWidgetItem* item)
{
    m_player->stop();
    on_BtnPreviewStart_clicked();
}


//----------------------------------------------------
void MusicMgr::dragEnterEvent( QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

//----------------------------------------------------
void MusicMgr::dragLeaveEvent(   QDragLeaveEvent *event)
{
    event->accept();
}

//----------------------------------------------------
void MusicMgr::dropEvent( QDropEvent *event)
{
    const QMimeData* pData ;

    pData = event->mimeData() ;

    if ( ! pData->hasUrls())
        return ;

    int  n ;
    QUrl url ;

    for ( n = 0 ;  n < pData->urls().count() ;  n ++) {
        url = pData->urls().at(n) ;
        ui->SongsList->addItem( url.toLocalFile());
    }
}

//----------------------------------------------------
void MusicMgr::dragMoveEvent( QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
