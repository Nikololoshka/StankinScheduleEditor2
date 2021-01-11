#include "weeklyExporter.h"

WeeklyExporter::WeeklyExporter() : Exporter()
{
    startDate_ = QDate(2020, 11, 16);
    endDate_ = QDate(2020, 12, 21);
}

void WeeklyExporter::setStartDate(const QDate &startDate)
{
    startDate_ = startDate;
}

void WeeklyExporter::setEndDate(const QDate &endDate)
{
    endDate_ = endDate;
}

void WeeklyExporter::setColorSubgroupA(const QColor &colorSubgroupA)
{
    colorSubgroupA_ = colorSubgroupA;
}

void WeeklyExporter::setColorSubgroupB(const QColor &colorSubgroupB)
{
    colorSubgroupB_ = colorSubgroupB;
}

void WeeklyExporter::drawSchedule(QPrinter &printer, QPainter &painter, int x, int y, int width, int height)
{
    auto start = startDate_;
    auto end = start.addDays(6);
    while (true) {

        // отрисовка названия расписания
        QString title = scheduleName_;
        title += " [" + dateToString(start) + " - " + dateToString(end) + "]";

        int titleHeight = drawTitleText(painter, x, y, width, title);

        auto schedule = schedule_.slice(start, end);

        // отрисовка таблицы расписания
        drawScheduleContent(painter, schedule, x, y + titleHeight, width, height - titleHeight);

        start = start.addDays(7);
        end = start.addDays(6);
        if (start >= endDate_) {
            break;
        }
        printer.newPage();
    }
}

void WeeklyExporter::drawScheduleContentPairs(QPainter &painter, Schedule &schedule, int x, int y,
                                              int rowStepSize, int columnStepSize, int scheduleHeaderSize)
{
    const auto indexes = schedule.indexes();
    const auto days = DateUtils::list();
    for (int i = 0; i < days.size(); ++i) {
        int subRowCount = indexes[i];
        int subRowSize = rowStepSize / subRowCount;
        auto pairs = schedule.pairsForDrawingByDay(days[i]);
        for (const auto& pair : pairs) {
            QColor color;
            if (pair.text.contains("(А)")) {
                color = colorSubgroupA_;
            } else if (pair.text.contains("(Б)")) {
                color = colorSubgroupB_;
            } else {
                color = QColor(-1, -1, -1);
            }

            drawInColoredRectText(painter,
                                  scheduleHeaderSize + x + columnStepSize * pair.column,
                                  scheduleHeaderSize + y + rowStepSize * i + subRowSize * pair.row,
                                  columnStepSize * pair.columnSpan,
                                  subRowSize * pair.rowSpan,
                                  pair.text,
                                  color,
                                  true,
                                  Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                                  30);
        }
    }
}

void WeeklyExporter::drawInColoredRectText(QPainter &painter, int x, int y, int width, int height,
                                           const QString &text, const QColor &color,
                                           bool boundRect, int flags, int padding)
{
    if (color.isValid()) {
        painter.save();
        painter.setBrush(color);
        painter.drawRect(x, y, width, height);
        painter.restore();
    }
    drawInRectText(painter, x, y, width, height, text, boundRect, flags, padding);
}

