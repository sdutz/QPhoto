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

#include "errordlg.h"
#include "ui_errordlg.h"


//----------------------------------------------------
ErrorDlg::ErrorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDlg)
{
    QSize currSize ;

    ui->setupUi(this);
    ui->LogView->setReadOnly( true);

    setWindowTitle( tr( "Error Dialog"));

    currSize = size() ;
    setMaximumSize( currSize);
    setMinimumSize( currSize);
}

//----------------------------------------------------
ErrorDlg::~ErrorDlg()
{
    delete ui;
}

//----------------------------------------------------
void ErrorDlg::on_ClearBtn_clicked()
{
    ui->LogView->clear();
    m_pConf->ResetLog() ;
}

//----------------------------------------------------
void ErrorDlg::on_CloseBtn_clicked()
{
    DoHide();
}


//----------------------------------------------------
void ErrorDlg::DoHide()
{
    m_pConf->WriteLog( ui->LogView->toPlainText()) ;
    hide() ;
    setVisible( false);
}

//----------------------------------------------------
void ErrorDlg::DoShow( const QRect& cRect, const QString& szLog)
{
    QString szFullLog ;

    szFullLog = m_pConf->GetLog() + "\n" + szLog ;

    ui->LogView->setPlainText( szFullLog);

    move(   cRect.topLeft());
    resize( cRect.width(), cRect.height());

    setVisible( true);
    show() ;
    raise();
    activateWindow();
}

//----------------------------------------------------
bool ErrorDlg::SetMgr( ConfMgr* pConf)
{
    if ( pConf == NULL)
        return false ;

    m_pConf = pConf ;
    return true ;
}
