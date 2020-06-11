#include "itemsetdialog.h"
#include "ui_itemsetdialog.h"

ItemsetDialog::ItemsetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemsetDialog)
{
    ui->setupUi(this);
}

ItemsetDialog::ItemsetDialog(QWidget *parent, QStringList l1, QStringList l2) :
    QDialog(parent), FIS(l1), IIS(l2),
    ui(new Ui::ItemsetDialog)
{
    ui->setupUi(this);
    FISlistModel = new QStringListModel(this);
    FISlistModel->setStringList(FIS);
    IISlistModel = new QStringListModel(this);
    IISlistModel->setStringList(IIS);
    ui->listView->setModel(FISlistModel);
    ui->listView_2->setModel(IISlistModel);
}

ItemsetDialog::~ItemsetDialog()
{
    delete FISlistModel;
    delete IISlistModel;
    delete ui;
}
