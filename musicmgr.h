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


#ifndef MUSICMGR_H
#define MUSICMGR_H

#include <QDialog>
#include <QListWidgetItem>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

namespace Ui {
    class MusicMgr;
}

class MusicMgr : public QDialog
{
    Q_OBJECT

public:
    explicit MusicMgr(QWidget *parent = 0);
    ~MusicMgr();
    void SetInitList( const QString& szFiles) ;
    void GetList(     QString* pszFiles) ;

protected:
    void dragEnterEvent(   QDragEnterEvent *event);
    void dropEvent(        QDropEvent *event);
    void dragLeaveEvent(   QDragLeaveEvent *event);
    void dragMoveEvent(    QDragMoveEvent *event);

private :
    void SetBtnIcons() ;
    void SetToolTips() ;

private slots:
    void on_BtnAdd_clicked() ;
    void on_BtnRemove_clicked() ;
    void on_BtnPreviewStop_clicked() ;
    void on_BtnCancel_clicked() ;
    void on_BtnOk_clicked() ;
    void keyPressEvent (  QKeyEvent* e) ;
    void keyReleaseEvent( QKeyEvent* e) ;
    void on_BtnPreviewStart_clicked();
    void on_SongsList_itemDoubleClicked(QListWidgetItem* item) ;

private:
    Ui::MusicMgr*        ui;
    QString              m_szExt ;
    QString              m_szFilters ;
    QString              m_szFolder ;
    QStringList          m_lFilesList ;
    bool                 m_bShiftPressed ;
    QMediaPlayer         m_player ;
    QMediaPlaylist       m_playList ;
};

#endif // MUSICMGR_H
