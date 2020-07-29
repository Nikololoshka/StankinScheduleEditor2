#ifndef PAIREDITORDIALOG_H
#define PAIREDITORDIALOG_H

#include <QtWidgets>
#include "schedule.h"


namespace Ui {
class PairEditorDialog;
}

class PairEditorDialog : public QDialog
{
    Q_OBJECT

public:
    PairEditorDialog(const QSharedPointer<Schedule> &schedule,
                     const Pair &pair, QWidget *parent = nullptr);
    ~PairEditorDialog();

public slots:
    void onOkButtonClicked();
    void onStartTimeChanged(int index);
    void onDateItemDoubleClicked(const QModelIndex &index);
    void onAddDateClicked();
    void onEditDateClicked();
    void onRemoveDateClicked();

private:
    void editDate(const std::unique_ptr<DateItem> &item);
    void updateDateList();

private:
    Ui::PairEditorDialog *ui;

    QSharedPointer<Schedule> schedule_;
    Pair pair_;
    Date date_;

    std::unique_ptr<Pair> result_;
};

#endif // PAIREDITORDIALOG_H
