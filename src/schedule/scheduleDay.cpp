#include "scheduleDay.h"

ScheduleDay::ScheduleDay()
    : index_(1)
{

}

void ScheduleDay::add(const Pair &pair)
{
    isAddCheck(pair); // throw std::logic_error
    pairs_.append({index_++, pair});
    reallocate();
}

void ScheduleDay::remove(const Pair &pair)
{
    // TODO
}

int ScheduleDay::size() const
{
    return pairs_.size();
}

int ScheduleDay::row() const
{
    return rows_.size() < 1 ? 1 : rows_.size();
}

int ScheduleDay::column() const
{
    return 8;
}

QVector<Pair> ScheduleDay::fromCell(const Cell &cell) const
{
    QVector<Pair> pairs;
    for (auto& data : pairs_) {
        for (auto id : cell) {
            if (data.id == id) {
                pairs.append(data.pair);
            }
        }
    }

    return pairs;
}

void ScheduleDay::isAddCheck(const Pair &pair) const
{
    for (const auto& data : pairs_ ) {
        if (pair.intersect(data.pair) && !pair.separate(data.pair)) {
            throw std::logic_error(("There can't be two pairs at the same time: '" +
                                    pair.toString() + "' and '" +
                                    data.pair.toString() + "'").toStdString());
        }
    }
}

void ScheduleDay::reallocate()
{
    std::sort(pairs_.begin(), pairs_.end(), [](const PairData &first, const PairData &second) {
        if (first.pair.time().duration() == second.pair.time().duration()) {
            return first.pair.before(second.pair);
        }
        return first.pair.time().duration() < second.pair.time().duration();
    });

    for (auto& data : pairs_) {
        auto& id = data.id;
        auto& pair = data.pair;

        bool insert = false;

        for (auto& row : rows_) {
            Cell cell = row[pair.time().number()];
            auto pairs = this->fromCell(cell);

            if (!pairs.isEmpty() &&
                pairs.first().time().duration() == pair.time().duration()) {
                cell.append(id);
                insert = true;
                break;

            } else {
                bool free = true;
                for (auto& rowCell : row) {
                    auto rowPairs = this->fromCell(rowCell);
                    if (!rowPairs.isEmpty() && rowPairs.first().time().intersect(pair.time())) {
                        free = false;
                        break;
                    }
                }

                if (free) {
                    row[pair.time().number()].append(id);
                    insert = true;
                    break;
                }
            }
        }

        if (!insert) {
            QVector<Cell> row(column(), Cell());

            for (int i = 0; i < pair.time().duration(); ++i) {
                row[pair.time().number()].append(id);
            }
            rows_.append(row);
        }
    }
}

int ScheduleDay::find(unsigned int id) const
{
    for (int i = 0; i < pairs_.size(); ++i) {
        if (pairs_[i].id == id) {
            return i;
        }
    }
    return -1;
}

unsigned int ScheduleDay::EMPTY = 0;
