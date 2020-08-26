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
           const Type &type, const Subgroup &subgroup, const Time_ &time, const Date &date)
    : title_(title),
      lecturer_(lecturer),
      classroom_(classroom),
      type_(type),
      subgroup_(subgroup),
      time_(time),
      date_(date)
{

}

Pair::Pair(const Pair &pair)
    : Pair(pair.title_, pair.lecturer_, pair.classroom_,
           pair.type_, pair.subgroup_, pair.time_)
{
    setDate(pair.date_);
}

Pair::Pair(Pair &&pair)
    : Pair(pair.title_, pair.lecturer_, pair.classroom_,
           pair.type_, pair.subgroup_, pair.time_)
{
    setDate(pair.date_);
}

QJsonObject Pair::toJson() const
{
    return QJsonObject({
        { "title", title_ },
        { "lecturer", lecturer_ },
        { "classroom", classroom_ },
        { "type", type_.tag() },
        { "subgroup", subgroup_.tag() },
        { "time", time_.toJson() },
        { "dates", date_.toJson() }
    });
}

Pair& Pair::operator=(const Pair &pair)
{
    setTitle(pair.title_);
    setLecturer(pair.lecturer_);
    setClassroom(pair.classroom_);
    setType(pair.type_);
    setSubgroup(pair.subgroup_);
    setTime(pair.time_);
    setDate(pair.date_);

    return *this;
}

bool Pair::operator==(const Pair &pair) const
{
    return title_ == pair.title_ &&
        lecturer_ == pair.lecturer_ &&
        classroom_ == pair.classroom_ &&
        type_ == pair.type_ &&
        subgroup_ == pair.subgroup_ &&
        time_ == pair.time_ &&
        date_ == pair.date_;
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

Date Pair::date() const
{
    return date_;
}

void Pair::setDate(const Date &date)
{
    date_ = date;
}

std::unique_ptr<Pair> Pair::copy() const
{
    auto pair = std::make_unique<Pair>(*this);
    pair->setDate(this->date());
    return pair;
}

void Pair::setTime(const Time_ &time)
{
    time_ = time;
}

Time_ Pair::time() const
{
    return time_;
}

void Pair::addDate(const std::unique_ptr<DateItem> &item)
{
    date_.addDate(item);
}

void Pair::removeDate(const std::unique_ptr<DateItem> &item)
{
    date_.removeDate(item);
}

bool Pair::contains(const std::unique_ptr<DateItem> &item) const
{
    return date_.contains(item);
}

DayOfWeek Pair::dayOfWeek() const
{
    return date_.dayOfWeek();
}

bool Pair::intersect(const Pair &pair) const
{
    bool isTime = time_.intersect(pair.time_);
    if (!isTime) {
        return false;
    }

    if (date_.contains(pair.date_)) {
        return true;
    }

    return false;
}

bool Pair::separate(const Pair &pair) const
{
    return subgroup_.separate(pair.subgroup_);
}

bool Pair::before(const Pair &pair) const
{
    return date_.before(pair.date_);
}

QString Pair::toString() const
{
    // Title. Lecture. Type. Subgroup. Classroom. [Date1, Date2...]
    return title_ + ". "
           + (lecturer_.isEmpty() ? "" : lecturer_ + ". ")
           + type_.text() + ". "
           + (subgroup_.isShow() ?  subgroup_.text() + ". " : "")
           + (classroom_.isEmpty() ? "" : classroom_ + ". ")
           + date_.toString();
}


