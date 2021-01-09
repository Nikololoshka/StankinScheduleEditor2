#ifndef SCHEDULEDAY_H
#define SCHEDULEDAY_H

#include "stdafx.h"
#include "pair/pair.h"


/**
 * Псевдоним для ячейки в дне расписания.
 */
using Cell = QVector<unsigned int>;

/**
 * @brief Визуальная ячейка расписания.
 */
struct ScheduleCell {
    //! отображаемый текст.
    QString text;
    //! занимаемое количество строк (визуально).
    int rowSpan;
    //! занимаемое количество столбцов (визуально).
    int columnSpan;

    /**
     * @brief Проверяет, занимает ли ячека больше чем 1 место в расписании (визуально).
     */
    bool isSpanValid() const;
};

/**
 * @brief Индекс ячейки в расписании
 */
struct ScheduleIndex {
    //! строка в расписании (день недели).
    int row;
    //! номер начала ячейки (номер столбца).
    int number;
    //! внутренняя строка (строка в строке дня недели).
    int innerRow;

    /**
     * @brief Возвращает текстовое представление индекса.
     */
    QString toString() const;
};

struct ScheduleDrawingCell {
    int row;
    int column;
    ScheduleCell cell;
};

/**
 * @brief День с парами в расписании.
 */
class ScheduleDay {

public:
    ScheduleDay();

    /**
     * @brief Возвращает json объект дня.
     */
    QJsonArray toJson() const;

    /**
     * @brief Добавляет пару ко дню расписания.
     */
    void add(const Pair& pair);
    /**
     * @brief Удаляет пару из дня расписания.
     */
    void remove(const Pair& pair);

    /**
     * @brief Возвращает количество пар в дне.
     */
    int size() const;
    /**
     * @brief Возвращает количество визуальных строк.
     */
    int row() const;
    /**
     * @brief Возвращает количество визуальных столбцов.
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

    /**
     * @brief Возвращает объект ячейки расписания для визуального представления.
     * @param index индекс ячейки расписания.
     */
    ScheduleCell pairsTextByIndex(const ScheduleIndex& index) const;
    /**
     * @brief Возвращает список пар в ячейки расписания.
     * @param index индекс ячейки расписания.
     */
    std::vector<Pair> pairsByIndex(const ScheduleIndex& index) const;
    /**
     * @brief Возвращает список пар из ячейки в дне расписания.
     */
    std::vector<Pair> fromCell(const Cell& cell) const;

    QVector<ScheduleDrawingCell> pairsForDrawing() const;

private:
    /**
     * @brief Проверяет, можно ли добавить пару в день.
     */
    void isAddCheck(const Pair& pair) const;
    /**
     * @brief Перераспределяет пары в дне для красивого и
     *        правильного визуального представления.
     */
    void reallocate();
    /**
     * @brief Ищет номер по порядку пары.
     * @param id ID пары.
     */
    int find(unsigned int id) const;
    /**
     * @brief Вычисляет занимаемое место ячейки в строках.
     * @param duration продолжительность пары (длина ячейки).
     * @param index индекс ячейки.
     */
    int computeRowSpan(int duration, const ScheduleIndex& index) const;

    bool isMerge(const std::vector<Pair> &pairs, const Pair &pair) const;

private:
    /**
     * @brief Вспомогательная структура для хранения пары.
     */
    struct PairData {
        //! ID пары.
        unsigned int id;
        //! пара
        Pair pair;
    };

    //! пул с ID для пар
    unsigned int index_;
    //! список с строками в дне расписания
    QVector<QVector<Cell>> rows_;
    //! список с парами.
    std::vector<PairData> pairs_;
};

#endif // SCHEDULEDAY_H
