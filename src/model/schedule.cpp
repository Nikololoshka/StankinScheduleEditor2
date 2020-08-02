#include "schedule.h"


Schedule::Schedule()
{
    for (auto& day : DateUtils::list()) {
        days_[day];
    }
}

Schedule::Schedule(const QJsonDocument &value)
    : Schedule()
{
    QJsonArray pairs = value.array();
    for (int i = 0; i < pairs.size(); ++i) {
        Pair pair = Pair::fromJson(pairs[i].toObject());
        addPair(std::move(pair));
    }
}

QJsonArray Schedule::toJson() const
{
    QJsonArray array;
    for (auto& day : days_) {
        auto dayArray = day.toJson();
        for (int i = 0; i < dayArray.size(); ++i) {
            array.append(dayArray[i]);
        }
    }
    return array;
}

void Schedule::addPair(const Pair &pair)
{
    days_[pair.dayOfWeek()].add(pair);
}

void Schedule::remove(const Pair &pair)
{
    days_[pair.dayOfWeek()].remove(pair);
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

void Schedule::changePair(const std::optional<Pair> &oldPair,
                          const std::optional<Pair> &newPair)
{
    days_[newPair->dayOfWeek()].changePair(oldPair, newPair);
}

void Schedule::possibleChange(const std::optional<Pair> &oldPair,
                              const std::optional<Pair> &newPair) const
{
    days_[newPair->dayOfWeek()].possibleChange(oldPair, newPair);
}

ScheduleCell Schedule::pairsTextByIndex(const ScheduleIndex &index) const
{
    return days_[DateUtils::of(index.row)].pairsTextByIndex(index);
}

ScheduleCell Schedule::pairsTextByIndex(int row, int column) const
{
    return pairsTextByIndex(transform(row, column));
}

QVector<Pair> Schedule::pairsByIndex(const ScheduleIndex &index) const
{
    return days_[DateUtils::of(index.row)].pairsByIndex(index);
}

ScheduleIndex Schedule::transform(int row, int column) const
{
    auto indexes = this->indexes();
    int tempIndex = row;

    for (int i = 0; i < indexes.size(); ++i) {
        tempIndex -= indexes[i];

        if (tempIndex < 0) {
            return { i, column, indexes[i] + tempIndex };
        }
    }

    return { indexes.size() - 1, column, indexes.last() - 1 };
}

void Schedule::print()
{
    for (auto &pair : pairs_) {
        qDebug() << pair.toString();
    }
}
