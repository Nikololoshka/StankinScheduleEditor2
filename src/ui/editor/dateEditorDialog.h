#ifndef DATEEDITORDIALOG_H
#define DATEEDITORDIALOG_H

#include "stdafx.h"
#include "schedule.h"

namespace Ui {
class DateEditorDialog;
}

class DateEditorDialog : public QDialog
{
    // Q_OBJECT

public:
    DateEditorDialog(const Date &date, const std::unique_ptr<DateItem> &item,
                     QWidget *parent = nullptr);
    ~DateEditorDialog();

    bool isValid() const;
    std::unique_ptr<DateItem>& result();

public slots:
    void onRangeModeChecked(bool checked);
    void onStartDateChanged(const QDate &date);
    void onFrequencyChanged(int index);
    void onOkButtonClicked();

private:
    void bindDate();
    int delta() const;
    Frequency frequency() const;
    bool checkDate();

private:
    Ui::DateEditorDialog *ui;

    Date date_;
    std::unique_ptr<DateItem> item_;

    std::unique_ptr<DateItem> result_;
};

#endif // DATEEDITORDIALOG_H
