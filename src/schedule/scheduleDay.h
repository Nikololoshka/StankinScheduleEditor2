#ifndef SCHEDULEDAY_H
#define SCHEDULEDAY_H

#include <QtWidgets>
#include "pair/pair.h"

using Cell = QVector<unsigned int>;

class ScheduleDay
{
public:
    ScheduleDay();

    void add(const Pair &pair);
    void remove(const Pair &pair);

    int size() const;
    int row() const;
    int column() const;

    QVector<Pair> fromCell(const Cell &cell) const;

private:
    void isAddCheck(const Pair &pair) const;
    void reallocate();
    int find(unsigned int id) const;

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
