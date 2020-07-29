#ifndef SCHEDULEDAY_H
#define SCHEDULEDAY_H

#include <QtWidgets>
#include "pair/pair.h"

using Cell = QVector<unsigned int>;

struct ScheduleCell {
    QString text;
    int rowSpan;
    int columnSpan;

    bool isSpanValid() const;
};

struct ScheduleIndex {
    int row;
    int number;
    int innerRow;

    QString toString() const;
};

class ScheduleDay
{
public:
    ScheduleDay();

    void add(const Pair &pair);
    void remove(const Pair &pair);

    int size() const;
    int row() const;
    int column() const;

    void possibleChange(const std::unique_ptr<Pair> &oldPair,
                        const std::unique_ptr<Pair> &newPair) const;

    ScheduleCell pairsTextByIndex(const ScheduleIndex &index) const;
    QVector<Pair> pairsByIndex(const ScheduleIndex &index) const;
    QVector<Pair> fromCell(const Cell &cell) const;

private:
    void isAddCheck(const Pair &pair) const;
    void reallocate();
    int find(unsigned int id) const;
    int computeRowSpan(int duration, const ScheduleIndex &index) const;

private:
    struct PairData {
        unsigned int id;
        Pair pair;
    };

    unsigned int index_;
    QVector<PairData> pairs_;
    QVector<QVector<Cell>> rows_;

    static unsigned int EMPTY;
};

#endif // SCHEDULEDAY_H
