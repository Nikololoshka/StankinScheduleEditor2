#include "pair.h"
#include "pairException.h"


Pair Pair::fromJson(const QJsonObject &value)
{
    Pair pair(value["title"].toString(),
              value["lecturer"].toString(),
              value["classroom"].toString(),
              Type::fromString(value["type"].toString()),
              Subgroup::fromString(value["subgroup"].toString()),
              Time_::fromJson(value["time"].toObject()));

    QJsonArray dates = value["dates"].toArray();
    for (int i = 0; i < dates.size(); ++i) {
        auto item = DateItem::fromJson(dates[i].toObject());
        pair.addDate(item);  // throw exceptions
    }

    return pair;
}

Pair::Pair(const QString &title, const QString &lecturer, const QString &classroom,
           const Type &type, const Subgroup &subgroup, const Time_ &time)
    : title_(title),
      lecturer_(lecturer),
      classroom_(classroom),
      type_(type),
      subgroup_(subgroup),
      time_(time)
{

}

Pair::Pair(const Pair &pair)
    : Pair(pair.title_, pair.lecturer_, pair.classroom_,
           pair.type_, pair.subgroup_, pair.time_)
{
    for (auto& date : pair.dates_) {
        dates_.emplace_back(date->copy());
    }
}

Pair::Pair(Pair &&pair)
    : Pair(pair.title_, pair.lecturer_, pair.classroom_,
           pair.type_, pair.subgroup_, pair.time_)
{
    dates_.swap(pair.dates_);
}

Pair& Pair::operator=(const Pair &pair)
{
    setTitle(pair.title_);
    setLecturer(pair.classroom_);
    setClassroom(pair.classroom_);
    setType(pair.type_);
    setSubgroup(pair.subgroup_);
    setTime(pair.time_);

    dates_.clear();
    for (auto& date : pair.dates_) {
        dates_.emplace_back(date->copy());
    }

    return *this;
}

QString Pair::title() const
{
    return title_;
}

void Pair::setTitle(const QString &title)
{
    if (title.isEmpty()) {
        throw std::invalid_argument("Pair title is empty");
    }

    title_ = title;
}

QString Pair::lecturer() const
{
    return lecturer_;
}

void Pair::setLecturer(const QString &lecturer)
{
    lecturer_ = lecturer;
}

QString Pair::classroom() const
{
    return classroom_;
}

void Pair::setClassroom(const QString &classroom)
{
    classroom_ = classroom;
}

Type Pair::type() const
{
    return type_;
}

void Pair::setType(const Type &type)
{
    type_ = type;
}

Subgroup Pair::subgroup() const
{
    return subgroup_;
}

void Pair::setSubgroup(const Subgroup &subgroup)
{
    subgroup_ = subgroup;
}

void Pair::addDate(const std::unique_ptr<DateItem> &item)
{
    if (!dates_.empty()) {
        auto& day = dates_.front();
        if (item->dayOfWeek() != day->dayOfWeek()) {
            throw std::invalid_argument(("Wrong day of the week: " +
                                         item->toString() + " and " +
                                         day->toString()).toStdString());
        }
    }
    if (contains(item)) {
        throw InvalidAddDateException("Date is intersect: " + item->toString() +
                                      " in " + datesToString());
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

void Pair::removeDate(DateItem *const item)
{
    // dates_.removeOne(item);
}

bool Pair::contains(const std::unique_ptr<DateItem> &item) const
{
    for (const auto& date : dates_ ) {
        if (date->contains(item.get())) {
            return true;
        }
    }
    return false;
}

DayOfWeek Pair::dayOfWeek() const
{
    return dates_.front()->dayOfWeek();
}

bool Pair::intersect(const Pair &pair) const
{
    bool isTime = time_.intersect(pair.time_);
    if (!isTime) {
        return false;
    }

    for (const auto& date : pair.dates_) {
        if (this->contains(date)) {
            return true;
        }
    }

    return false;
}

bool Pair::separate(const Pair &pair) const
{
    return subgroup_.separate(pair.subgroup_);
}

bool Pair::before(const Pair &pair) const
{
    for (const auto& date : dates_) {
        for (const auto& otherDate : pair.dates_) {
            if (!date->before(otherDate.get())) {
                return false;
            }
        }
    }
    return true;
}

QString Pair::toString() const
{
    // Title. Lecture. Type. Subgroup. Classroom. [Date1, Date2...]
    return title_ + ". "
           + (lecturer_.isEmpty() ? "" : lecturer_ + ". ")
           + type_.text() + ". "
           + (subgroup_.isShow() ?  subgroup_.text() + ". " : "")
           + (classroom_.isEmpty() ? "" : classroom_ + ". ")
           + datesToString();
}

QString Pair::datesToString() const
{
    QStringList list;
    std::transform(dates_.begin(), dates_.end(), std::back_inserter(list),
                   [](auto& date) -> QString {
                       return date->toString();
                   });

    return "[" + list.join(", ") + "]";
}

void Pair::setTime(const Time_ &time)
{
    time_ = time;
}

Time_ Pair::time() const
{
    return time_;
}
