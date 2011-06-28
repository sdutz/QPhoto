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


#include "photoview.h"
#include "cphoto.h"
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QUrl>
#include <QGraphicsSceneDragDropEvent>

//----------------------------------------------------
#define SCALE_FACTOR_MINUS 0.9
#define SCALE_FACTOR_PLUS  1.1
#define SCALE_FACTOR       0.1
#define OPACITY_FACTOR     0.1


//----------------------------------------------------
PhotoView::PhotoView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_pParent       = parent ;
    m_dScale        = 1. ;
    m_pScene        = new QGraphicsScene( this) ;
    setScene( m_pScene);
    m_bDrag         = false ;
    m_pRect         = NULL ;
    m_pHelpText     = NULL ;
    m_pCurrImgTitle = NULL ;
    m_pPrevImg      = NULL ;
    m_pCurrImg      = NULL ;
    m_bFullScreen   = false ;
    m_bSlideShow    = false ;
    setAcceptDrops( true);
    m_pTextTimer    = new QTimer( this) ;
    m_pFadeTimer    = new QTimer( this) ;
    connect( m_pTextTimer, SIGNAL( timeout()), this, SLOT( DecreaseAlfa())) ;
    connect( m_pFadeTimer, SIGNAL( timeout()), this, SLOT( DoFadeInOut())) ;
}

//----------------------------------------------------
PhotoView::~PhotoView()
{
    if ( m_pScene != NULL)
        delete m_pScene ;
    if ( m_pTextTimer != NULL)
        delete m_pTextTimer ;
    if ( m_pFadeTimer != NULL)
        delete m_pFadeTimer ;
}

//----------------------------------------------------
void PhotoView::mouseReleaseEvent( QMouseEvent* e)
{

    if ( e->type() == QEvent::MouseButtonRelease) {
        if ( e->button() == Qt::LeftButton  &&  m_bDrag)
            EndZoomRect();
    }
}

//----------------------------------------------------
void PhotoView::mouseMoveEvent( QMouseEvent* e)
{
    int     nWidth ;
    int     nHeight ;
    QPointF ptP ;
    QPen    pen ;
    QBrush  brush ;

    if ( ! m_bDrag) {
        if ( e->button() == Qt::LeftButton)
        return ;
    }

    ptP = mapToScene( e->pos()) ;

    nWidth  = ptP.x() - m_Pt.x() ;
    nHeight = ptP.y() - m_Pt.y() ;

    if ( m_pRect != NULL)
        m_pScene->removeItem( ( QGraphicsItem*) m_pRect) ;

    pen.setColor( GetColorFromConfig());

    m_pRect = m_pScene->addRect( m_Pt.x(), m_Pt.y(), nWidth, nHeight, pen) ;
}

//----------------------------------------------------
void PhotoView::wheelEvent( QWheelEvent* e)
{
    if ( ! m_bShift) {
        QGraphicsView::wheelEvent( e) ;
        return ;
    }

    if ( e->delta() > 0)
        ZoomIn();
    else
        ZoomOut();
}

//----------------------------------------------------
void PhotoView::ResetView( bool bClearAll)
{
    float diff ;

    diff = m_dScale - 1. ;
    if ( diff > 0.) {
        scale( diff, diff);
        m_dScale = 1. ;
    }

    centerOn(0, 0);

    if ( bClearAll)
        m_pScene->clear();
}

//----------------------------------------------------
bool
PhotoView::ShowPhoto( const QString& szFile)
{
    bool bRet ;
    bool bDoFade ;
    int  nSec ;
    int  nFadeType ;

    m_pConf->GetIntProp( PROP_INT_FADE, &nFadeType) ;

    ResetView( nFadeType == FADE_NONE);

    bRet = m_cImage.load( szFile) ;

    if ( m_pCurrImg != NULL)
        m_pPrevImg = m_pCurrImg ;

    m_pCurrImg = m_pScene->addPixmap( m_cImage) ;

    if ( bRet)
        setToolTip( szFile) ;

    if ( m_bFullScreen)
        SetCurrTitleOnScene() ;

    bDoFade = nFadeType == FADE_ALWAYS  ||
              ( nFadeType == FADE_ONSLIDESHOW  &&  m_bSlideShow) ;

    if ( bDoFade) {
        m_pCurrImg->setOpacity( 0.);
        m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;
        m_pFadeTimer->stop() ;
        m_pFadeTimer->setInterval( nSec * 10) ;
        m_pFadeTimer->start();
    }

    return bRet ;
}

//----------------------------------------------------
void PhotoView::ZoomOut() {

    scale( SCALE_FACTOR_MINUS, SCALE_FACTOR_MINUS) ;
    m_dScale -= SCALE_FACTOR ;
}

//----------------------------------------------------
void PhotoView::ZoomIn() {

    scale( SCALE_FACTOR_PLUS, SCALE_FACTOR_PLUS);
    m_dScale += SCALE_FACTOR ;
}


