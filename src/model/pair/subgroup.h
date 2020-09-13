#ifndef SUBGROUP_H
#define SUBGROUP_H

#include <QtWidgets>

/**
 * @brief Подгруппа пары.
 */
class Subgroup {

public:
    /**
     * @brief Возвращает подгруппу из строки.
     * @param value строка с тэгом подгруппы.
     * @throw std::invalid_argument если не удалось определить подгруппу.
     */
    static Subgroup fromString(const QString& value);
    /**
     * @brief Возвращает список возможных подгрупп.
     */
    static std::vector<Subgroup> list();

    /**
     * @brief Возвращает подгруппу "А".
     */
    static Subgroup getA();
    /**
     * @brief Возвращает подгруппу "Б".
     */
    static Subgroup getB();
    /**
     * @brief Возвращает подгруппу "Общую".
     * Обозначает отсутсвие подгруппы. Такая пара будет являться
     * обязательной для "А" и "Б" подгрупп.
     */
    static Subgroup getCommon();

    /**
     * @brief Возвращает true, если подгруппа отображается в расписании, иначе false.
     */
    bool isShow() const;
    /**
     * @brief Определяет, возможно ли существование пар у обоих погрупп одновременно.
     * @param subgroup другая подгруппа.
     */
    bool separate(const Subgroup& subgroup) const;

    /**
     * @brief Возвращает тэг подгруппы.
     */
    QString tag() const;
    /**
     * @brief Возвращает человеко-читаемый текст подгруппы.
     */
    QString text() const;

    bool operator==(const Subgroup& subgroup) const;
    bool operator!=(const Subgroup& subgroup) const;

private:
    Subgroup(QString tag, QString text);

private:
    //! тэг подгруппы.
    QString tag_;
    //! читаемый текст подгруппы.
    QString text_;
};

#endif // SUBGROUP_H
