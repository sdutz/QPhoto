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


//----------------------------------------------------
AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);

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
    QString  szBuff ;

    szBuff = "QPhoto version 0.1 by Sdutz" ;

    ui->textEdit->setText( szBuff);
}
