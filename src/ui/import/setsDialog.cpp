#include "setsDialog.h"
#include "ui_setsDialog.h"

SetsDialog::SetsDialog(const QSharedPointer<ParseWorkerManager> &workerManager, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SetsDialog),
      workerManager_(workerManager)
{
    ui->setupUi(this);

    updateTable(workerManager_->titles(), ui->titleTableWidget);
    updateTable(workerManager_->lecturers(), ui->lecturerTableWidget);
    updateTable(workerManager_->classrooms(), ui->classroomTableWidget);

    connect(ui->okButton, &QPushButton::clicked,
            this, &SetsDialog::onOkButtonClicked);
    connect(ui->addButton, &QPushButton::clicked,
            this, &SetsDialog::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked,
            this, &SetsDialog::onEditButtoClicked);
    connect(ui->removeButton, &QPushButton::clicked,
            this, &SetsDialog::onRemoveButtonClicked);
}

SetsDialog::~SetsDialog()
{
    delete ui;
}

void SetsDialog::onOkButtonClicked()
{
    auto titles = tableValues(ui->titleTableWidget);
    auto lecturers = tableValues(ui->lecturerTableWidget);
    auto classrooms = tableValues(ui->classroomTableWidget);

    workerManager_->updateSets(titles, lecturers, classrooms);
    close();
}

void SetsDialog::onAddButtonClicked()
{
    auto table = currectTable();
    table->scrollToTop();
    table->insertRow(0);

    auto item = new QTableWidgetItem();
    table->setItem(0, 0, item);
    table->editItem(item);
}

void SetsDialog::onEditButtoClicked()
{
    auto table = currectTable();
    auto item = table->currentItem();
    if (item != nullptr) {
        table->editItem(item);
    }
}

void SetsDialog::onRemoveButtonClicked()
{
    auto table = currectTable();
    int row = table->currentRow();
    if (row >= 0) {
        table->removeRow(row);
    }
}

void SetsDialog::updateTable(const QVector<QString> &list, QTableWidget *table)
{
    table->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        auto item = new QTableWidgetItem(list[i]);
        table->setItem(i, 0, item);
    }
}

QTableWidget *SetsDialog::currectTable()
{
    int index = ui->tabWidget->currentIndex();
    switch (index) {
        case 0: return ui->titleTableWidget;
        case 1: return ui->lecturerTableWidget;
        case 2: return ui->classroomTableWidget;
    }

    throw std::logic_error("Unknown index: " + QString::number(index).toStdString());
}

QVector<QString> SetsDialog::tableValues(QTableWidget *table) const
{
    QVector<QString> values;
    for (int i = 0; i < table->rowCount(); ++i) {
        auto text = table->item(i, 0)->text();
        if (!text.isEmpty()) {
            values.append(text);
        }
    }
    return values;
}
