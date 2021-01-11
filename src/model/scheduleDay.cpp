#include "scheduleDay.h"

bool ScheduleCell::isSpanValid() const
{
    return rowSpan > 1 || columnSpan > 1;
}

ScheduleDay::ScheduleDay()
    : index_(1)
    , rows_(1, QVector<Cell>(8))
{
}

QJsonArray ScheduleDay::toJson() const
{
    QJsonArray array;
    for (const auto& data : pairs_) {
        array.append(data.pair.toJson());
    }
    return array;
}

void ScheduleDay::add(const Pair& pair)
{
    isAddCheck(pair); // throw std::logic_error
    pairs_.emplace_back(PairData { index_++, pair });
    reallocate();
}

void ScheduleDay::remove(const Pair& pair)
{
    pairs_.erase(std::remove_if(pairs_.begin(), pairs_.end(), [&pair](const PairData& data) {
        return pair == data.pair;
    }),
        pairs_.end());
    reallocate();
}

size_t ScheduleDay::size() const
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

void ScheduleDay::changePair(const std::optional<Pair>& oldPair,
    const std::optional<Pair>& newPair)
{
    possibleChange(oldPair, newPair);

    pairs_.emplace_back(PairData { index_++, *newPair });
    if (oldPair.has_value()) {
        remove(*oldPair);
    }

    reallocate();
}

void ScheduleDay::possibleChange(const std::optional<Pair>& oldPair,
    const std::optional<Pair>& newPair) const
{
    bool isNull = true;
    if (oldPair.has_value()) {
        if (oldPair->dayOfWeek() != newPair->dayOfWeek()) {
            throw std::logic_error(("Wrong day of the week: "
                + DateUtils::toString(oldPair->dayOfWeek()) + " and " + DateUtils::toString(newPair->dayOfWeek()))
                                       .toStdString());
        }

        isNull = false;
    }

    for (auto& data : pairs_) {
        if (!isNull && data.pair == *oldPair) {
            continue;
        }

        if (newPair->intersect(data.pair) && !newPair->separate(data.pair)) {
            throw std::logic_error(("There can't be two pairs at the same time: '"
                + newPair->toString() + "' and '" + data.pair.toString() + "'")
                                       .toStdString());
        }
    }
}

ScheduleCell ScheduleDay::pairsTextByIndex(const ScheduleIndex& index) const
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
    if (!pairs.empty()) {
        columnSpan = pairs.front().time().duration();
        rowSpan = computeRowSpan(pairs.front().time().duration(), index);
    } else {
        rowSpan = computeRowSpan(1, index);
    }

//    qDebug() << "========== day ============";
//    for (int i = 0; i < rows_.size(); ++i) {
//        QString r = "";
//        for (int j = 0; j < rows_[i].size(); ++j) {
//            r += "[";
//            for (int k = 0; k < rows_[i][j].size(); ++k) {
//                r += QString::number(rows_[i][j][k]) + ", ";
//            }
//            r += "] ";
//        }
//        qDebug() << r;
//    }

    return {
        texts.join("\n"),
        rowSpan,
        columnSpan
    };
}

std::vector<Pair> ScheduleDay::pairsByIndex(const ScheduleIndex& index) const
{
    auto& row = rows_.at(index.innerRow);
    auto& cell = row.at(index.number);
    return fromCell(cell);
}

std::vector<Pair> ScheduleDay::fromCell(const Cell& cell) const
{
    std::vector<Pair> pairs;

    if (cell.isEmpty()) {
        return pairs;
    }

    for (auto& data : pairs_) {
        for (auto id : cell) {
            if (data.id == id) {
                pairs.emplace_back(data.pair);
            }
        }
    }

    return pairs;
}

