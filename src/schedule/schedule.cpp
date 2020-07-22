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
    days_[pair.dayOfWeek()].add(pair);
}

QVector<int> Schedule::indexes() const
{
    QVector<int> indexes;
    for (auto& day : days_) {
        indexes.append(day.row());
    }
    return indexes;
}

int Schedule::row() const
{
    int row = 0;

    for (const auto& day : days_) {
        row += day.row();
    }

    return row;
}

int Schedule::column() const
{
    return 8;
}

void Schedule::print()
{
    for (auto &pair : pairs_) {
        qDebug() << pair.toString();
    }
}
