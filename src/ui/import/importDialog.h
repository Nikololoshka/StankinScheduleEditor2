#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QtWidgets>


namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = nullptr);
    ~ImportDialog();

private:
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
