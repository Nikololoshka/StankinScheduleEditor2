#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QtWidgets>
#include <vector>

#include "scheduleDay.h"
#include "pair/pair.h"


class Schedule
{
public:
    static Schedule fromJson(const QJsonDocument &value);

    Schedule();
    void addPair(const Pair &pair);

    QVector<int> indexes() const;
    int row() const;
    int column() const;

    void print();

private:
    QMap<DayOfWeek, ScheduleDay> days_;
    std::vector<Pair> pairs_;
};

#endif // SCHEDULE_H
