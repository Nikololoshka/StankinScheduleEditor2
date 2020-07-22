#include "scheduleHeaderView.h"

ScheduleHeaderView::ScheduleHeaderView()
    : QHeaderView(Qt::Vertical)
{
    connect(this, &ScheduleHeaderView::sectionClicked, [](int logicalIndex) {
        qDebug() << logicalIndex;
    });
}

void ScheduleHeaderView::setIndexses(const QVector<int> &indexes)
{
    indexes_ = indexes;
    update();
}


void ScheduleHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    const int REAL_ROW_SIZE = this->size().height() / 6;

    if (indexes_.isEmpty()) {
        QHeaderView::paintSection(painter, rect, logicalIndex);
        return;
    }

    auto realIndex = computeIndex(logicalIndex);
//    qDebug() << indexes_;
//    qDebug() << rect << logicalIndex;
//    qDebug() << realIndex << logicalIndex << model()->headerData(realIndex, orientation(), Qt::DisplayRole);
//    qDebug() << "----------";


    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBackground(Qt::transparent);

    auto table = static_cast<QTableWidget*>(this->parentWidget());
    if (table != nullptr) {
        auto selectedIndex = table->currentRow();
        if (computeIndex(selectedIndex) == realIndex) {
             auto font = painter->font();
             font.setWeight(5);
             font.setBold(true);
             painter->setFont(font);
        }
    }

    int rowSize = REAL_ROW_SIZE;
    if (realIndex == indexes_.size() - 1) {
        rowSize = this->size().height() - realIndex * REAL_ROW_SIZE;
    }

    QHeaderView::paintSection(painter,
                              QRect(rect.x(), realIndex * REAL_ROW_SIZE, rect.width(), rowSize),
                              realIndex);
    /*
    painter->save();

    auto index = computeIndex(logicalIndex);
    int realIndex = index.first;
    int paddingIndex = index.second;
    int rowCount = indexes_[realIndex];

    auto data = model()->headerData(realIndex, orientation(), Qt::DisplayRole).toString();
    qDebug() << data;

    painter->translate(rect.x() + rect.width(), rect.y() + rect.height());
    painter->rotate(-90);
    painter->drawText(QRectF(-rect.height() * paddingIndex, 0, rect.height() * rowCount, -rect.width()),
                      Qt::AlignCenter, data);

    painter->restore();
    */
}

int ScheduleHeaderView::computeIndex(int logicalIndex) const
{
    int tempIndex = logicalIndex;
    int padding = 0;

    for (int i = 0; i < indexes_.size(); ++i) {
        tempIndex -= indexes_[i];
        padding += indexes_[i];

        if (tempIndex < 0) {
            return i;
        }
    }

    return indexes_.size() - 1;
}

