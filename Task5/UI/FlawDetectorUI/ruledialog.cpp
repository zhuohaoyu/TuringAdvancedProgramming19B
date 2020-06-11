#include "ruledialog.h"
#include "ui_ruledialog.h"

RuleDialog::RuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleDialog)
{
    ui->setupUi(this);
}
RuleDialog::RuleDialog(QWidget *parent, QStringList l1, QStringList l2) :
    QDialog(parent), PAR(l1), NAR(l2),
    ui(new Ui::RuleDialog)
{
    ui->setupUi(this);
    modelPAR = new QStringListModel(this);
    modelPAR->setStringList(PAR);
    modelNAR = new QStringListModel(this);
    modelNAR->setStringList(NAR);
    ui->listView->setModel(modelPAR);
    ui->listView_2->setModel(modelNAR);
}
RuleDialog::~RuleDialog()
{
    delete modelNAR;
    delete modelPAR;
    delete ui;
}
