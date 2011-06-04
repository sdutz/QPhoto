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

    szBuff = "sdutz power" ;

    ui->textEdit->setText( szBuff);
}