//----------------------------------------------------
void PhotoView::ZoomAll()
{
    QSize ImgSize ;

    ImgSize = m_cImage.size() ;

    fitInView( 0, 0, ImgSize.width(), ImgSize.height(), Qt::KeepAspectRatio);
}


//----------------------------------------------------
void PhotoView::StartZoomRect( const QPoint& pos)
{
    m_bDrag = true ;
    m_Pt    = mapToScene( pos) ;
}

//----------------------------------------------------
void PhotoView::EndZoomRect()
{
    QRectF cRect ;

    if ( m_pRect == NULL)
        return ;

    cRect   = m_pRect->rect() ;

    fitInView( cRect.topLeft().x(), cRect.topLeft().y(),
               cRect.width(), cRect.height(), Qt::KeepAspectRatio);

    m_bDrag = false ;
    m_pScene->removeItem( ( QGraphicsItem*) m_pRect);
    m_pRect = NULL ;
    m_pScene->invalidate();
}

//----------------------------------------------------
void PhotoView::ShowHelp( bool bShow)
{

    if ( bShow) {
        if ( m_pHelpText == NULL) {
            QString szHelp ;
            m_pConf->GetHelpFromFile( &szHelp) ;
            m_pHelpText = m_pScene->addText( szHelp, GetFontFromConfig()) ;
            m_pHelpText->setDefaultTextColor( GetColorFromConfig());
            m_pHelpText->setOpacity( 0.5);
        }
        else
            m_pHelpText->show();
    }
    else if ( ! bShow  &&  m_pHelpText != NULL)
        m_pHelpText->hide();
}


//----------------------------------------------------
void PhotoView::SetCurrTitleOnScene()
{
    int nSec ;

    m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;
    m_pCurrImgTitle = m_pScene->addText( toolTip(), GetFontFromConfig()) ;
    m_pCurrImgTitle->setDefaultTextColor( GetColorFromConfig());
    m_pCurrImgTitle->show();
    m_pTextTimer->stop();
    m_pTextTimer->setInterval( nSec * 50);
    m_pTextTimer->start() ;
}

//----------------------------------------------------
void PhotoView::SetFullScreen( bool bFullScreen)
{

    if ( ! bFullScreen  &&  m_pCurrImgTitle != NULL)
        m_pScene->removeItem( m_pCurrImgTitle);
    else if ( bFullScreen)
        SetCurrTitleOnScene();

    m_bFullScreen = bFullScreen ;
}

//----------------------------------------------------
void PhotoView::DecreaseAlfa()
{
    float fOpacity ;

    fOpacity = m_pCurrImgTitle->opacity() - OPACITY_FACTOR ;
    if ( fOpacity < OPACITY_FACTOR) {
        m_pCurrImgTitle->hide();
        m_pTextTimer->stop();
    }
    else
        m_pCurrImgTitle->setOpacity( fOpacity) ;
}

//----------------------------------------------------
void PhotoView::DoFadeInOut()
{
    float fOpacity ;

    if ( m_pPrevImg != NULL) {
        fOpacity = m_pPrevImg->opacity() - OPACITY_FACTOR ;
        m_pPrevImg->setOpacity( fOpacity);
    }


    fOpacity = m_pCurrImg->opacity() + OPACITY_FACTOR ;

    if ( fOpacity > 1 - OPACITY_FACTOR) {
        if ( m_pPrevImg != NULL)
            m_pScene->removeItem( m_pPrevImg);
        m_pFadeTimer->stop();
        m_pCurrImg->setOpacity( 1.);
    }
    else
        m_pCurrImg->setOpacity( fOpacity);

}

//----------------------------------------------------
QFont PhotoView::GetFontFromConfig()
{
    QFont   font ;
    QString szFont ;

    if ( ! m_pConf->GetStrProp( PROP_STR_FONT, &szFont) )
        return QFont() ;

    font.fromString( szFont) ;
    return font ;
}

//----------------------------------------------------
QColor PhotoView::GetColorFromConfig()
{
    QColor  color ;
    QString szColor ;

    if ( ! m_pConf->GetStrProp( PROP_STR_COLOR, &szColor))
        return Qt::black ;
    color.setNamedColor( szColor);
    if ( ! color.isValid())
        return Qt::black ;

    return color ;
}

//----------------------------------------------------
void PhotoView::dragEnterEvent( QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

//----------------------------------------------------
void PhotoView::dragLeaveEvent(   QDragLeaveEvent *event)
{
    event->accept();
}

//----------------------------------------------------
void PhotoView::dropEvent( QDropEvent *event)
{
    const QMimeData* pData ;

    pData = event->mimeData() ;

    if ( ! pData->hasUrls())
        return ;

    QUrl url = pData->urls().at(0) ;
    (( CPhoto*) m_pParent)->on_ImgDropped( url.toLocalFile());
}

//----------------------------------------------------
void PhotoView::dragMoveEvent( QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
