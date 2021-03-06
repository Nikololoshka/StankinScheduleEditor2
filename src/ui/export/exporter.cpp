#include "exporter.h"

#include "schedule.h"
#include "dayOfWeek.h"
#include "time_.h"


Exporter::Exporter()
{
    font_ = QApplication::font();
    showDate_ = true;
}


void Exporter::setSchedule(const QString &schedulePath)
{
    QFile file(schedulePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file!" << schedulePath;
        throw std::logic_error("Cannot open file!");
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    schedule_ = Schedule(doc);
    scheduleName_ = QFileInfo(schedulePath).baseName();
}

void Exporter::runExport(QPrinter &printer)
{
    QPainter painter;
    if (!painter.begin(&printer)) {
        throw std::logic_error("Не удалось запустить печать на переданное устройство");
    }

    runExport(printer, painter);
    painter.end();
}

void Exporter::runExport(QPrinter &printer, QPainter &painter)
{
//    qDebug() << printer.pageLayout().margins(QPageLayout::Millimeter);
//    qDebug() << printer.pageLayout().minimumMargins();

//    qDebug() << printer.pageLayout().fullRect();
//    qDebug() << printer.pageLayout().paintRect(QPageLayout::Millimeter);
//    qDebug() << printer.pageLayout().paintRectPixels(printer.resolution());
//    qDebug() << printer.pageLayout().paintRectPoints();
//    qDebug() << painter.window();

    QRectF paintRect = printer.pageLayout().paintRectPixels(printer.resolution());
    int pageX = qRound(paintRect.x());
    int pageY = qRound(paintRect.y());
    int pageWidth = qRound(paintRect.width()) - 2 * pageX;
    int pageHeight = qRound(paintRect.height()) - 2 * pageY;

    QPen pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    drawSchedule(printer, painter, pageX, pageY, pageWidth, pageHeight);
}

int Exporter::drawTitleText(QPainter &painter, int x, int y, int width, const QString &text, int flags)
{
    const int TITLE_FONT_SIZE = 14;

    painter.save();

    QFont currentFont = painter.font();
    currentFont.setPointSize(TITLE_FONT_SIZE);
    painter.setFont(currentFont);

    int titleHeight = painter.fontMetrics().height();
    drawInRectText(painter, x, y, width, titleHeight, text, false, flags);
    titleHeight += titleHeight / 2;

    painter.restore();

    return titleHeight;
}

void Exporter::drawSchedule(QPrinter &printer, QPainter &painter, int x, int y, int width, int height)
{
    Q_UNUSED(printer)

    // отрисовка названия расписания
    QString title = scheduleName_;
    int titleHeight = drawTitleText(painter, x, y, width, title);

    // отрисовка таблицы расписания
    drawScheduleContent(painter, schedule_, x, y + titleHeight, width, height - titleHeight);
}

void Exporter::drawScheduleContent(QPainter &painter, Schedule &schedule, int x, int y, int width, int height)
{
    const int ROW_COUNT = 6;
    const int COLUMN_COUNT = 8;

    const int scheduleHeaderSize = painter.fontMetrics().height();
    const int columnStepSize = (width - scheduleHeaderSize) / COLUMN_COUNT;
    const int rowStepSize = (height - scheduleHeaderSize) / ROW_COUNT;


    // угол расписания
    painter.drawRect(x, y, scheduleHeaderSize, scheduleHeaderSize);

    // столбец дней недели
    for (int i = 0; i < ROW_COUNT; ++i) {
        drawRotateText(painter,
                       x, scheduleHeaderSize + y + rowStepSize * i,
                       scheduleHeaderSize, rowStepSize,
                       DateUtils::dayOfWeekList()[i], true, Qt::AlignCenter, 10);
    }

    // строка с временм
    for (int j = 0; j < COLUMN_COUNT; ++j) {
        drawInRectText(painter,
                       scheduleHeaderSize + x + columnStepSize * j, y,
                       columnStepSize, scheduleHeaderSize,
                       Time_::timeList()[j], true, Qt::AlignCenter, 10);
    }

    // ячейки с парами
    drawScheduleContentPairs(painter, schedule, x, y, rowStepSize, columnStepSize, scheduleHeaderSize);
}

void Exporter::drawScheduleContentPairs(QPainter &painter, Schedule &schedule, int x, int y,
                                        int rowStepSize, int columnStepSize, int scheduleHeaderSize)
{
    const auto indexes = schedule.indexes();
    const auto days = DateUtils::list();
    for (int i = 0; i < days.size(); ++i) {
        int subRowCount = indexes[i];
        int subRowSize = rowStepSize / subRowCount;
        auto pairs = schedule.pairsForDrawingByDay(days[i]);
        for (const auto& pair : pairs) {
            drawInRectText(painter,
                           scheduleHeaderSize + x + columnStepSize * pair.column,
                           scheduleHeaderSize + y + rowStepSize * i + subRowSize * pair.row,
                           columnStepSize * pair.columnSpan,
                           subRowSize * pair.rowSpan,
                           pair.text,
                           true,
                           Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                           30);
        }
    }
}

void Exporter::adaptFontSize(QPainter &painter, int x, int y, int width, int height,
                             const QString &text, int flags)
{
    QFont font = painter.font();
    while (true) {
        auto rect = painter.fontMetrics()
                        .boundingRect(x, y, width, height, flags, text);
        if (rect.width() < width && rect.height() < height) {
            break;
        }
        font.setPointSizeF(font.pointSizeF() * 0.95);
        painter.setFont(font);
    }
}

void Exporter::drawRotateText(QPainter &painter, int x, int y, int width, int height,
                              const QString &text, bool boundRect, int flags, int padding)
{
    painter.save();
    painter.translate(x, y);
    painter.rotate(270);

    drawInRectText(painter, -height, 0, height, width, text, boundRect, flags, padding);
    painter.restore();
}

void Exporter::drawInRectText(QPainter &painter, int x, int y, int width, int height,
                              const QString &text, bool boundRect, int flags, int padding)
{
    painter.save();
    if (boundRect) {
        painter.drawRect(x, y, width, height);
    }
    if (!text.isEmpty()) {
        adaptFontSize(painter, x + padding, y + padding, width - 2 * padding, height - 2 * padding, text, flags);
        painter.drawText(x + padding, y + padding, width - 2 * padding, height - 2 * padding, flags, text);
    }
    painter.restore();
}

QString Exporter::dateToString(const QDate &date) const
{
    return date.toString("dd.MM.yyyy");
}

void Exporter::setShowDate(bool showDate)
{
    showDate_ = showDate;
}

void Exporter::setFont(const QFont &font)
{
    font_ = font;
}
