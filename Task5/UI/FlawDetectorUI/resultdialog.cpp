#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
}
ResultDialog::ResultDialog(QWidget *parent, QStringList ls) :
    QDialog(parent), result(ls), ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    out = new QTextEdit();
    out->setReadOnly(true);
    ui->scrollArea->setWidget(out);
    ui->scrollArea->setWidgetResizable(true);
    for(auto i: result) out->append(i);
}


ResultDialog::~ResultDialog()
{
    delete out;
    delete ui;
}
