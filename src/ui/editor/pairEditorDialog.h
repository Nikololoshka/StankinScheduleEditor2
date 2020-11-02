#ifndef PAIREDITORDIALOG_H
#define PAIREDITORDIALOG_H

#include "stdafx.h"
#include "schedule.h"

namespace Ui {
class PairEditorDialog;
}

class PairEditorDialog : public QDialog
{
    // Q_OBJECT

public:
    PairEditorDialog(const QSharedPointer<Schedule> &schedule,
                     const std::optional<Pair> &pair, QWidget *parent = nullptr);
    ~PairEditorDialog();

    std::optional<Pair>& result();

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
    void bind();

private:
    Ui::PairEditorDialog *ui;

    QSharedPointer<Schedule> schedule_;
    std::optional<Pair> pair_;
    Date date_;

    std::optional<Pair> result_;
};

#endif // PAIREDITORDIALOG_H
