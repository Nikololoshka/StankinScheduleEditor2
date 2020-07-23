#include "scheduleHorizontalHeader.h"

ScheduleHorizontalHeader::ScheduleHorizontalHeader(QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
    setDefaultAlignment(Qt::AlignCenter);
}

void ScheduleHorizontalHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBackground(Qt::transparent);

    auto table = static_cast<QTableWidget*>(this->parentWidget());
    auto indexes = table->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty()) {
        auto selectedIndex = indexes.first().column();
        if (selectedIndex == logicalIndex) {
            setBoldText(painter);
        }
    }

    QHeaderView::paintSection(painter, rect, logicalIndex);
}

void ScheduleHorizontalHeader::setBoldText(QPainter *painter) const
{
    auto font = painter->font();
    font.setBold(true);
    painter->setFont(font);
}
