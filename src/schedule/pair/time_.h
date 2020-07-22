#ifndef TIME_H
#define TIME_H

#include <QtWidgets>

class Time_
{
public:
    static Time_ fromJson(const QJsonObject &value);

    static const QString PATTERN;
    static QVector<QTime> startTime();
    static QVector<QTime> endTime();

    static QStringList timeList();

    Time_(const QTime &start, const QTime &end);
    Time_(const Time_ &time) = default;

    bool intersect(const Time_ &time) const;
    QJsonObject toJson() const;

    int number() const;
    int duration() const;

    bool operator==(const Time_ &time) const;

private:
    QTime start_;
    QTime end_;

    int number_;
    int duration_;
};

#endif // TIME_H
