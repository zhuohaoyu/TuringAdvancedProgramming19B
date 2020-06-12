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
    QStandardItemModel *modelRES = new QStandardItemModel(ui->treeView);
    modelRES->setHorizontalHeaderLabels(QStringList()<< "Triggered Rules" << "Detail" << "Line");
    int errIndx = 0;
    for(auto i:result) {
        ++errIndx;

        QStringList sp0 = i.split('|');
        QString conf = sp0[0].mid(2);
        QString cs = sp0[1];

        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem("Error " + QString::number(errIndx) + " ("+ conf +"%)");
//        QStandardItem* item2 = new QStandardItem("");
        items1.append(item1);
        modelRES->appendRow(items1);

        if(i.startsWith("$X")) {//Triggered PAR
            QList<QStandardItem*> r1items;
            QStandardItem* r1item1 = new QStandardItem("Found");
//            QStandardItem* r1item2 = new QStandardItem("");
            r1items.append(r1item1);
            item1->appendRow(r1items);
            QStringList sp = cs.split('#');
            for(int jj = 0; jj < sp.size(); ++jj) {
                QString j = sp[jj];
                if(jj != sp.size() - 1) {
                    QStringList nmsplit = j.split('&');
                    QString inst = nmsplit[0], loc = nmsplit[1];
                    //instrction = inst, location = loc
                    QList<QStandardItem*> items2;
                    QStandardItem* jitem1 = new QStandardItem("");
                    QStandardItem* jitem2 = new QStandardItem(inst);
                    QStandardItem* jitem3 = new QStandardItem(loc);
                    items2.append(jitem1), items2.append(jitem2), items2.append(jitem3);
                    r1item1->appendRow(items2);
                }
                else {
                    //instruction = j, no location.
                    QList<QStandardItem*> r2items;
                    QStandardItem* r2item1 = new QStandardItem("Missing");
//                    QStandardItem* r2item2 = new QStandardItem("");
                    r2items.append(r2item1);
                    item1->appendRow(r2items);
                    QList<QStandardItem*> items2;
                    QStandardItem* jitem1 = new QStandardItem("");
                    QStandardItem* jitem2 = new QStandardItem(j);
                    items2.append(jitem1), items2.append(jitem2);
                    r2item1->appendRow(items2);
                }
            }
        }
        else {//Triggered NAR
            QList<QStandardItem*> r1items;
            QStandardItem* r1item1 = new QStandardItem("Found");
//            QStandardItem* r1item2 = new QStandardItem("");
            r1items.append(r1item1);
            item1->appendRow(r1items);
            QStringList sp = cs.split('#');
            for(int jj = 0; jj < sp.size() - 1; ++jj) {
                QString j = sp[jj];
                QStringList nmsplit = j.split('&');
                QString inst = nmsplit[0], loc = nmsplit[1];
                QList<QStandardItem*> items2;
                QStandardItem* jitem1 = new QStandardItem("");
                QStandardItem* jitem2 = new QStandardItem(inst);
                QStandardItem* jitem3 = new QStandardItem(loc);
                items2.append(jitem1), items2.append(jitem2), items2.append(jitem3);
                r1item1->appendRow(items2);
            }
            QStringList spb = sp[sp.size() - 1].split('$');
            QList<QStandardItem*> r2items;
            QStandardItem* r2item1 = new QStandardItem("Unexpected");
//            QStandardItem* r2item2 = new QStandardItem("");
            r2items.append(r2item1);
            item1->appendRow(r2items);
            for(int jj = 0; jj < spb.size() - 1; ++jj) {
                QString j = spb[jj];
                QStringList nmsplit = j.split('&');
                QString inst = nmsplit[0], loc = nmsplit[1];
                QList<QStandardItem*> items2;
                QStandardItem* jitem1 = new QStandardItem("");
                QStandardItem* jitem2 = new QStandardItem(inst);
                QStandardItem* jitem3 = new QStandardItem(loc);
                items2.append(jitem1), items2.append(jitem2), items2.append(jitem3);
                r2item1->appendRow(items2);
            }
        }
    }
    ui->treeView->setModel(modelRES);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->expandAll();
    ui->treeView->setColumnWidth(0, 175);
    ui->treeView->setColumnWidth(1, 450);
}


ResultDialog::~ResultDialog()
{
    delete modelRES;
    delete ui;
}
