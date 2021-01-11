#ifndef WEEKLYEXPORTER_H
#define WEEKLYEXPORTER_H

#include "exporter.h"


class WeeklyExporter : public Exporter
{
public:
    WeeklyExporter();
    ~WeeklyExporter() override = default;

    void setStartDate(const QDate &startDate);
    void setEndDate(const QDate &endDate);
    void setColorSubgroupA(const QColor &colorSubgroupA);
    void setColorSubgroupB(const QColor &colorSubgroupB);


protected:
    void drawSchedule(QPrinter &printer, QPainter &painter, int x, int y, int width, int height) override;
    void drawScheduleContentPairs(QPainter &painter, Schedule &schedule, int x, int y,
                                  int rowStepSize, int columnStepSize, int scheduleHeaderSize) override;

private:

    /**
     * @brief Отрисовавает текст внутри заданной области.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param height высота области.
     * @param text текст.
     * @param color цвет области.
     * @param boundRect нужно ли отрисовывать область, где рисуется текст.
     * @param flags флаги позиционирования текста внутри области.
     * @param padding внутренний отступ текста от области.
     */
    void drawInColoredRectText(QPainter &painter, int x, int y, int width, int height,
                               const QString& text, const QColor &color, bool boundRect = false,
                               int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                               int padding = 0);

private:
    //! дата начала
    QDate startDate_;
    //! дата конца
    QDate endDate_;
    //! цвет подгруппы А
    QColor colorSubgroupA_;
    //! цвет подгруппы Б
    QColor colorSubgroupB_;
};

#endif // WEEKLYEXPORTER_H
