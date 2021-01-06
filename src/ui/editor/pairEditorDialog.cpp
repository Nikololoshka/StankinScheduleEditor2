#include "pairEditorDialog.h"
#include "ui_pairEditorDialog.h"

#include "dateEditorDialog.h"

PairEditorDialog::PairEditorDialog(const QSharedPointer<Schedule> &schedule,
                                   const std::optional<Pair> &pair, QWidget *parent) :
      QDialog(parent),
      ui(new Ui::PairEditorDialog),
      schedule_(schedule),
      pair_(pair)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    auto types = Type::list();
    for (auto& type : types) {
        ui->typeComboBox->addItem(type.text(), type.tag());
    }

    auto subgroups = Subgroup::list();
    for (auto& subgroup : subgroups) {
        ui->subgroupComboBox->addItem(subgroup.text(), subgroup.tag());
    }

    auto startTimes = Time_::startTime();
    for (auto& time : startTimes) {
        ui->timeStartComboBox->addItem(time.toString(Time_::PATTERN));
    }

    auto endTimes = Time_::endTime();
    for (auto& time : endTimes) {
        ui->timeEndComboBox->addItem(time.toString(Time_::PATTERN));
    }

    bind();

    connect(ui->timeStartComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PairEditorDialog::onStartTimeChanged);
    connect(ui->dateList, &QListWidget::doubleClicked,
            this, &PairEditorDialog::onDateItemDoubleClicked);
    connect(ui->addButton, &QPushButton::clicked,
            this, &PairEditorDialog::onAddDateClicked);
    connect(ui->editButton, &QPushButton::clicked,
            this, &PairEditorDialog::onEditDateClicked);
    connect(ui->removeButton, &QPushButton::clicked,
            this, &PairEditorDialog::onRemoveDateClicked);
    connect(ui->okButton, &QPushButton::clicked,
            this, &PairEditorDialog::onOkButtonClicked);
}

PairEditorDialog::~PairEditorDialog()
{
    delete ui;
}

std::optional<Pair> &PairEditorDialog::result()
{
    return result_;
}

void PairEditorDialog::onOkButtonClicked()
{
    auto title = ui->titleLineEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::information(this,
                                 "Пустое поле",
                                 "Поле с названием предмета пустое!");
        return;
    }

    if (date_.isEmpty()) {
        QMessageBox::information(this,
                                 "Нет дат",
                                 "У пары должно быть хотя бы одна дата!");
        return;
    }

    auto lecture = ui->lecturerLineEdit->text().trimmed();
    auto classroom = ui->classroomLineEdit->text().trimmed();

    try {
        auto type = Type::fromString(ui->typeComboBox->currentData().toString());
        auto subgroup = Subgroup::fromString(ui->subgroupComboBox->currentData().toString());

        auto startTime = ui->timeStartComboBox->currentText();
        auto endTime = ui->timeEndComboBox->currentText();

        Time_ time(ui->timeStartComboBox->currentText(), ui->timeEndComboBox->currentText());

        auto newPair = std::make_optional<Pair>(title,
                                                lecture,
                                                classroom,
                                                type,
                                                subgroup,
                                                time);
        newPair->setDate(date_);
        schedule_->possibleChange(pair_, newPair);
        result_ = newPair;

        close();

    } catch (std::exception &e) {
        QMessageBox::critical(this,
                              "Ошибка сохранения",
                              QString("Ошибка:") + e.what());
    }
}

void PairEditorDialog::onStartTimeChanged(int index)
{
    auto endTimes = Time_::endTime();
    ui->timeEndComboBox->clear();
    for (int i = index; i < endTimes.size(); ++i) {
        ui->timeEndComboBox->addItem(endTimes[i].toString(Time_::PATTERN));
    }
}

void PairEditorDialog::onDateItemDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    auto item = date_.copy(row);
    editDate(item);
}

void PairEditorDialog::onAddDateClicked()
{
    editDate(std::unique_ptr<DateItem>(nullptr));
}

void PairEditorDialog::onEditDateClicked()
{
    int row = ui->dateList->currentRow();
    if (row >= 0) {
        auto item = date_.copy(row);
        editDate(item);
    }
}

void PairEditorDialog::onRemoveDateClicked()
{
    int row = ui->dateList->currentRow();
    if (row >= 0) {
        auto item = date_.copy(row);
        date_.removeDate(item);
        updateDateList();
    }
}

void PairEditorDialog::editDate(const std::unique_ptr<DateItem> &item)
{
    auto editor = new DateEditorDialog(date_, item, this);
    editor->exec();

    if (editor->isValid()) {
        auto& result = editor->result();

        try {
            date_.changeDate(item, result);
        } catch (std::exception &e) {
            QMessageBox::critical(this,
                                  "Ошибка сохранения даты",
                                  QString("Ошибка: ") + e.what());
        }
        updateDateList();
    }
}

void PairEditorDialog::updateDateList()
{
    ui->dateList->clear();
    for (int i = 0; i < date_.size(); ++i) {
        ui->dateList->addItem(date_[i]->toString());
    }
}

void PairEditorDialog::bind()
{
    if (!pair_.has_value()) {
        return;
    }

    ui->titleLineEdit->setText(pair_->title());
    ui->lecturerLineEdit->setText(pair_->lecturer());
    ui->classroomLineEdit->setText(pair_->classroom());
    ui->typeComboBox->setCurrentText(pair_->type().text());
    ui->subgroupComboBox->setCurrentText(pair_->subgroup().text());
    ui->timeStartComboBox->setCurrentText(pair_->time().start());
    ui->timeEndComboBox->setCurrentText(pair_->time().end());

    date_ = pair_->date();
    updateDateList();
}
