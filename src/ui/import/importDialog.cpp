#include "importDialog.h"
#include "ui_importDialog.h"


ImportDialog::ImportDialog(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::ImportDialog)
{
    ui->setupUi(this);

    try {


    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

ImportDialog::~ImportDialog()
{
    delete ui;
}
