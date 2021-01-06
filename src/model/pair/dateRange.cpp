#include "dateRange.h"
#include "dateSingle.h"

DateRange::DateRange(const QDate& start, const QDate& end, const Frequency& frequency)
    : DateItem(frequency)
{
    init(start, end, frequency);
}

DateRange::DateRange(const QJsonObject& item, const Frequency& frequency)
    : DateItem(frequency)
{
    auto dateString = item["date"].toString();

    auto list = dateString.split("-");
    if (list.size() != 2) {
        throw std::invalid_argument("Date range is invald: "
            + dateString.toStdString());
    }

    QDate start = QDate::fromString(list[0], DateItem::FULL_FORMAT);
    QDate end = QDate::fromString(list[1], DateItem::FULL_FORMAT);

    init(start, end, frequency);
}

DateRange::DateRange(const DateRange& date)
    : DateItem(date.frequency_)
    , start_(date.start_)
    , end_(date.end_)
{
}

void DateRange::init(const QDate& start, const QDate& end, const Frequency& frequency)
{
    qint64 days = start.daysTo(end);
    if ((days % frequency.delta() != 0) || (days <= 0)) {
        throw std::invalid_argument(("Date range is not correct: "
            + start.toString() + " - " + end.toString() + ", "
            + frequency.tag())
                                        .toStdString());
    }

    if (DateUtils::of(start) != DateUtils::of(end)) {
        throw std::invalid_argument(("Date range day of week is not correct: "
            + start.toString() + " - " + end.toString() + ", "
            + frequency.tag())
                                        .toStdString());
    }

    start_ = start;
    end_ = end;
}

QDate DateRange::start() const
{
    return start_;
}

QDate DateRange::end() const
{
    return end_;
}

QJsonObject DateRange::toJson() const
{
    return {
        { "date", start_.toString(DateItem::FULL_FORMAT) + "-" + end_.toString(DateItem::FULL_FORMAT) },
        { "frequency", frequency_.tag() }
    };
}

DayOfWeek DateRange::dayOfWeek() const
{
    return DateUtils::of(start_);
}

bool DateRange::contains(const DateItem* item) const
{
    auto dateRange = dynamic_cast<const DateRange*>(item);
    if (dateRange != nullptr) {
        for (int i = 0; i < count(); ++i) {
            for (int j = 0; j < dateRange->count(); ++j) {
                if ((*this)[i] == (*dateRange)[j]) {
                    return true;
                }
            }
        }
        return false;
    }

    auto dateSingle = dynamic_cast<const DateSingle*>(item);
    if (dateSingle != nullptr) {
        for (int i = 0; i < count(); ++i) {
            if ((*dateSingle) == (*this)[i]) {
                return true;
            }
        }
        return false;
    }

    throw std::invalid_argument("Can't compare dates");
}

bool DateRange::before(const DateItem* item) const
{
    auto dateRange = dynamic_cast<const DateRange*>(item);
    if (dateRange != nullptr) {
        return (start_ < dateRange->start_) && (end_ < dateRange->end_);
    }

    auto dateSingle = dynamic_cast<const DateSingle*>(item);
    if (dateSingle != nullptr) {
        return (start_ < dateSingle->date_) && (end_ < dateSingle->date_);
    }

    throw std::invalid_argument("Can't compare dates");
}

std::unique_ptr<DateItem> DateRange::copy() const
{
    return std::make_unique<DateRange>(*this);
}

QString DateRange::toString() const
{
    return start_.toString(DateItem::PRINT_FORMAT)
        + "-" + end_.toString(DateItem::PRINT_FORMAT);
}

qint64 DateRange::count() const
{
    return start_.daysTo(end_) / frequency_.delta();
}

QDate DateRange::at(const int index) const
{
    if (index >= count() || index < 0) {
        throw std::out_of_range(("DateRange index not found: index: "
            + QString::number(index) + ", count: " + QString::number(count()))
                                    .toStdString());
    }
    return start_.addDays(frequency_.delta() * index);
}

QDate DateRange::operator[](const int index) const
{
    return start_.addDays(frequency_.delta() * index);
}
