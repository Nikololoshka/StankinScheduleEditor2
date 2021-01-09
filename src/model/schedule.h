#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "stdafx.h"

#include "pair/pair.h"
#include "scheduleDay.h"

/**
 * @brief Объект расписания.
 */
class Schedule {

public:
    /**
     * @brief Конструктор для создания пустого расписания.
     */
    Schedule();
    /**
     * @brief Конструктор для создания расписания из json документа.
     */
    Schedule(const QJsonDocument& value);

    /**
     * @brief Возвращает json объект расписания.
     */
    QJsonArray toJson() const;

    /**
     * @brief Добавляет пару в расписание.
     */
    void addPair(const Pair& pair);
    /**
     * @brief Удаляет пару из расписания.
     */
    void remove(const Pair& pair);

    /**
     * @brief Возвращает список индексов (строк) расписания по дням.
     *
     * Некоторые дни расписания могут быть представлены в несколько строк.
     * Такова визуальная структура расписания.
     *
     * Если в день 0 (понедельник) 2 строки расписания, то в
     * массиве indexes[0] == 2.
     *
     * Всего список состоит из 6 элементов (дней недели по порядку).
     */
    QVector<int> indexes() const;
    /**
     * @brief Общие количество строк (визуальных) расписания.
     */
    int row() const;
    /**
     * @brief Количество столбцов (визуальных) в расписании.
     */
    int column() const;

    /**
     * @brief Заменяет одну пару на другую в расписании.
     * @param oldPair заменяемая пара.
     * @param newPair новая пары.
     */
    void changePair(const std::optional<Pair>& oldPair,
        const std::optional<Pair>& newPair);
    /**
     * @brief Проверяет, возможно ли заменить одну пару на другую в расписании.
     * @param oldPair заменяемая пара.
     * @param newPair новая пары.
     * @throw std::logic_error если невозможно заменить пару.
     */
    void possibleChange(const std::optional<Pair>& oldPair,
        const std::optional<Pair>& newPair) const;

    QVector<ScheduleDrawingCell> pairsForDrawingByDay(const DayOfWeek &dayOfWeek) const;

    /**
     * @brief Возвращает объект ячейки расписания для визуального представления.
     * @param index индекс ячейки расписания.
     */
    ScheduleCell pairsTextByIndex(const ScheduleIndex& index) const;
    /**
     * @brief Возвращает объект ячейки расписания для визуального представления.
     * @param row строка (визуальная) ячейки.
     * @param column строка (визуальная) ячейки.
     */
    ScheduleCell pairsTextByIndex(int row, int column) const;
    /**
     * @brief Возвращает список пар в ячейки расписания.
     * @param index индекс ячейки расписания.
     */
    std::vector<Pair> pairsByIndex(const ScheduleIndex& index) const;

    /**
     * @brief Переводит визуальные координаты ячейки в индекс.
     * @param row строка (визуальная) ячейки.
     * @param column строка (визуальная) ячейки.
     */
    ScheduleIndex transform(int row, int column) const;
    /**
     * @brief Выводит содержимое расписание в stdout.
     */
    void print();

private:
    //! коллекция дней расписания
    QMap<DayOfWeek, ScheduleDay> days_;
    //! список (буфер) всех пар расписания
    std::vector<Pair> pairs_;
};

#endif // SCHEDULE_H
