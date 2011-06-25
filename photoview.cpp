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

//----------------------------------------------------
#define SCALE_FACTOR_MINUS 0.9
#define SCALE_FACTOR_PLUS  1.1
#define SCALE_FACTOR       0.1


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
    m_bFullScreen   = false ;

    m_pTimer = new QTimer( this) ;
    connect( m_pTimer, SIGNAL( timeout()), this, SLOT( DecreaseAlfa())) ;
}

//----------------------------------------------------
PhotoView::~PhotoView()
{
    if ( m_pScene != NULL)
        delete m_pScene ;
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
void PhotoView::SetShiftPressed( bool bPress)
{
    m_bShift = bPress ;
}

//----------------------------------------------------
void PhotoView::mouseMoveEvent( QMouseEvent* e)
{
    int     nWidth ;
    int     nHeight ;
    QPointF ptP ;
    QPen    pen ;
    QBrush  brush ;

    if ( ! m_bDrag)
        return ;

    ptP = mapToScene( e->pos()) ;

    nWidth  = ptP.x() - m_Pt.x() ;
    nHeight = ptP.y() - m_Pt.y() ;

    if ( m_pRect != NULL)
        m_pScene->removeItem( ( QGraphicsItem*) m_pRect) ;

    pen.setColor( m_pConf->GetColor());

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
void PhotoView::ResetView()
{
    float diff ;

    diff = m_dScale - 1. ;
    if ( diff > 0.) {
        scale( diff, diff);
        m_dScale = 1. ;
    }
    m_pScene->clear();
    centerOn(0, 0);
}

//----------------------------------------------------
bool
PhotoView::ShowPhoto( const QString& szFile)
{
    bool bRet ;

    ResetView();

    bRet = m_cImage.load( szFile) ;

    bRet = bRet  &&  m_pScene->addPixmap( m_cImage) ;

    if ( bRet)
        setToolTip( szFile) ;

    if ( m_bFullScreen)
        SetCurrTitleOnScene() ;

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
            m_pHelpText = m_pScene->addText( szHelp, m_pConf->GetFont()) ;
            m_pHelpText->setDefaultTextColor( m_pConf->GetColor());
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

    m_pCurrImgTitle = m_pScene->addText( toolTip(), m_pConf->GetFont()) ;
    m_pCurrImgTitle->setDefaultTextColor( m_pConf->GetColor());
    m_pCurrImgTitle->show();
    m_pTimer->stop();
    m_pTimer->setInterval( m_pConf->GetSeconds() * 50);
    m_pTimer->start() ;
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

    fOpacity = m_pCurrImgTitle->opacity() - 0.1 ;
    if ( fOpacity < 0.1) {
        m_pCurrImgTitle->hide();
        m_pTimer->stop();
    }
    else
        m_pCurrImgTitle->setOpacity( fOpacity) ;
}
