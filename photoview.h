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


#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QGraphicsView>
#include "confmgr.h"

class PhotoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PhotoView(QWidget *parent = 0);
    ~PhotoView() ;
    bool     ShowPhoto(       const QString& szFile) ;
    void     ZoomOut() ;
    void     ZoomIn() ;
    void     ZoomAll() ;
    void     ResetView() ;
    void     StartZoomRect(   const QPoint& pos) ;
    void     SetShiftPressed( bool bPress) ;
    void     SetConfMgr(      ConfMgr* pConf) { m_pConf = pConf ; } ;
    void     ShowHelp(        bool bShow) ;

private:

    void     EndZoomRect() ;

signals:

public slots:
    void     mouseMoveEvent(    QMouseEvent* e);
    void     mouseReleaseEvent( QMouseEvent* e);
    void     wheelEvent(        QWheelEvent* e);

private:
    QPixmap             m_cImage ;
    QPointF             m_Pt ;
    bool                m_bDrag ;
    bool                m_bShift ;
    float               m_dScale ;
    QWidget*            m_pParent ;
    QGraphicsScene*     m_pScene ;
    QGraphicsRectItem*  m_pRect ;
    QGraphicsTextItem*  m_pHelpText ;
    ConfMgr*            m_pConf ;
};

#endif // PHOTOVIEW_H
