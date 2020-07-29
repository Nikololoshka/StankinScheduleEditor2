#include "dateEditorDialog.h"
#include "ui_dateEditorDialog.h"

#include "pair/dateSingle.h"
#include "pair/dateRange.h"

DateEditorDialog::DateEditorDialog(const Date &date, const std::unique_ptr<DateItem> &item,
                                   QWidget *parent) :
      QDialog(parent),
      ui(new Ui::DateEditorDialog),
      date_(date),
      item_(item.get() != nullptr ? item->copy() : nullptr),
      result_(nullptr)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);

    auto frequencies = Frequency::list();
    for (auto& frequency : frequencies) {
        ui->frequencyComboBox->addItem(frequency.text(), frequency.tag());
    }

    ui->dateEdit->setDate(QDate::currentDate());
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate().addDays(14));

    bindDate();

    connect(ui->rangeModeCheckBox, &QCheckBox::clicked,
            this, &DateEditorDialog::onRangeModeChecked);
    connect(ui->dateEdit, &QDateEdit::dateChanged,
            this, &DateEditorDialog::onStartDateChanged);
    connect(ui->frequencyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DateEditorDialog::onFrequencyChanged);
    connect(ui->okButton, &QPushButton::clicked,
            this, &DateEditorDialog::onOkButtonClicked);
}

DateEditorDialog::~DateEditorDialog()
{
    delete ui;
}

bool DateEditorDialog::isValid() const
{
    return result_.get() != nullptr;
}

std::unique_ptr<DateItem>& DateEditorDialog::result()
{
    return result_;
}

void DateEditorDialog::onRangeModeChecked(bool checked)
{
    bool showRange = checked;
    bool showSingle = !checked;

    ui->dateLabel->setVisible(showSingle);
    ui->dateEdit->setVisible(showSingle);

    ui->startLabel->setVisible(showRange);
    ui->startDateEdit->setVisible(showRange);
    ui->endLabel->setVisible(showRange);
    ui->endDateEdit->setVisible(showRange);
    ui->frequencyLabel->setVisible(showRange);
    ui->frequencyComboBox->setVisible(showRange);
}

void DateEditorDialog::onStartDateChanged(const QDate &date)
{
    ui->endDateEdit->setDateRange(date.addDays(delta()), date.addYears(1));
}

void DateEditorDialog::onFrequencyChanged(int /* index */)
{
    ui->endDateEdit->setMinimumDate(
        ui->startDateEdit->date().addDays(delta())
    );
}

void DateEditorDialog::onOkButtonClicked()
{
    if (checkDate()) {
        close();
    }
}

void DateEditorDialog::bindDate()
{
    if (item_.get() == nullptr) {
        onRangeModeChecked(false);
        return;
    }

    auto frequency = item_->frequency();
    if (frequency == Frequency::getOnce()) {
        auto single = dynamic_cast<DateSingle*>(item_.get());
        if (single != nullptr) {
            ui->dateEdit->setDate(single->date());
        }

        onRangeModeChecked(false);
    } else {
        auto range = dynamic_cast<DateRange*>(item_.get());
        if (range != nullptr) {
            ui->startDateEdit->setDate(range->start());
            ui->endDateEdit->setDate(range->end());
        }

        ui->rangeModeCheckBox->setChecked(true);
        ui->frequencyComboBox->setCurrentText(frequency.text());
        onRangeModeChecked(true);
    }
}

int DateEditorDialog::delta() const
{
    return frequency().delta();
}

Frequency DateEditorDialog::frequency() const
{
    return Frequency::fromString(ui->frequencyComboBox->currentData().toString());
}

bool DateEditorDialog::checkDate()
{
    bool isRangeMode = ui->rangeModeCheckBox->isChecked();

    try {
        if (isRangeMode) {
            result_ = std::make_unique<DateRange>(ui->startDateEdit->date(),
                                                  ui->endDateEdit->date(),
                                                  frequency());
        } else {
            result_ = std::make_unique<DateSingle>(ui->dateEdit->date());
        }
        date_.possibleChange(item_, result_);

        return true;

    } catch (std::exception &e) {
        QMessageBox::critical(this,
                              "Ошибка сохранения даты",
                              QString("Ошибка: ") + e.what());
        result_.reset(nullptr);
    }
    return false;
}
