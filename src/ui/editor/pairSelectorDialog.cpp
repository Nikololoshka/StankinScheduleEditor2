#include "pairSelectorDialog.h"
#include "ui_pairSelectorDialog.h"

#include "pairEditorDialog.h"
#include "../stankinScheduleEditor2.h"

PairSelectorDialog::PairSelectorDialog(const QSharedPointer<Schedule> &schedule,
                                       const ScheduleIndex &index, QWidget *parent) :
      QDialog(parent),
      ui(new Ui::PairSelectorDialog),
      schedule_(schedule),
      index_(index)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle(windowTitle() + " (" + index_.toString() + ")");

    updateList();

    connect(ui->list, &QListWidget::doubleClicked,
            this, &PairSelectorDialog::onItemDoubleClicked);
    connect(ui->addButton, &QPushButton::clicked,
            this, &PairSelectorDialog::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked,
            this, &PairSelectorDialog::onEditButtomClicked);
    connect(ui->removeButton, &QPushButton::clicked,
            this, &PairSelectorDialog::onRemoveButtomClicked);
}

PairSelectorDialog::~PairSelectorDialog()
{
    delete ui;
}

void PairSelectorDialog::onItemDoubleClicked(const QModelIndex &index)
{
    auto pair = std::optional<Pair>(pairs_[index.row()]);
    editPair(pair);
}

void PairSelectorDialog::onAddButtonClicked()
{
    auto pair = std::optional<Pair>();
    editPair(pair);
}

void PairSelectorDialog::onEditButtomClicked()
{
    auto index = ui->list->currentIndex();
    if (!index.isValid() || index.row() < 0) {
        QMessageBox::information(this, "Информация", "Пара не выбрана");
        return;
    }

    onItemDoubleClicked(index);
}

void PairSelectorDialog::onRemoveButtomClicked()
{
    auto index = ui->list->currentIndex();
    int row = index.row();
    if (!index.isValid() || row < 0) {
        QMessageBox::information(this, "Информация", "Пара не выбрана");
        return;
    }

    auto pair = pairs_[row];
    schedule_->remove(pair);
    updateList();
}

void PairSelectorDialog::editPair(std::optional<Pair> &pair)
{
    auto editor = new PairEditorDialog(schedule_, pair, this);
    editor->exec();

    auto& result = editor->result();
    if (result.has_value()) {
        try {
            schedule_->changePair(pair, result);

        } catch (std::exception &e) {
            QMessageBox::critical(this,
                                  "Ошибка сохранения пары",
                                  QString("Ошибка: ") + e.what());
        }
        updateList();
    }
}

void PairSelectorDialog::updateList()
{
    pairs_ = schedule_->pairsByIndex(index_);

    ui->list->clear();
    for (const auto& pair : pairs_) {
        ui->list->addItem(pair.toString());
    }

    auto window = dynamic_cast<StankinScheduleEditor2*>(parent());
    if (window != nullptr) {
        window->updateTable();
    }
}
