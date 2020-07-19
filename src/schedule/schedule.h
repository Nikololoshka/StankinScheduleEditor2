#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QtWidgets>
#include <vector>

#include "pair/pair.h"


class Schedule
{
public:
    static Schedule fromJson(const QJsonDocument &value);

    Schedule();
    void addPair(const Pair &pair);

    void print();

private:
    std::vector<Pair> pairs_;
};

#endif // SCHEDULE_H
