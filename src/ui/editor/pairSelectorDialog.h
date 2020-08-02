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
    PairSelectorDialog(const QSharedPointer<Schedule> &schedule,
                       const ScheduleIndex &index, QWidget *parent = nullptr);
    ~PairSelectorDialog();

public slots:
    void onItemDoubleClicked(const QModelIndex &index);
    void onAddButtonClicked();
    void onEditButtomClicked();
    void onRemoveButtomClicked();

private:
    void editPair(std::optional<Pair> &pair);
    void updateList();

private:
    Ui::PairSelectorDialog *ui;

    QSharedPointer<Schedule> schedule_;
    ScheduleIndex index_;
    QVector<Pair> pairs_;
};

#endif // PAIRSELECTORDIALOG_H
