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

#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include "confmgr.h"

struct QPhotoSettings {
    int     nSec ;
    int     nFadeType ;
    int     nLang ;
    QString szFont ;
    QString szColor ;
    QString szSongs ;
};

namespace Ui {
    class SettingsDlg;
}

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget *parent = 0);
    ~SettingsDlg();
    void SetInitSettings( const QPhotoSettings& sets) ;
    QPhotoSettings GetSettings( void) { return m_sets ; } ;

private :
    void UpdateColorButton() ;
    void InitButton() ;

private slots:

    void on_Color_Btn_clicked();

    void on_Default_Btn_clicked();

    void on_spinSec_valueChanged(int );

    void on_Font_Btn_clicked();

    void on_FadeCmbBox_currentIndexChanged(int index);

    void on_Music_Btn_clicked();

    void on_LangCmbBox_currentIndexChanged(int index);

private:
    Ui::SettingsDlg *ui;
    QPhotoSettings  m_sets ;
};

#endif // SETTINGSDLG_H
