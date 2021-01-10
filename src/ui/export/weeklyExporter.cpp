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

void WeeklyExporter::drawInColoredRectText(QPainter &painter, int x, int y, int width, int height,
                                           const QString &text, const QColor &color,
                                           bool boundRect, int flags, int padding)
{
    painter.save();
    painter.setBrush(color);
    painter.drawRect(x, y, width, height);
    painter.restore();
    drawInRectText(painter, x, y, width, height, text, boundRect, flags, padding);
}

