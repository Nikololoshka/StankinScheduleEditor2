#include "stdafx.h"
#include "date.h"
#include "pairException.h"

Date::Date()
{
}

Date::Date(const Date& date)
{
    for (auto& d : date.dates_) {
        dates_.emplace_back(d->copy());
    }
}

Date::Date(Date&& date)
{
    dates_.swap(date.dates_);
}

Date& Date::operator=(const Date& date)
{
    dates_.clear();
    for (const auto& d : date.dates_) {
        dates_.emplace_back(d->copy());
    }

    return *this;
}

const std::unique_ptr<DateItem>& Date::operator[](int index) const
{
    return dates_[static_cast<size_t>(index)];
}

bool Date::operator==(const Date& date) const
{
    if (dates_.size() != date.dates_.size()) {
        return false;
    }

    return contains(date);
}

QJsonArray Date::toJson() const
{
    QJsonArray array;
    for (const auto& date : dates_) {
        array.append(date->toJson());
    }

    return array;
}

std::unique_ptr<DateItem> Date::copy(int index) const
{
    return std::unique_ptr<DateItem>((*this)[index]->copy());
}

void Date::addDate(const std::unique_ptr<DateItem>& item)
{
    if (!dates_.empty()) {
        auto& day = dates_.front();
        if (item->dayOfWeek() != day->dayOfWeek()) {
            throw std::invalid_argument(("Wrong day of the week: "
                + item->toString() + " and " + day->toString())
                                            .toStdString());
        }
    }
    if (contains(item)) {
        throw InvalidAddDateException("Date is intersect: "
            + item->toString() + " in " + toString());
    }

    auto iter = dates_.end();
    for (auto it = dates_.begin(); it != dates_.end(); ++it) {
        if ((*it)->before(item.get())) {
            break;
        }
        iter = it;
    }

    dates_.emplace(iter, item->copy());
}

void Date::removeDate(const std::unique_ptr<DateItem>& item)
{
    if (item.get() == nullptr) {
        return;
    }

    dates_.erase(std::remove_if(dates_.begin(), dates_.end(),
                     [&item](const std::unique_ptr<DateItem>& it) {
                         return it->toString() == item->toString();
                     }),
        dates_.end());
}

int Date::size() const
{
    return static_cast<int>(dates_.size());
}

bool Date::isEmpty() const
{
    return dates_.empty();
}

void Date::changeDate(const std::unique_ptr<DateItem>& oldDate,
    const std::unique_ptr<DateItem>& newDate)
{
    Date copy = *this;
    copy.removeDate(oldDate);
    copy.addDate(newDate);

    dates_.swap(copy.dates_);
}

void Date::possibleChange(const std::unique_ptr<DateItem>& oldDate,
    const std::unique_ptr<DateItem>& newDate) const
{
    Date copy = *this;
    copy.removeDate(oldDate);
    copy.addDate(newDate);
}

bool Date::contains(const std::unique_ptr<DateItem>& item) const
{
    for (const auto& date : dates_) {
        if (date->contains(item.get())) {
            return true;
        }
    }
    return false;
}

bool Date::contains(const Date& date) const
{
    for (const auto& d : date.dates_) {
        if (contains(d)) {
            return true;
        }
    }
    return false;
}

bool Date::before(const Date& date) const
{
    for (const auto& d : dates_) {
        for (const auto& other : date.dates_) {
            if (!d->before(other.get())) {
                return false;
            }
        }
    }
    return true;
}

DayOfWeek Date::dayOfWeek() const
{
    return dates_.front()->dayOfWeek();
}

QString Date::toString() const
{
    QStringList list;
    std::transform(dates_.begin(), dates_.end(), std::back_inserter(list),
        [](const auto& date) -> QString {
            return date->toString();
        });

    return "[" + list.join(", ") + "]";
}
