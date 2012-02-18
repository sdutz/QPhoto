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
#include <QFontDialog>
#include "util.h"
#include "musicmgr.h"

#define MIN_SEC 1
#define MAX_SEC 30

//----------------------------------------------------
SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{
    ui->setupUi(this);
    ui->FadeCmbBox->addItem( tr( "None")) ;
    ui->FadeCmbBox->addItem( tr( "On SlideShow")) ;
    ui->FadeCmbBox->addItem( tr( "Always")) ;

    ui->LangCmbBox->addItem( tr( "English")) ;
    ui->LangCmbBox->addItem( tr( "Italian")) ;

    setWindowTitle( tr( "Settings dialog"));
    setMinimumSize( width(), height()) ;
    setMaximumSize( width(), height()) ;
    InitButton() ;
}

//----------------------------------------------------
SettingsDlg::~SettingsDlg()
{
    delete ui;
}

//----------------------------------------------------
void SettingsDlg::InitButton()
{
    QIcon icon ;
    QSize pixSize ;

    GetPixBtnSize( ui->Font_Btn->size(), &pixSize) ;
    icon.addFile( "icons/font.png", pixSize);
    ui->Font_Btn->setIcon( icon) ;

    GetPixBtnSize( ui->Music_Btn->size(), &pixSize) ;
    icon.addFile( "icons/music.png", pixSize);
    ui->Music_Btn->setIcon( icon) ;

    ui->label_color->setText( tr( "Choose Color")) ;
    ui->label_fade->setText( tr(  "Fade type")) ;
    ui->label_font->setText( tr( "Choose Font")) ;
    ui->label_lang->setText( tr(  "Choose Language")) ;
    ui->Default_Btn->setText( tr( "Default")) ;
}

//----------------------------------------------------
void SettingsDlg::UpdateColorButton()
{
    QString szStyle ;
    QColor  color ;

    color.setNamedColor( m_sets.szColor);
    if ( ! color.isValid())
        color = Qt::black ;

    szStyle = QString( "* { background-color: rgb(%1,%2,%3)}").arg( color.red()).arg( color.green()).arg( color.blue()) ;
    ui->Color_Btn->setStyleSheet( szStyle);
}

//----------------------------------------------------
void SettingsDlg::on_Color_Btn_clicked()
{
    QColorDialog cDlg ;
    QColor       cNewColor ;
    QColor       cOldColor ;

    cOldColor.setNamedColor( m_sets.szColor);
    if( ! cOldColor.isValid())
        cOldColor = Qt::black ;
    cNewColor = cDlg.getColor( cOldColor) ;
    if ( cNewColor.isValid()) {
        m_sets.szColor = cNewColor.name() ;
        UpdateColorButton();
    }
}

//----------------------------------------------------
void SettingsDlg::on_Default_Btn_clicked()
{
    QColor black ;
    QFont  font  ;

    m_sets.szColor   = black.name() ;
    m_sets.szFont    = font.toString() ;
    m_sets.nSec      = 5 ;
    ui->spinSec->setValue( 5);
    m_sets.nFadeType = FADE_NONE ;
    ui->FadeCmbBox->setCurrentIndex( FADE_NONE);
    m_sets.nLang     = ENGLISH ;
    ui->LangCmbBox->setCurrentIndex( ENGLISH);

    UpdateColorButton();
}

//----------------------------------------------------
void SettingsDlg::on_spinSec_valueChanged(int )
{
    m_sets.nSec = ui->spinSec->value() ;
}

//----------------------------------------------------
void SettingsDlg::on_Font_Btn_clicked()
{
    bool         bOk ;
    QFontDialog  cDlg ;
    QFont        NewFont ;
    QFont        OldFont ;

    OldFont.fromString( m_sets.szFont) ;
    NewFont = cDlg.getFont( &bOk, OldFont) ;
    if( bOk)
        m_sets.szFont = NewFont.toString() ;
}

//----------------------------------------------------
void SettingsDlg::SetInitSettings( const QPhotoSettings& sets)
{
    m_sets = sets ;
    ui->spinSec->setValue(   m_sets.nSec);
    ui->spinSec->setMinimum( MIN_SEC);
    ui->spinSec->setMaximum( MAX_SEC);

    ui->FadeCmbBox->setCurrentIndex( m_sets.nFadeType);
    ui->LangCmbBox->setCurrentIndex( m_sets.nLang);

    UpdateColorButton();
}

//---------------------------------------------------
void SettingsDlg::on_FadeCmbBox_currentIndexChanged(int index)
{
    m_sets.nFadeType = index ;
}

//---------------------------------------------------
void SettingsDlg::on_Music_Btn_clicked()
{
    MusicMgr mmgr ;

    mmgr.SetInitList( m_sets.szSongs) ;

    if ( mmgr.exec() == QDialog::Accepted)
        mmgr.GetList( &m_sets.szSongs);
}

//---------------------------------------------------
void SettingsDlg::on_LangCmbBox_currentIndexChanged(int index)
{
    m_sets.nLang = index ;
}
