#ifndef DATE_H
#define DATE_H

#include "stdafx.h"
#include "dateItem.h"

/**
 * @brief Дата пары.
 */
class Date {

public:
    Date();
    Date(const Date& date);
    Date(Date&& date);

    Date& operator=(const Date& date);
    const std::unique_ptr<DateItem>& operator[](int index) const;
    bool operator==(const Date& date) const;

    /**
     * @brief Возвращает json объекты даты пары.
     */
    QJsonArray toJson() const;

    /**
     * @brief Возвращает копию даты из дат пары.
     * @param index индекс даты.
     */
    std::unique_ptr<DateItem> copy(int index) const;

    /**
     * @brief Добавляет новую дату в даты пары.
     * @param item новая дата.
     * @throw InvalidAddDateException если дата пересекается с текущими датами пары.
     * @throw std::invalid_argument если день недели даты не совпадает с датами пары.
     */
    void addDate(const std::unique_ptr<DateItem>& item);
    /**
     * @brief Удаляет дату из дат пары.
     * @param item удаляемая дата.
     */
    void removeDate(const std::unique_ptr<DateItem>& item);

    /**
     * @brief Возвращает количество элементов дат в дате пары.
     */
    int size() const;
    /**
     * @brief Определяет, есть или нет дат в дате пары.
     * Возвращает true если нет, иначе false.
     */
    bool isEmpty() const;
    /**
     * @brief Заменяет одну дату на другую в датах пары.
     * @param oldDate заменяемая дата.
     * @param newDate новоя дата.
     * @throws исключения эквивалентны Date::addDate
     */
    void changeDate(const std::unique_ptr<DateItem>& oldDate,
        const std::unique_ptr<DateItem>& newDate);
    /**
     * @brief Проверяет, можно ли заменить одну дату на другую.
     * @param oldDate заменяемая дата (допускается nullptr).
     * @param newDate новая дата.
     * @throws исключения эквивалентны Date::addDate
     */
    void possibleChange(const std::unique_ptr<DateItem>& oldDate,
        const std::unique_ptr<DateItem>& newDate) const;

    /**
     * @brief Проверяет, пересекается ли дата с датами пары.
     */
    bool contains(const std::unique_ptr<DateItem>& item) const;
    /**
     * @brief Проверяет, пересекаются ли даты пары между собой.
     */
    bool contains(const Date& date) const;

    /**
     * @brief Определяет, идет ли по порядку одни даты раньще другой.
     */
    bool before(const Date& date) const;

    /**
     * @brief Возвращает день недели даты пары.
     */
    DayOfWeek dayOfWeek() const;
    /**
     * @brief Возвращает строковое представление даты пары.
     */
    QString toString() const;

private:
    //! вектор дат
    std::vector<std::unique_ptr<DateItem>> dates_;
};

#endif // DATE_H
