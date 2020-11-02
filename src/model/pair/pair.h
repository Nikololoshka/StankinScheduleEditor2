#ifndef PAIR_H
#define PAIR_H

#include "stdafx.h"

#include "date.h"
#include "subgroup.h"
#include "time_.h"
#include "type.h"

/**
 * @brief Объект пары в расписании.
 */
class Pair {

public:
    /**
     * @brief Возвращает пару из json объекта.
     */
    static Pair fromJson(const QJsonObject& value);

    /**
     * @brief Конструктор пары.
     * @param title название дисциплины.
     * @param lecturer преподаватель.
     * @param classroom аудитория.
     * @param type тип.
     * @param subgroup подгруппа.
     * @param time время.
     * @param date даты.
     */
    Pair(const QString& title, const QString& lecturer,
        const QString& classroom, const Type& type,
        const Subgroup& subgroup, const Time_& time,
        const Date& date = Date());
    Pair(const Pair& pair);
    Pair(Pair&& pair);

    /**
     * @brief Возвращает json объект пары.
     */
    QJsonObject toJson() const;

    Pair& operator=(const Pair& pair);
    bool operator==(const Pair& pair) const;

    /**
     * @brief Возвращает дисциплину пары.
     */
    QString title() const;
    /**
     * @brief Устанавливает дисциплину пары.
     */
    void setTitle(const QString& title);

    /**
     * @brief Возвращает преподавателя пары.
     */
    QString lecturer() const;
    /**
     * @brief Устанавливает преподавателя пары.
     */
    void setLecturer(const QString& lecturer);

    /**
     * @brief Возвращает аудиторию пары.
     */
    QString classroom() const;
    /**
     * @brief Устанавливает аудиторию пары.
     */
    void setClassroom(const QString& classroom);

    /**
     * @brief Возвращает тип пары.
     */
    Type type() const;
    /**
     * @brief Устанавливает тип пары.
     */
    void setType(const Type& type);

    /**
     * @brief Возвращает подгруппу пары.
     */
    Subgroup subgroup() const;
    /**
     * @brief Устанавливает подгруппу пары.
     */
    void setSubgroup(const Subgroup& subgroup);

    /**
     * @brief Возвращает время пары.
     */
    Time_ time() const;
    /**
     * @brief Устанавливает время пары.
     */
    void setTime(const Time_& time);

    /**
     * @brief Возвращает даты пары.
     */
    Date date() const;
    /**
     * @brief Устанавливает даты пары.
     */
    void setDate(const Date& date);

    /**
     * @brief Возвращает копию пары.
     */
    std::unique_ptr<Pair> copy() const;

    /**
     * @brief Добавляет дату к датам пары.
     */
    void addDate(const std::unique_ptr<DateItem>& item);
    /**
     * @brief Удаляет дату из дат пары.
     */
    void removeDate(const std::unique_ptr<DateItem>& item);

    /**
     * @brief Определяет, идет ли одна пары раньше другой.
     */
    bool before(const Pair& pair) const;
    /**
     * @brief Определяет, пересекается ли дата с датами пары.
     */
    bool contains(const std::unique_ptr<DateItem>& item) const;

    /**
     * @brief Возвращает день недели пары.
     */
    DayOfWeek dayOfWeek() const;
    /**
     * @brief Определяет, пересекаются ли пары по времени и дате.
     * @param pair другая пара.
     */
    bool intersect(const Pair& pair) const;
    /**
     * @brief Определяет, возможно ли одновременное
     *        существование пар с установленными подгруппами.
     */
    bool separate(const Pair& pair) const;

    /**
     * @brief Возвращает строку соответсвующей пары.
     */
    QString toString() const;

private:
    //! дисуиплина
    QString title_;
    //! преподаватель
    QString lecturer_;
    //! аудитория
    QString classroom_;
    //! тип
    Type type_;
    //! подгруппа
    Subgroup subgroup_;
    //! время
    Time_ time_;
    //! даты
    Date date_;
};

#endif // PAIR_H
