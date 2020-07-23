#ifndef SCHEDULEHORIZONTALHEADER_H
#define SCHEDULEHORIZONTALHEADER_H

#include <QtWidgets>

class ScheduleHorizontalHeader : public QHeaderView
{
public:
    ScheduleHorizontalHeader(QWidget *parent = nullptr);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;

private:
    void setBoldText(QPainter *painter) const;
};

#endif // SCHEDULEHORIZONTALHEADER_H
