#ifndef PAIRSELECTORDIALOG_H
#define PAIRSELECTORDIALOG_H

#include <QtWidgets>
#include "schedule.h"

namespace Ui {
class PairSelectorDialog;
}

class PairSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PairSelectorDialog(const QSharedPointer<Schedule> &schedule,
                                const ScheduleIndex &index, QWidget *parent = nullptr);
    ~PairSelectorDialog();

public slots:
    void onItemClicked(const QModelIndex &index);
    void onEditBottomClicked();

private:
    Ui::PairSelectorDialog *ui;

    QSharedPointer<Schedule> schedule_;
    ScheduleIndex index_;
    QVector<Pair> pairs_;
};

#endif // PAIRSELECTORDIALOG_H
