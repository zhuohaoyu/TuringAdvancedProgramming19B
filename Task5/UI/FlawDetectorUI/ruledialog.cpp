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
    modelPAR = new QStandardItemModel(ui->treeView);
    modelNAR = new QStandardItemModel(ui->treeView_2);
    modelPAR->setHorizontalHeaderLabels(QStringList()<<"Rules"<< "Content" );
    modelNAR->setHorizontalHeaderLabels(QStringList()<<"Rules"<< "Content");
    int indx = 0;
    for(auto i: PAR) {
        ++indx;
        QStringList split1 = i.split('|');
        QString confVal = split1[1];
        QStringList splits = split1[0].split('^');
        QString b = splits[1];
        QStringList split2 = splits[0].split('#');
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem("Rule " + QString::number(indx) + "("+confVal+")");
        QStandardItem* item2 = new QStandardItem("");
        items1.append(item1), items1.append(item2);
        modelPAR->appendRow(items1);
        QList<QStandardItem*> r1items;
        QStandardItem* r1item1 = new QStandardItem("If Contain");
        QStandardItem* r1item2 = new QStandardItem("");
        r1items.append(r1item1), r1items.append(r1item2);
        item1->appendRow(r1items);
        int itemIndx = 0;
        for(auto j: split2) {
            ++itemIndx;
            QList<QStandardItem*> items2;
            QStandardItem* jitem1 = new QStandardItem("");
            QStandardItem* jitem2 = new QStandardItem(j);
            items2.append(jitem1), items2.append(jitem2);
            r1item1->appendRow(items2);
        }
        QList<QStandardItem*> r2items;
        QStandardItem* r2item1 = new QStandardItem("Then Contain");
        QStandardItem* r2item2 = new QStandardItem("");
        r2items.append(r2item1), r2items.append(r2item2);
        item1->appendRow(r2items);

        QList<QStandardItem*> items2;
        QStandardItem* jitem1 = new QStandardItem("");
        QStandardItem* jitem2 = new QStandardItem(b);
        items2.append(jitem1), items2.append(jitem2);
        r2item1->appendRow(items2);

    }

    indx = 0;
    for(auto i: NAR) {
        ++indx;
        QStringList split1 = i.split('|');
        QString confVal = split1[1];
        QStringList splits = split1[0].split('^');
        QString b = splits[1];
        QStringList split2 = splits[0].split('#');
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem("Rule " + QString::number(indx) + "("+confVal+")");
        QStandardItem* item2 = new QStandardItem("");
        items1.append(item1), items1.append(item2);
        modelNAR->appendRow(items1);
        QList<QStandardItem*> r1items;
        QStandardItem* r1item1 = new QStandardItem("If Contain");
        QStandardItem* r1item2 = new QStandardItem("");
        r1items.append(r1item1), r1items.append(r1item2);
        item1->appendRow(r1items);
        int itemIndx = 0;
        for(auto j: split2) {
            ++itemIndx;
            QList<QStandardItem*> items2;
            QStandardItem* jitem1 = new QStandardItem("");
            QStandardItem* jitem2 = new QStandardItem(j);
            items2.append(jitem1), items2.append(jitem2);
            r1item1->appendRow(items2);
        }
        QList<QStandardItem*> r2items;
        QStandardItem* r2item1 = new QStandardItem("Then NOT Contain");
        QStandardItem* r2item2 = new QStandardItem("");
        r2items.append(r2item1), r2items.append(r2item2);
        item1->appendRow(r2items);

        QList<QStandardItem*> items2;
        QStandardItem* jitem1 = new QStandardItem("");
        QStandardItem* jitem2 = new QStandardItem(b);
        items2.append(jitem1), items2.append(jitem2);
        r2item1->appendRow(items2);
    }

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setModel(modelPAR);
    ui->treeView->expandAll();
    ui->treeView->setColumnWidth(0, 240);

    ui->treeView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView_2->setModel(modelNAR);
    ui->treeView_2->expandAll();
    ui->treeView_2->setColumnWidth(0, 240);
}
RuleDialog::~RuleDialog()
{
    delete modelNAR;
    delete modelPAR;
    delete ui;
}
