#include "dateSingle.h"
#include "dateRange.h"

DateSingle::DateSingle(const QJsonObject &item)
    : DateItem(Frequency::getOnce())
{
    date_ = QDate::fromString(item["date"].toString(), DateItem::FULL_FORMAT);
}

DateSingle::DateSingle(const DateSingle &date)
    : DateItem(date.frequency_),
      date_(date.date_)
{
}

QJsonObject DateSingle::toJson() const
{
    return QJsonObject({
        { "date", date_.toString(DateItem::FULL_FORMAT) },
        { "frequency", frequency_.tag() }
    });
}

DayOfWeek DateSingle::dayOfWeek() const
{
    return DateUtils::of(date_);
}

bool DateSingle::contains(const DateItem *item) const
{
    auto dateRange = dynamic_cast<const DateRange*>(item);
    if (dateRange != nullptr) {
        return dateRange->contains(this);
    }

    auto dateSingle = dynamic_cast<const DateSingle*>(item);
    if (dateSingle != nullptr) {
        return date_ == dateSingle->date_;
    }

    throw std::invalid_argument("Can't compare dates");
}

bool DateSingle::before(const DateItem *item) const
{
    auto dateRange = dynamic_cast<const DateRange*>(item);
    if (dateRange != nullptr) {
        return (date_ < dateRange->start_) && (date_ < dateRange->end_);
    }

    auto dateSingle = dynamic_cast<const DateSingle*>(item);
    if (dateSingle != nullptr) {
        return date_ < dateSingle->date_;
    }

    throw std::invalid_argument("Can't compare dates");
}

std::unique_ptr<DateItem> DateSingle::copy() const
{
    return std::make_unique<DateSingle>(*this);
}

QString DateSingle::toString() const
{
    return date_.toString(DateItem::PRINT_FORMAT);
}

bool DateSingle::operator==(const QDate &date) const
{
    return date_ == date;
}


