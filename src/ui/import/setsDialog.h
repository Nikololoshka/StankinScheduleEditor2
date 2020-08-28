#ifndef SETSDIALOG_H
#define SETSDIALOG_H

#include <QtWidgets>
#include "parseWorkerManager.h"

namespace Ui {
class SetsDialog;
}

class SetsDialog : public QDialog
{
    Q_OBJECT

public:
    SetsDialog(const QSharedPointer<ParseWorkerManager> &workerManager, QWidget *parent = nullptr);
    ~SetsDialog();

public slots:
    void onOkButtonClicked();
    void onAddButtonClicked();
    void onEditButtoClicked();
    void onRemoveButtonClicked();

private:
    void updateTable(const QVector<QString> &list, QTableWidget *table);
    QTableWidget* currectTable();
    QVector<QString> tableValues(QTableWidget *table) const;

private:
    Ui::SetsDialog *ui;

    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // SETSDIALOG_H
