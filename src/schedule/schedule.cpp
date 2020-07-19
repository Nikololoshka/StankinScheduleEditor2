#include "schedule.h"

Schedule Schedule::fromJson(const QJsonDocument &value)
{
    Schedule schedule;
    QJsonArray pairs = value.array();
    for (int i = 0; i < pairs.size(); ++i) {
        Pair pair = Pair::fromJson(pairs[i].toObject());
        schedule.addPair(std::move(pair));
    }

    return schedule;
}

Schedule::Schedule()
{

}

void Schedule::addPair(const Pair &pair)
{
    pairs_.push_back(pair);
}

void Schedule::print()
{
    for (auto &pair : pairs_) {
        qDebug() << pair.toString();
    }
}
