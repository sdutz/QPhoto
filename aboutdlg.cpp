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

#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include <QLibraryInfo>

#define  VERSION  "1.0"

//----------------------------------------------------
AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this) ;
    setWindowTitle( "QPhoto about");

    FillSysInfo() ;
}

//----------------------------------------------------
AboutDlg::~AboutDlg()
{
    delete ui;
}

//----------------------------------------------------
void AboutDlg::on_buttonBox_accepted()
{
    close();
}

//----------------------------------------------------
void AboutDlg::FillSysInfo()
{
    ui->textEdit->setText( QString( "QPhoto version %1 by Sdutz\n using %2").arg( VERSION).arg( QLibraryInfo::buildKey())) ;
}
