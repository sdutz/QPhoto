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


#ifndef CPHOTO_H
#define CPHOTO_H

#include <QDialog>
#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QMenu>
#include "confmgr.h"
#include "errordlg.h"

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
    void ShowList(  const QString& szFile = "") ;
    void DeleteSingle();
    void DeleteAll();
    void LoadImage();
    void LoadFile();
    void UpdateLayoutAfterResize( int nXMove, int nYMove) ;
    void SetIds();
    void ResetView();
    void ShowAboutDlg() ;
    void ShowContextMenu(   const QPoint& pos);
    void ShowSlideShowMenu( const QPoint& pos);
    void CreateActions() ;
    void BuildContextMenu() ;
    void BuildSlideShowMenu() ;
    void SeePrevImg() ;
    void SeeNextImg() ;
    bool IsPosOnView(     const QPoint& pos) ;
    void DoDebug(         const QString& szDebug) ;
    void SetToolTips() ;
    void RefreshList() ;
    void DeleteAction() ;
    void InitLogDlg() ;
    void GoToStartEnd( bool bStart) ;

private slots:
    void on_BtnOpen_clicked();
    void on_BtnExit_clicked();
    void on_ImgList_itemDoubleClicked( QListWidgetItem* item);
    void on_BtnMinus_clicked();
    void on_BtnPlus_clicked();
    void on_BtnLeft_clicked();
    void on_BtnRight_clicked();
    void keyPressEvent (  QKeyEvent * e ) ;
    void keyReleaseEvent( QKeyEvent* e);
    void mousePressEvent( QMouseEvent* e);
    void on_BtnDel_clicked();
    void on_BtnSave_clicked();
    void resizeEvent(     QResizeEvent* event) ;
    void OnMoveCurrUp() ;
    void OnMoveCurrDown() ;
    void OnZoomAll() ;
    void OnConfig() ;
    void OnStartSlideShow() ;
    void OnEndSlideShow() ;
    void OnPauseSlideShow() ;
    void SwitchFullScreen() ;
    void OnHelp() ;
    void OnShowLog() ;

private:
    Ui::CPhoto      *ui;
    bool            m_bShiftPressed ;
    bool            m_bOrdChanged ;
    bool            m_bFullScreen ;
    bool            m_bCtrlPressed ;
    bool            m_bShowHelp ;
    int             m_nCurr ;
    QString         m_szFileName ;
    QString         m_szLog ;
    QAction*        m_pMoveUpAct ;
    QAction*        m_pMoveDownAct ;
    QAction*        m_pZoomAllAct ;
    QAction*        m_pConfigAct ;
    QAction*        m_pStartSlideShowAct ;
    QAction*        m_pEndSlideShowAct ;
    QAction*        m_pPauseSlideShowAct ;
    QAction*        m_pShowFullScreen ;
    QAction*        m_pExitFullScreen ;
    QMenu           m_ContextMenu ;
    QMenu           m_SlideShowMenu ;
    QSize           m_DiffSize ;
    ErrorDlg        m_cErrDlg ;
    ConfMgr*        m_pConf ;
    QTimer*         m_pTimer ;

};

#endif // CPHOTO_H