QVector<ScheduleDrawingCell> ScheduleDay::pairsForDrawing() const
{
    struct Temp {
        Temp(Cell& c, int rowSpan, int columnSpan, int row, int column) {
            this->c = c;
            this->rowSpan = rowSpan;
            this->columnSpan = columnSpan;
            this->row = row;
            this->column = column;
        }

        Cell c;
        int rowSpan;
        int columnSpan;
        int row;
        int column;
    };
    std::vector<Temp> tests;

    QVector<QVector<Cell>> cells = rows_;
    for (int i = 0; i < cells.size(); ++i) {
        for (int j = 0; j < cells[i].size(); ++j) {
            auto currentCell = cells[i][j];
            if (!currentCell.isEmpty() && std::all_of(tests.begin(), tests.end(),
                                                      [currentCell](Temp &t) { return t.c != currentCell; })) {
                auto cell = fromCell(currentCell);
                int duration = cell.front().time().duration();
                tests.emplace_back(currentCell, 1, duration, i, j);

                // по гориз. текущую строку
                bool prevRow = i - 1 >= 0;
                bool nextRow = i + 1 < cells.size();

                for (int k = 0; k < duration; ++k) {
                    cells[i][j + k] = currentCell;
                    if (nextRow && !cells[i + 1][j + k].isEmpty()) {
                        nextRow = false;
                    }
                    if (prevRow && !cells[i - 1][j + k].isEmpty()) {
                        prevRow = false;
                    }
                }

                // дозаполнение сверху
                int m = i + 1;
                while (nextRow) {
                    nextRow = m + 1 < cells.size();
                    for (int k = 0; k < duration; ++k) {
                        cells[m][j + k] = currentCell;
                        if (nextRow && !cells[m + 1][j + k].isEmpty()) {
                            nextRow = false;
                        }
                    }
                    ++m;
                    ++tests.back().rowSpan;
                }

                // дозаполнение снизу
                int n = i - 1;
                while (prevRow) {
                    prevRow = n - 1 >= 0;
                    for (int k = 0; k < duration; ++k) {
                        cells[n][j + k] = currentCell;
                        if (prevRow && !cells[n - 1][j + k].isEmpty()) {
                            prevRow = false;
                        }
                    }
                    --n;
                    ++tests.back().rowSpan;
                    --tests.back().row;
                }
            }
        }
    }

//    qDebug() << "======================";
    // добавление пустых ячеек
    unsigned int index = index_;
    for (int i = 0; i < cells.size(); ++i) {
        for (int j = 0; j < cells[i].size(); ++j) {
            if (cells[i][j].isEmpty()) {
                bool prevRow = i - 1 >= 0;
                bool nextRow = i + 1 < cells.size();

                cells[i][j].append(index++);
                tests.emplace_back(cells[i][j], 1, 1, i, j);

                if (nextRow && !cells[i + 1][j].isEmpty()) {
                    nextRow = false;
                }
                if (prevRow && !cells[i - 1][j].isEmpty()) {
                    prevRow = false;
                }

                // дозаполнение сверху
                int m = i + 1;
                while (nextRow) {
                    nextRow = m + 1 < cells.size();
                    cells[m][j] = cells[i][j];
                    ++tests.back().rowSpan;
                    ++m;
                }

                // дозаполнение снизу
                int n = i - 1;
                while (prevRow) {
                    prevRow = n - 1 >= 0;
                    cells[n][j] = cells[i][j];
                    ++tests.back().rowSpan;
                    --tests.back().row;
                    --n;
                }
            }
        }
//        qDebug() << cells[i];
    }

    QVector<ScheduleDrawingCell> temps;
    for (auto &t : tests) {
        QStringList texts;
        for (auto& pair : fromCell(t.c)) {
            texts << pair.toString();
        }

        temps.append({
            t.row,
            t.column,
            texts.join("\n"),
            t.rowSpan,
            t.columnSpan
        });
    }


//    QVector<ScheduleDrawingCell> empties;

//    for (int i = 0; i < rows_.size(); ++i) {
//        for (int j = 0; j < rows_[i].size(); ++j) {
//            auto cell = pairsTextByIndex({-1, j, i});
//            if (cell.text.isEmpty()) {
//                empties.append({i, j, cell});
//            } else {
//                temps.append({i, j, cell});
//            }
//        }
//    }

////    qDebug() << empties.size();
//    for (auto &pair : temps) {
//        int i = 0;
//        while (i < empties.size()) {
//            auto &e = empties[i];
//            if (((pair.row <= e.row) && ((pair.row + pair.cell.rowSpan - 1) >= e.row))
//                && ((pair.column <= e.column) && ((pair.column + pair.cell.columnSpan - 1) >= e.column))) {
////                qDebug() << pair.cell.text << pair.column << e.column << pair.column + pair.cell.columnSpan << e.column;
//                empties.remove(i);
//            } else {
//                ++i;
//            }
//        }
//    }
////    qDebug() << empties.size();

//    return temps + empties;
    return temps;
}

const Pair &ScheduleDay::operator[](size_t index) const
{
    return pairs_[index].pair;
}

void ScheduleDay::isAddCheck(const Pair& pair) const
{
    for (const auto& data : pairs_) {
        if (pair.intersect(data.pair) && !pair.separate(data.pair)) {
            throw std::logic_error(("There can't be two pairs at the same time: '"
                + pair.toString() + "' and '" + data.pair.toString() + "'")
                                       .toStdString());
        }
    }
}

void ScheduleDay::reallocate()
{
    std::sort(pairs_.begin(), pairs_.end(), [](const PairData& first, const PairData& second) {
        if (first.pair.time().duration() == second.pair.time().duration()) {
            return first.pair.before(second.pair);
        }
        return first.pair.time().duration() > second.pair.time().duration();
    });

    rows_.clear();
    rows_.fill(QVector<Cell>(8), 1);

    for (auto& data : pairs_) {
        auto& id = data.id;
        auto& pair = data.pair;

        bool insert = false;

        for (auto& row : rows_) {
            Cell& cell = row[pair.time().number()];
            auto pairs = this->fromCell(cell);

            if (!pairs.empty() && pairs.front().time().duration() == pair.time().duration() && isMerge(pairs, pair)) {
                cell.append(id);
                insert = true;
                break;

            } else {
                bool free = true;
                for (auto& rowCell : row) {
                    auto rowPairs = this->fromCell(rowCell);
                    if (!rowPairs.empty() && rowPairs.front().time().intersect(pair.time())) {
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

            for (int i = 0; i < 1 /* pair.time().duration() */; ++i) {
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

int ScheduleDay::computeRowSpan(int duration, const ScheduleIndex& index) const
{
    int rowSpanCount = 1;

    QVector<bool> freeRow(column());
    for (int i = index.innerRow + 1; i < rows_.size(); ++i) {
        const auto& row = rows_[i];
        freeRow.fill(true, column());

        for (int j = 0; j < row.size(); ++j) {
            auto pairs = fromCell(row[j]);
            if (!pairs.empty()) {
                auto time = pairs.front().time();
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

bool ScheduleDay::isMerge(const std::vector<Pair> &pairs, const Pair &pair) const
{
    for (const auto &p : pairs) {
        if (p.subgroup() == pair.subgroup()) {
            return true;
        }
    }
    return false;
}

QString ScheduleIndex::toString() const
{
    return QString("Координаты: (%1; %2; %3)")
        .arg(row)
        .arg(number)
        .arg(innerRow);
}
