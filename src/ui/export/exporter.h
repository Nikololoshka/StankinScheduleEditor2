#ifndef EXPORTER_H
#define EXPORTER_H

#include "schedule.h"
#include "stdafx.h"
#include <QPrinter>

/**
 * @brief The ExportType enum
 */
enum class ExportType {
    Weekly,
    Full,
};

/**
 * @brief Экспертер расписания по умолчанию (полного).
 */
class Exporter {

public:
    /**
     * @brief Конструктор по умолчанию экспортера.
     */
    Exporter();
    virtual ~Exporter() = default;

    /**
     * @brief Устанавливает путь к расписанию, которое надо будет экспортировать.
     * @param schedulePath путь до расписания.
     */
    void setSchedule(const QString& schedulePath);
    /**
     * @brief Запускает процесс экспорта.
     * @param printer устройства вывода печати расписания.
     */
    void runExport(QPrinter& printer);

    /**
     * @brief Устанавливает шрифт для печати.
     * @param font шрифт печати.
     */
    void setFont(const QFont& font);
    /**
     * @brief Устанавливает, нужно ли отображать в названии расписания его дату (начала и конца).
     * @param showDate показывать ли дату.
     */
    void setShowDate(bool showDate);

protected:
    /**
     * @brief Запускает процесс экспорта.
     * @param printer устройства вывода печати расписания.
     * @param painter объект для рисования.
     */
    virtual void runExport(QPrinter& printer, QPainter& painter);

    /**
     * @brief Отрисовывает название расписания.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param text название расписания.
     * @param flags флаги позиционирования названия внутри области.
     * @return высота отрисованного названия расписания.
    */
    virtual int drawTitleText(QPainter& painter, int x, int y, int width, const QString& text,
        int flags = Qt::AlignCenter | Qt::AlignHCenter);
    /**
     * @brief Отрисовавает расписание.
     * @param printer устройства вывода печати расписания.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина расписания.
     * @param height высота расписания.
    */
    virtual void drawSchedule(QPrinter& printer, QPainter& painter, int x, int y, int width, int height);
    /**
     * @brief Отрисовавает непосредственно расписание.
     * @param painter объект для рисования.
     * @param schedule расписание.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param height высота области.
     */
    virtual void drawScheduleContent(QPainter& painter, Schedule& schedule, int x, int y, int width, int height);

    /**
     * @brief Подбирает размер шрифта для отрисовки в области.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param height высота области.
     * @param text текст.
     * @param flags флаги позиционирования текста внутри области.
     */
    virtual void adaptFontSize(QPainter& painter, int x, int y, int width, int height, const QString& text,
        int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap);

    /**
     * @brief Отрисовавает повернутый текст на 90 градусов против часов стрелки.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param height высота области.
     * @param text текст.
     * @param boundRect нужно ли отрисовывать область, где рисуется текст.
     * @param flags флаги позиционирования текста внутри области.
     * @param padding внутренний отступ текста от области.
     */
    virtual void drawRotateText(QPainter& painter, int x, int y, int width, int height, const QString& text,
        bool boundRect = false, int flags = Qt::AlignCenter, int padding = 0);
    /**
     * @brief Отрисовавает текст внутри заданной области.
     * @param painter объект для рисования.
     * @param x координата по оси асцисс.
     * @param y координата по оси ординат.
     * @param width ширина области.
     * @param height высота области.
     * @param text текст.
     * @param boundRect нужно ли отрисовывать область, где рисуется текст.
     * @param flags флаги позиционирования текста внутри области.
     * @param padding внутренний отступ текста от области.
     */
    virtual void drawInRectText(QPainter& painter, int x, int y, int width, int height, const QString& text,
        bool boundRect = false, int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
        int padding = 0);

    virtual QString dateToString(const QDate& date) const;

protected:
    //! расписание для экспорта
    Schedule schedule_;
    //! название расписания
    QString scheduleName_;
    // шрифт для печати
    QFont font_;
    // показывать дату расписания
    bool showDate_;
};

#endif // EXPORTER_H
