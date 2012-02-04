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
    m_pParent        = ( CPhoto*) parent ;
    m_dScale         = 1. ;
    m_pScene         = new QGraphicsScene( this) ;
    setScene( m_pScene);
    m_bDrag          = false ;
    m_pRect          = NULL ;
    m_pHelpText      = NULL ;
    m_pCurrImgTitle  = NULL ;
    m_pPrevImg       = NULL ;
    m_pCurrImg       = NULL ;
    m_pauseGrp       = NULL ;
    m_pTria          = NULL ;
    m_bFullScreen    = false ;
    m_bSlideShow     = false ;
    setAcceptDrops( true);
    m_pTextTimer     = new QTimer( this) ;
    m_pFadeTimer     = new QTimer( this) ;
    m_pControlsTimer = new QTimer( this) ;
    connect( m_pTextTimer, SIGNAL( timeout()), this, SLOT( DecreaseAlfa())) ;
    connect( m_pFadeTimer, SIGNAL( timeout()), this, SLOT( DoFadeInOut())) ;
    connect( m_pControlsTimer, SIGNAL( timeout()), this, SLOT( DoControlsFadeOut())) ;
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
    if ( m_pControlsTimer != NULL)
        delete m_pControlsTimer ;
}


//----------------------------------------------------
void PhotoView::PrepareSlideshowItems()
{
    QPen                   pen ;
    QBrush                 brush ;
    QList< QGraphicsItem*> list ;
    QVector< QPoint>       points ;
    QRect     rect1(0, 0, 20, 60 ) ;
    QRect     rect2(40, 0, 20, 60 ) ;

    pen.setColor( GetColorFromConfig());
    brush.setColor( GetColorFromConfig());
    brush.setStyle( Qt::SolidPattern);
    list.append( m_pScene->addRect( rect1, pen, brush)) ;
    list.append( m_pScene->addRect( rect2, pen, brush)) ;
    m_pauseGrp = m_pScene->createItemGroup( list) ;
    m_pauseGrp->setVisible( false);

    points.append( QPoint( 0,0));
    points.append( QPoint( 50, 50));
    points.append( QPoint( 0, 100));
    QPolygon  poly( points) ;
    m_pTria = m_pScene->addPolygon( poly, pen, brush) ;
    m_pTria->setVisible( false);
}

//----------------------------------------------------
void PhotoView::DrawPlay()
{
    QSize   sceneSize ;
    QSizeF  grpSize ;
    QPointF pos ;


    sceneSize = m_pParent->GetSceneSize() ;
    grpSize   = m_pTria->boundingRect().size();
    pos.setX( sceneSize.width() * 0.5 - grpSize.width() * 0.5);
    pos.setY( sceneSize.height() * 0.5 - grpSize.height() * 0.5);

    m_pTria->setPos( pos);
    m_pTria->setVisible( true);
    StartControlsTimer();
}


//----------------------------------------------------
void PhotoView::StartControlsTimer()
{
    int nSec ;

    m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;

    if ( m_pControlsTimer->isActive())
        m_pControlsTimer->stop();
    m_pControlsTimer->setInterval( nSec * 50);
    m_pControlsTimer->start();
}

//----------------------------------------------------
void PhotoView::DrawPause()
{
    QSize   sceneSize ;
    QSizeF  grpSize ;
    QPointF pos ;


    sceneSize = m_pParent->GetSceneSize() ;
    grpSize   = m_pauseGrp->boundingRect().size() ;
    pos.setX( sceneSize.width() * 0.5 - grpSize.width() * 0.5);
    pos.setY( sceneSize.height() * 0.5 - grpSize.height() * 0.5);

    m_pauseGrp->setPos( pos);
    m_pauseGrp->setVisible( true);
    StartControlsTimer();
}

//----------------------------------------------------
void PhotoView::mouseMoveEvent( QMouseEvent* e)
{
    int     nWidth ;
    int     nHeight ;
    QPointF ptP ;
    QPen    pen ;
    QRect   rect ;

    if ( ! m_bDrag)
        return ;

    ptP = mapToScene( e->pos()) ;

    nWidth  = ptP.x() - m_Pt.x() ;
    nHeight = ptP.y() - m_Pt.y() ;

    if ( m_pRect != NULL)
        m_pScene->removeItem( ( QGraphicsItem*) m_pRect) ;

    pen.setColor( GetColorFromConfig());

    if ( nWidth > 0  &&  nHeight > 0) {
        rect.setX( m_Pt.x());
        rect.setY( m_Pt.y());
    }
    else if ( nWidth < 0  &&  nHeight < 0) {
        rect.setX( ptP.x());
        rect.setY( ptP.y());
    }
    else if ( nWidth < 0) {
        rect.setX( ptP.x());
        rect.setY( m_Pt.y());
    }
    else if ( nHeight < 0) {
        rect.setX( m_Pt.x());
        rect.setY( ptP.y());
    }

    rect.setWidth( abs( nWidth));
    rect.setHeight( abs( nHeight));

    m_pRect = m_pScene->addRect( rect, pen) ;
}


