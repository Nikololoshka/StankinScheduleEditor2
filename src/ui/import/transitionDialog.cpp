#include "transitionDialog.h"
#include "ui_transitionDialog.h"

TransitionDialog::TransitionDialog(const QSharedPointer<ParseWorkerManager> &workerManager, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::TransitionDialog),
      workerManager_(workerManager)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    const auto& transitions = workerManager_->transitions();
    ui->tableWidget->setRowCount(transitions.size());

    int i = 0;
    for (auto it = transitions.begin(); it != transitions.end(); ++it, ++i) {
        auto itemSource = new QTableWidgetItem(it.key());
        auto itemTransition = new QTableWidgetItem(it.value());
        ui->tableWidget->setItem(i, 0, itemSource);
        ui->tableWidget->setItem(i, 1, itemTransition);
    }
    ui->tableWidget->sortItems(0);

    connect(ui->okButton, &QPushButton::clicked,
            this, &TransitionDialog::onOkButtonClicked);
    connect(ui->addButton, &QPushButton::clicked,
            this, &TransitionDialog::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked,
            this, &TransitionDialog::onEditButtoClicked);
    connect(ui->removeButton, &QPushButton::clicked,
            this, &TransitionDialog::onRemoveButtonClicked);
}

TransitionDialog::~TransitionDialog()
{
    delete ui;
}

void TransitionDialog::onOkButtonClicked()
{
    auto values = tableValues();
    workerManager_->updateTransition(values);
    close();
}

void TransitionDialog::onAddButtonClicked()
{
    ui->tableWidget->scrollToTop();
    ui->tableWidget->insertRow(0);

    auto itemSource = new QTableWidgetItem();
    auto itemTransition = new QTableWidgetItem();
    ui->tableWidget->setItem(0, 0, itemSource);
    ui->tableWidget->setItem(0, 1, itemTransition);

    ui->tableWidget->editItem(itemSource);
}

void TransitionDialog::onEditButtoClicked()
{
    auto item = ui->tableWidget->currentItem();
    if (item != nullptr) {
        ui->tableWidget->editItem(item);
    }
}

void TransitionDialog::onRemoveButtonClicked()
{
    int row = ui->tableWidget->currentRow();
    if (row >= 0) {
        ui->tableWidget->removeRow(row);
    }
}

QMap<QString, QString> TransitionDialog::tableValues() const
{
    QMap<QString, QString> values;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        auto source = ui->tableWidget->item(i, 0)->text();
        auto transition = ui->tableWidget->item(i, 1)->text();
        if (!source.isEmpty() && !transition.isEmpty()) {
            values[source] = transition;
        }
    }
    return values;
}
