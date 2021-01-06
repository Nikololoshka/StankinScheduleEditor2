#include "time_.h"

Time_ Time_::fromJson(const QJsonObject& value)
{
    return Time_(QTime::fromString(value["start"].toString(), PATTERN),
        QTime::fromString(value["end"].toString(), PATTERN));
}

const QString Time_::PATTERN = "h:mm";

QVector<QTime> Time_::startTime()
{
    return {
        QTime(8, 30), QTime(10, 20), QTime(12, 20), QTime(14, 10),
        QTime(16, 00), QTime(18, 00), QTime(19, 40), QTime(21, 20)
    };
}

QVector<QTime> Time_::endTime()
{
    return {
        QTime(10, 10), QTime(12, 00), QTime(14, 00), QTime(15, 50),
        QTime(17, 40), QTime(19, 30), QTime(21, 10), QTime(22, 50)
    };
}

QStringList Time_::timeList()
{
    QStringList list;

    auto start = startTime();
    auto end = endTime();
    for (auto s = start.begin(), e = end.begin(); s != start.end() && e != end.end(); ++s, ++e) {
        list << s->toString(PATTERN) + " - " + e->toString(PATTERN);
    }

    return list;
}

Time_::Time_(const QTime& start, const QTime& end)
    : start_(start)
    , end_(end)
{
    init();
}

Time_::Time_(const QString& start, const QString& end)
    : Time_(QTime::fromString(start, PATTERN),
        QTime::fromString(end, PATTERN))
{
}

void Time_::init()
{
    int startIndex = startTime().indexOf(start_);
    int endIndex = endTime().indexOf(end_);

    if (startIndex == -1 || endIndex == -1 || endIndex < startIndex) {
        throw std::invalid_argument(("Not correct time: '" + start_.toString()
            + "', '" + end_.toString() + "'")
                                        .toStdString());
    }

    number_ = startIndex;
    duration_ = endIndex - startIndex + 1;
}

QString Time_::start() const
{
    return start_.toString(PATTERN);
}

QString Time_::end() const
{
    return end_.toString(PATTERN);
}

bool Time_::intersect(const Time_& time) const
{
    return (start_ >= time.start_ && end_ <= time.end_)
        || (start_ <= time.start_ && end_ >= time.end_)
        || (start_ <= time.end_ && end_ >= time.end_);
}

QJsonObject Time_::toJson() const
{
    return QJsonObject({ { "start", start_.toString(PATTERN) },
        { "end", end_.toString(PATTERN) } });
}

int Time_::number() const
{
    return number_;
}

int Time_::duration() const
{
    return duration_;
}

bool Time_::operator==(const Time_& time) const
{
    return start_ == time.start_ && end_ == time.end_;
}