//----------------------------------------------------
void PhotoView::mouseReleaseEvent( QMouseEvent* e)
{

    if ( e->type() == QEvent::MouseButtonRelease) {
        if ( e->button() == Qt::LeftButton  &&  m_bDrag) {
            EndZoomRect();
            m_pParent->EndDrag();
        }
    }

    m_bDrag = false ;
}

//----------------------------------------------------
void PhotoView::wheelEvent( QWheelEvent* e)
{
    if ( ! m_bShift) {
        QGraphicsView::wheelEvent( e) ;
        return ;
    }

    e->delta() > 0 ? ZoomIn() : ZoomOut();
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

    if ( bClearAll) {
        m_pScene->clear();
        PrepareSlideshowItems();
    }
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

    ResetView();


    bDoFade = nFadeType == FADE_ALWAYS  ||
              ( nFadeType == FADE_ONSLIDESHOW  &&  m_bSlideShow) ;

    if ( bDoFade)
        CheckFading() ;

    if ( m_pCurrImg != NULL)
        m_pPrevImg = m_pCurrImg ;

    if ( nFadeType == FADE_NONE)
        m_pScene->removeItem( m_pPrevImg);

    bRet = m_cImage.load( szFile) ;

    m_pCurrImg = m_pScene->addPixmap( m_cImage) ;

    if ( bDoFade) {
        m_pCurrImg->setOpacity( 0.);
        m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;
        m_pFadeTimer->setInterval( nSec * 10) ;
        m_pFadeTimer->start();
    }

    if ( bRet)
        setToolTip( szFile) ;

    if ( m_bFullScreen)
        SetCurrTitleOnScene() ;

    return bRet ;
}

//----------------------------------------------------
void
PhotoView::CheckFading()
{
    if ( m_pFadeTimer->isActive()) {
        m_pFadeTimer->stop() ;
        if ( m_pPrevImg != NULL)
            m_pScene->removeItem( m_pPrevImg);
        m_pCurrImg->setOpacity( 1.);
    }

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
    setSceneRect( m_cImage.rect());
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

    if( cRect.width() > 10 && cRect.height() > 10)
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

    if ( m_pCurrImgTitle != NULL)
        m_pCurrImgTitle->hide();
    if ( m_pTextTimer->isActive())
        m_pTextTimer->stop();

    m_pConf->GetIntProp( PROP_INT_SEC, &nSec) ;
    m_pCurrImgTitle = m_pScene->addText( toolTip(), GetFontFromConfig()) ;
    m_pCurrImgTitle->setDefaultTextColor( GetColorFromConfig());
    m_pCurrImgTitle->show();
    m_pTextTimer->setInterval( nSec * 35);
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

    if ( fOpacity > 1 - OPACITY_FACTOR)
        CheckFading() ;
    else
        m_pCurrImg->setOpacity( fOpacity);
}

//----------------------------------------------------
void PhotoView::DoControlsFadeOut()
{
    float          fOpacity ;
    QGraphicsItem* pItem ;

    if ( m_pTria->isVisible())
        pItem = m_pTria ;
    else
        pItem = m_pauseGrp ;

    fOpacity = pItem->opacity() - OPACITY_FACTOR ;
    if ( fOpacity < OPACITY_FACTOR) {
        pItem->setVisible( false);
        m_pControlsTimer->stop();
    }
    else
        pItem->setOpacity( fOpacity);
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
void PhotoView::dragLeaveEvent( QDragLeaveEvent *event)
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

    int  n ;
    QUrl url ;

    for ( n = 0 ;  n < pData->urls().count() ;  n ++) {
        url = pData->urls().at(n) ;
        m_pParent->on_ImgDropped( url.toLocalFile(), n == pData->urls().count() - 1);
    }
}

//----------------------------------------------------
void PhotoView::dragMoveEvent( QDragMoveEvent *event)
{
    event->acceptProposedAction();
}    
