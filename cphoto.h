#ifndef CPHOTO_H
#define CPHOTO_H

#include <QDialog>
#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QMenu>

namespace Ui {
    class CPhoto;
}

class CPhoto : public QDialog
{
    Q_OBJECT

public:
    explicit CPhoto(QWidget *parent = 0);
    ~CPhoto();

private :
    bool ShowPhoto( bool bToAddToList) ;
    bool FindInList( const QString& szFile) ;
    void WriteList( const QString& szFile = "") ;
    void LoadList( const QString& szFile = "") ;
    void DeleteSingle();
    void DeleteAll();
    void LoadImage();
    void LoadFile();
    void UpdateLayoutAfterResize( int nXMove, int nYMove) ;
    void SetIds();
    void ResetView();
    void ShowAboutDlg() ;
    void ShowContextMenu( const QPoint& pos);
    void CreateActions() ;
    void BuildMenu() ;
    void SeePrevImg() ;
    void SeeNextImg() ;
    bool IsPosOnView( const QPoint& pos) ;
    void DoDebug( const QString& szDebug) ;
    void SetToolTips() ;

private slots:
    void on_BtnOpen_clicked();
    void on_BtnExit_clicked();
    void on_ImgList_itemDoubleClicked( QListWidgetItem* item);
    void on_BtnMinus_clicked();
    void on_BtnPlus_clicked();
    void on_BtnLeft_clicked();
    void on_BtnRight_clicked();
    void keyPressEvent ( QKeyEvent * e ) ;
    void keyReleaseEvent( QKeyEvent* e);
    void mousePressEvent( QMouseEvent* e);
    void on_BtnDel_clicked();
    void on_BtnSave_clicked();
    void resizeEvent( QResizeEvent* event) ;
    void MoveCurrUp() ;
    void MoveCurrDown() ;
    void ZoomAll() ;


private:
    Ui::CPhoto      *ui;
    bool            m_bShiftPressed ;
    int             m_nCurr ;
    QString         m_szFileName ;
    QString         m_szLastDir ;
    QStringList     m_lszList ;
    QAction*        m_pMoveUpAct ;
    QAction*        m_pMoveDownAct ;
    QAction*        m_pZoomAllAct ;
    QMenu           m_ContextMenu ;

};

#endif // CPHOTO_H
