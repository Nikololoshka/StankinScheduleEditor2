#include "pairSelectorDialog.h"
#include "ui_pairSelectorDialog.h"

#include "pairEditorDialog.h"

PairSelectorDialog::PairSelectorDialog(const QSharedPointer<Schedule> &schedule,
                                       const ScheduleIndex &index, QWidget *parent) :
      QDialog(parent),
      ui(new Ui::PairSelectorDialog),
      schedule_(schedule),
      index_(index)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowTitle(windowTitle() + " (" + index_.toString() + ")");

    pairs_ = schedule_->pairsByIndex(index_);
    for (auto& pair : pairs_) {
        ui->list->addItem(pair.toString());
    }

    connect(ui->list, &QListWidget::doubleClicked,
            this, &PairSelectorDialog::onItemClicked);
    connect(ui->editButton, &QPushButton::clicked,
            this, &PairSelectorDialog::onEditBottomClicked);
}

PairSelectorDialog::~PairSelectorDialog()
{
    delete ui;
}

void PairSelectorDialog::onItemClicked(const QModelIndex &index)
{
    auto pair = pairs_[index.row()];
    auto editor = new PairEditorDialog(schedule_, pair, this);
    editor->show();
}

void PairSelectorDialog::onEditBottomClicked()
{
    auto index = ui->list->currentIndex();
    if (!index.isValid() || index.row() < 0) {
        QMessageBox::information(this, "Информация", "Пара не выбрана");
        return;
    }

    onItemClicked(index);
}
