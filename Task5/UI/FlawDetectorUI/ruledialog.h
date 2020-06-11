#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class RuleDialog;
}

class RuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuleDialog(QWidget *parent = nullptr);
    explicit RuleDialog(QWidget *parent, QStringList l1, QStringList l2);
    ~RuleDialog();

private:
    Ui::RuleDialog *ui;
    QStringList PAR, NAR;
    QStringListModel *modelPAR, *modelNAR;
};

#endif // RULEDIALOG_H
