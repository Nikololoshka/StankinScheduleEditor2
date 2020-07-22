#ifndef SCHEDULEHEADERVIEW_H
#define SCHEDULEHEADERVIEW_H

#include <QtWidgets>


class ScheduleHeaderView : public QHeaderView
{
public:
    ScheduleHeaderView();
    void setIndexses(const QVector<int> &indexes);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;

private:
    int computeIndex(int logicalIndex) const;

private:
    QVector<int> indexes_;
};

#endif // SCHEDULEHEADERVIEW_H
