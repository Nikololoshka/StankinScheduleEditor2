#include "scheduleDay.h"

bool ScheduleCell::isSpanValid() const
{
    return rowSpan > 1 || columnSpan > 1;
}

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

void ScheduleDay::possibleChange(const std::unique_ptr<Pair> &oldPair,
                                 const std::unique_ptr<Pair> &newPair) const
{
    for (auto& data : pairs_) {
        if (oldPair.get() != nullptr && data.pair == *oldPair) {
            continue;
        }

        if (newPair->intersect(data.pair) && !newPair->separate(data.pair)) {
            throw std::logic_error(("There can't be two pairs at the same time: '" +
                                    newPair->toString() + "' and '" +
                                    data.pair.toString() + "'").toStdString());
        }
    }
}

ScheduleCell ScheduleDay::pairsTextByIndex(const ScheduleIndex &index) const
{
    auto& row = rows_.at(index.innerRow);
    auto& cell = row.at(index.number);

    auto pairs = fromCell(cell);

    QStringList texts;
    for (auto& pair : pairs) {
        texts << pair.toString();
    }

    int columnSpan = 1;
    int rowSpan = 1;
    if (!pairs.isEmpty()) {
        columnSpan = pairs.first().time().duration();
        rowSpan = computeRowSpan(pairs.first().time().duration(), index);
    } else {
        rowSpan = computeRowSpan(1, index);
    }

    return {
        texts.join("\n"),
        rowSpan,
        columnSpan
    };
}

QVector<Pair> ScheduleDay::pairsByIndex(const ScheduleIndex &index) const
{
    auto& row = rows_.at(index.innerRow);
    auto& cell = row.at(index.number);
    return fromCell(cell);
}

QVector<Pair> ScheduleDay::fromCell(const Cell &cell) const
{
    QVector<Pair> pairs;

    if (cell.isEmpty()) {
        return pairs;
    }

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

int ScheduleDay::computeRowSpan(int duration, const ScheduleIndex &index) const
{
    int rowSpanCount = 1;

    QVector<bool> freeRow(column());
    for (int i = index.innerRow + 1; i < rows_.size(); ++i) {
        const auto& row = rows_[i];
        freeRow.fill(true, column());

        for (int j = 0; j < row.size(); ++j) {
            auto pairs = fromCell(row[j]);
            if (!pairs.isEmpty()) {
                auto time = pairs.first().time();
                for (int k = 0; k < time.duration(); ++k) {
                    freeRow[time.number() + k] = false;
                }
            }
        }

        bool free = true;
        for (int j = 0; j < duration; ++j) {
            if (!freeRow[index.number + j]) {
                free = false;
                break;
            }
        }

        if (free) {
            ++rowSpanCount;
        }
    }

    return rowSpanCount;
}

unsigned int ScheduleDay::EMPTY = 0;


QString ScheduleIndex::toString() const
{
    return QString("Координаты: (%1; %2; %3)")
        .arg(row).arg(number).arg(innerRow);
}
