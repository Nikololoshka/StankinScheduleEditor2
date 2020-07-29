#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QtWidgets>
#include <vector>

#include "scheduleDay.h"
#include "pair/pair.h"

class Schedule
{
public:
    Schedule();
    Schedule(const QJsonDocument &value);

    void addPair(const Pair &pair);

    QVector<int> indexes() const;
    int row() const;
    int column() const;

    void possibleChange(const std::unique_ptr<Pair> &oldPair,
                        const std::unique_ptr<Pair> &newPair) const;

    ScheduleCell pairsTextByIndex(const ScheduleIndex &index) const;
    ScheduleCell pairsTextByIndex(int row, int column) const;
    QVector<Pair> pairsByIndex(const ScheduleIndex &index) const;

    ScheduleIndex transform(int row, int column) const;
    void print();

private:
    QMap<DayOfWeek, ScheduleDay> days_;
    std::vector<Pair> pairs_;
};

#endif // SCHEDULE_H
