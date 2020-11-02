#ifndef TRANSITIONDIALOG_H
#define TRANSITIONDIALOG_H

#include "stdafx.h"
#include "parseWorkerManager.h"

namespace Ui {
class TransitionDialog;
}

class TransitionDialog : public QDialog
{
    // Q_OBJECT

public:
    TransitionDialog(const QSharedPointer<ParseWorkerManager> &workerManager,
                     QWidget *parent = nullptr);
    ~TransitionDialog();

public slots:
    void onOkButtonClicked();
    void onAddButtonClicked();
    void onEditButtoClicked();
    void onRemoveButtonClicked();

private:
    QMap<QString, QString> tableValues() const;

private:
    Ui::TransitionDialog *ui;

    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // TRANSITIONDIALOG_H
