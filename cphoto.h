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
#include "collectionmgr.h"
#include "errordlg.h"
#include "collectiondlg.h"
#include <QTranslator>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

namespace Ui {
    class CPhoto;
}

class CPhoto : public QDialog
{
    Q_OBJECT

public:
    explicit CPhoto(QWidget *parent = 0);
    ~CPhoto();
    void     on_ImgDropped( const QString& szFile, bool bShow) ;
    void     on_DirDropped( const QString& szDir,  bool bShow) ;
    void     on_UrlDropped( const QString& szUrl,  bool bShow) ;
    void     EndDrag() ;
    QSize    GetSceneSize() ;
    void     ShowList( const QString& szFile = "") ;
    void     OnSave() ;
    void     SetCurrImgNotes( const QString& szNotes) ;


private :
    bool ShowPhoto( bool bToAddToList, bool bShow = true) ;
    void DeleteSingle();
    void DeleteAll();
    void LoadImage( bool bShow = true);
    void LoadImages() ;
    void LoadFile();
    void UpdateLayoutAfterResize( int nXMove, int nYMove) ;
    void SetIds();
    void ResetView();
    void ShowAboutDlg() ;
    void ShowContextMenu(   const QPoint& pos);
    void ShowSlideShowMenu( const QPoint& pos);
    void CreateActions() ;
    void CreateTimers() ;
    void BuildContextMenu() ;
    void BuildSlideShowMenu() ;
    void SeePrevImg() ;
    void SeeNextImg() ;
    bool IsPosOnView(     const QPoint& pos) ;
    void RefreshList() ;
    void DeleteAction() ;
    void DeleteTimers() ;
    void InitDialogs() ;
    void SetBtnIcons() ;
    void GoToStartEnd( bool bStart) ;
    void InitPlayer() ;
    void InitLang() ;
    void ChangeLang( int nLang) ;
    void RetranslateDialog() ;
    void UpdateCollDlg() ;

private slots:
    void on_BtnOpen_clicked();
    void on_BtnExit_clicked();
    void on_ImgList_itemDoubleClicked( QListWidgetItem* item);
    void on_BtnMinus_clicked();
    void on_BtnPlus_clicked();
    void on_BtnLeft_clicked();
    void on_BtnRight_clicked();
    void OnIniLoad();
    void OnFullSw() ;
    void keyPressEvent (  QKeyEvent * e ) ;
    void keyReleaseEvent( QKeyEvent* e);
    void mousePressEvent( QMouseEvent* e);
    void resizeEvent(     QResizeEvent* event) ;
    void moveEvent(       QMoveEvent* event);
    void closeEvent(      QCloseEvent * e) ;
    void on_BtnDel_clicked();
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
    void on_BtnConfig_clicked();
    void on_BtnLibrary_clicked();
    void on_BtnNotes_clicked();
    void OnStartEditNotes() ;

private:
    Ui::CPhoto*          ui;
    bool                 m_bShiftPressed ;
    bool                 m_bChanged ;
    bool                 m_bFullScreen ;
    bool                 m_bCtrlPressed ;
    bool                 m_bShowHelp ;
    bool                 m_bLoadSingle ;
    int                  m_nCurr ;
    QString              m_szFileName ;
    QString              m_szPrev ;
    QString              m_szLog ;
    QString              m_szExt ;
    QString              m_szFilters ;
    QAction*             m_pMoveUpAct ;
    QAction*             m_pMoveDownAct ;
    QAction*             m_pZoomAllAct ;
    QAction*             m_pConfigAct ;
    QAction*             m_pStartSlideShowAct ;
    QAction*             m_pEndSlideShowAct ;
    QAction*             m_pPauseSlideShowAct ;
    QAction*             m_pShowFullScreen ;
    QAction*             m_pExitFullScreen ;
    QAction*             m_pEditNotesAct ;
    QMenu                m_ContextMenu ;
    QMenu                m_SlideShowMenu ;
    QSize                m_DiffSize ;
    ErrorDlg             m_cErrDlg ;
    CollectionDlg        m_cCollDlg ;
    ConfMgr*             m_pConf ;
    CollectionMgr*       m_pColl ;
    QTimer*              m_pTimer ;
    QTimer*              m_pLoadIniTimer ;
    QTimer*              m_pFullScreenTimer ;
    QMediaPlayer         m_player ;
    QMediaPlaylist       m_playList ;
    QTranslator          m_cTranslator ;

};

#endif // CPHOTO_H
