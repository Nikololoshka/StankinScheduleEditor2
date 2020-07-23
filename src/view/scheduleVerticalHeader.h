#ifndef SCHEDULEVERTICALHEADER_H
#define SCHEDULEVERTICALHEADER_H

#include <QtWidgets>


class ScheduleVerticalHeader : public QHeaderView
{
public:
    ScheduleVerticalHeader(QWidget *parent = nullptr);
    void setIndexses(const QVector<int> &indexes);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;

private:
    int computeIndex(int logicalIndex) const;
    void setBoldText(QPainter *painter) const;

private:
    QVector<int> indexes_;
};

#endif // SCHEDULEVERTICALHEADER_H
