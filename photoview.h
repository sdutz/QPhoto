#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QGraphicsView>

class PhotoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PhotoView(QWidget *parent = 0);
    ~PhotoView() ;
    bool     ShowPhoto( const QString& szFile) ;
    void     ZoomOut() ;
    void     ZoomIn() ;
    void     ZoomAll() ;
    void     ResetView() ;
    void     StartZoomRect( const QPoint& pos) ;
    void     SetShiftPressed( bool bPress) ;

private:

    void     EndZoomRect() ;

signals:

public slots:
    void     mouseMoveEvent( QMouseEvent* e);
    void     mouseReleaseEvent( QMouseEvent* e);
    void     wheelEvent( QWheelEvent* e);

private:
    QWidget*            m_pParent ;
    QGraphicsScene*     m_pScene ;
    QGraphicsRectItem*  m_pRect ;
    QPixmap             m_cImage ;
    QPointF             m_Pt ;
    bool                m_bDrag ;
    bool                m_bShift ;
    float               m_dScale ;
};

#endif // PHOTOVIEW_H
