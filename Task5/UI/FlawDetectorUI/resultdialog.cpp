#include "resultdialog.h"
#include "ui_resultdialog.h"


ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
}

void ResultDialog::on_current_column_changed(const QModelIndex& current, const QModelIndex& previous) {
    QString curLine = current.model()->data(current).toString();
    if(curLine.isEmpty()) return;
//    qDebug() << curLine;
    for(auto i: curLine) {
        if(!i.isDigit()) return;
    }
//    qDebug() << curLine.toInt();
    code->setFocus();
    QTextCursor tc = code->textCursor();
    if(previousSelectedLine != -1) {
        int position = code->document()->findBlockByNumber ( previousSelectedLine - 1 ).position();
        tc.setPosition(position,QTextCursor::KeepAnchor);
        tc.select(QTextCursor::LineUnderCursor);
        QTextCharFormat fmt;
        fmt.setForeground(QColor(Qt::black));
        fmt.setFontWeight(QFont::Normal);
        tc.mergeCharFormat(fmt);
        tc.clearSelection();
    }
    gotoline(curLine.toInt());
}


void ResultDialog::gotoline(int line)
{
    QTextCursor tc = code->textCursor();
    int position = code->document()->findBlockByNumber ( line-1 ).position();
    tc.setPosition(position,QTextCursor::MoveAnchor);
    code->setTextCursor(tc);
    previousSelectedLine = line;
    tc.select(QTextCursor::LineUnderCursor);
    QTextCharFormat fmt;
    fmt.setForeground(QColor(Qt::red));
    fmt.setFontWeight(QFont::Bold);
    tc.mergeCharFormat(fmt);
    tc.clearSelection();
}

ResultDialog::ResultDialog(QWidget *parent, QStringList ls, QString filePath) :
    QDialog(parent), result(ls), filep(filePath), ui(new Ui::ResultDialog)
{
    previousSelectedLine = -1;
    ui->setupUi(this);
    QFile fi(filep);
    if(!fi.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    code = new QTextEdit;
//    code->setReadOnly(true);
    ui->scrollArea->setWidget(code);
    int curlinenum = 0;
    while(!fi.atEnd()) {
        ++curlinenum;
        QByteArray ba = fi.readLine();
        QString linecont = ba,linenum = QString("%1| ").arg(curlinenum,3,10,QChar(' '));
        if(linecont.endsWith('\n') | linecont.endsWith('\r'))
            linecont = linecont.mid(0, linecont.size() - 1);
        QString cont = linenum + linecont;
        code->append(cont);
    }
    QStandardItemModel *modelRES = new QStandardItemModel(ui->treeView);
    QItemSelectionModel *modelSEL = ui->treeView->selectionModel();
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
    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,  SLOT(on_current_column_changed(const QModelIndex&, const QModelIndex&)));
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->expandAll();
    ui->treeView->setColumnWidth(0, 175);
    ui->treeView->setColumnWidth(1, 450);
}


ResultDialog::~ResultDialog()
{
    delete code;
    delete modelRES;
    delete ui;
}
