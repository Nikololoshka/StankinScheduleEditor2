#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "stdafx.h"

/**
 * @brief Периодичность даты.
 */
class Frequency {

public:
    /**
     * @brief Возвращает периодичность даты из строки.
     * @param value строка с периодичностью.
     * @throw std::invalid_argument если не удалось определить перидичность.
     */
    static Frequency fromString(const QString& value);
    /**
     * @brief Возвращает список возможных периодичностей даты.
     */
    static std::vector<Frequency> list();

    /**
     * @brief Возвращает "единождую" периодичность.
     */
    static Frequency getOnce();
    /**
     * @brief Возвращает "каждую недели" периодичность.
     */
    static Frequency getEvery();
    /**
     * @brief Возвращает "через неделю" периодичность.
     */
    static Frequency getThroughout();

    /**
     * @brief Возвращает разницу в днях между датами.
     * Если:
     *     "единождны" - 1;
     *     "каждую н." - 7;
     *     "через н." - 14.
     */
    int delta() const;
    /**
     * @brief Возвращает тэг периодичности.
     */
    QString tag() const;
    /**
     * @brief Возвращает человеко-читаемый текст периодичности.
     */
    QString text() const;

    bool operator==(const Frequency& other) const;

private:
    Frequency(const int delta, const QString& tag, const QString& text);

private:
    //! разница между дней
    int delta_;
    //! тэг
    QString tag_;
    //! читаемый текст
    QString text_;
};

#endif // FREQUENCY_H
