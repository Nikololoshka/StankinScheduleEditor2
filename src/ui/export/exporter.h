#ifndef EXPORTER_H
#define EXPORTER_H

#include "stdafx.h"
#include "schedule.h"
#include <QPrinter>


/**
 * @brief The ExportType enum
 */
enum class ExportType {
    Weekly,
    Full,
};

/**
 * @brief The Exporter class
 */
class Exporter {
public:
    /**
     * @brief Exporter
     */
    Exporter();

    /**
     * @brief runExport
     * @param printer
     */
    void runExport(QPrinter &printer);

protected:
    /**
     * @brief runExport
     * @param printer
     * @param painter
     */
    void runExport(QPrinter &printer, QPainter &painter);

    /**
     * @brief drawTitleText
     * @param painter
     * @param x
     * @param y
     * @param width
     * @param text
     * @param flags
     * @return
     */
    int drawTitleText(QPainter &painter, int x, int y, int width, const QString &text,
                       int flags = Qt::AlignCenter | Qt::AlignHCenter);
    void drawScheduleContent(QPainter &painter, int x, int y, int width, int height);

    /**
     * @brief adaptFontSize
     * @param painter
     * @param boundRect
     * @param text
     * @param flags
     */
    void adaptFontSize(QPainter &painter, int x, int y, int width, int height, const QString &text,
                       int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap);
    /**
     * @brief drawRotateText
     * @param painter
     * @param x
     * @param y
     * @param width
     * @param height
     * @param text
     * @param flags
     * @param angle
     */
    void drawRotateText(QPainter &painter, int x, int y, int width, int height, const QString& text,
                        bool boundRect = false, int flags = Qt::AlignCenter, int padding = 0);
    /**
     * @brief drawInRectText
     * @param painter
     * @param x
     * @param y
     * @param width
     * @param height
     * @param text
     * @param flags
     */
    void drawInRectText(QPainter &painter, int x, int y, int width, int height, const QString& text,
                        bool boundRect = false, int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                        int padding = 0);
    /**
     * @brief drawInColoredRectText
     * @param painter
     * @param x
     * @param y
     * @param width
     * @param height
     * @param text
     * @param flags
     */
    void drawInColoredRectText(QPainter &painter, int x, int y, int width, int height,
                               const QString& text, const QColor &color,
                               int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap);

};


#endif // EXPORTER_H
