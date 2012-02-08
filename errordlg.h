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

#ifndef ERRORDLG_H
#define ERRORDLG_H

#include <QDialog>
#include "confmgr.h"

namespace Ui {
    class ErrorDlg;
}

class ErrorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDlg(QWidget *parent = 0);
    void     DoShow( const QRect& cRect, const QString& szLog = "") ;
    void     DoHide() ;
    bool     SetMgr( ConfMgr* pConf) ;

    ~ErrorDlg();

private slots:
    void on_ClearBtn_clicked();

    void on_CloseBtn_clicked();



private:
    Ui::ErrorDlg *ui;
    ConfMgr*     m_pConf ;
};

#endif // ERRORDLG_H
