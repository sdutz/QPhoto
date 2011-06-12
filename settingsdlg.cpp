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

#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include <QColorDialog>

#define MIN_SEC 1
#define MAX_SEC 30

//----------------------------------------------------
SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{

    m_Color = Qt::black ;
    ui->setupUi(this);
}


//----------------------------------------------------
SettingsDlg::~SettingsDlg()
{
    delete ui;
}

//----------------------------------------------------
void SettingsDlg::UpdateColorButton()
{
    QString szStyle ;

    szStyle = QString( "* { background-color: rgb(%1,%2,%3)}").arg( m_Color.red()).arg(m_Color.green()).arg(m_Color.blue()) ;
    ui->Color_Btn->setStyleSheet( szStyle);
}


//----------------------------------------------------
void SettingsDlg::on_Color_Btn_clicked()
{
    QColorDialog cDlg ;
    QColor       cTmp ;

    cTmp = cDlg.getColor( m_Color) ;
    if ( cTmp.isValid()) {
        m_Color = cTmp ;
        UpdateColorButton();
    }
}

//----------------------------------------------------
void SettingsDlg::on_Default_Btn_clicked()
{
    m_Color = Qt::black ;
}

//----------------------------------------------------
void SettingsDlg::SetInitColor( const QColor& color)
{
    if ( color.isValid())
        m_Color = color ;
    UpdateColorButton();
}

//----------------------------------------------------
void SettingsDlg::SetInitSeconds( int nSec)
{
    ui->spinSec->setValue(   nSec);
    ui->spinSec->setMinimum( MIN_SEC);
    ui->spinSec->setMaximum( MAX_SEC);
}

//----------------------------------------------------
void SettingsDlg::on_spinSec_valueChanged(int )
{
    m_nSeconds = ui->spinSec->value() ;
}
