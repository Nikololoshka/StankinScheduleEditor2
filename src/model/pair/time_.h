#ifndef TIME_H
#define TIME_H

#include <QtWidgets>

/**
 * @brief Время пары.
 */
class Time_ {

public:
    /**
     * @brief Возвращает время пары из json объекта.
     * @param value json объект.
     * @throw std::invalid_argument если время начала или конца
     *        не зарегистрированы в возможных значениях.
     */
    static Time_ fromJson(const QJsonObject& value);

    /**
     * @brief Шаблон, по которому сохраняется/читается время.
     */
    static const QString PATTERN;

    /**
     * @brief Возвращает список возможных времен начала пар.
     */
    static QVector<QTime> startTime();
    /**
     * @brief
     */
    static QVector<QTime> endTime();

    /**
     * @brief Возвращает список времен пар по порядку.
     *
     * Возвращаемое время представлено как '<start> - <end>' набор.
     * Где <start> - время начала пары и <end> время конца.
     */
    static QStringList timeList();

    /**
     * @brief Конструктор из времени начала и конца пары.
     * @param start время начала.
     * @param end время конца.
     * @throw std::invalid_argument если время начала или конца
     *        не зарегистрированы в возможных значениях.
     */
    Time_(const QTime& start, const QTime& end);
    /**
     * Аналогичен контруктору с временем, но использует в качестве
     * параметров строки.
     * @see Time_(const QTime& start, const QTime& end)
     */
    Time_(const QString& start, const QString& end);
    Time_(const Time_& time) = default;

    /**
     * @brief Время начало в виде строки.
     */
    QString start() const;
    /**
     * @brief Время конца в виде строки.
     */
    QString end() const;

    /**
     * @brief Определяет, пересекаются ли времена.
     * @param time другое время.
     */
    bool intersect(const Time_& time) const;
    /**
     * @brief Представляет время в виде json объекта.
     */
    QJsonObject toJson() const;

    /**
     * @brief Возвращает номер по порядку пары.
     */
    int number() const;
    /**
     * @brief Возвращает длительность пары.
     */
    int duration() const;

    bool operator==(const Time_& time) const;

private:
    /**
     * Вызывается, для инициализации объекта.
     */
    void init();

private:
    //! время начала
    QTime start_;
    //! время конца
    QTime end_;

    //! номер начала
    int number_;
    //! длительность
    int duration_;
};

#endif // TIME_H
