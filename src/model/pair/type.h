#ifndef TYPE_H
#define TYPE_H

#include "stdafx.h"

/**
 * @brief Тип пары.
 */
class Type {

public:
    /**
     * @brief Возвращает тип пары из строки.
     * @param value строка с парой.
     */
    static Type fromString(const QString& value);
    /**
     * @brief Список всех возможных типов пары.
     */
    static std::vector<Type> list();

    /**
     * @brief Возвращает тип лекции.
     */
    static Type getLecture();
    /**
     * @brief Возвращает тип семинара.
     */
    static Type getSeminar();
    /**
     * @brief Возвращает тип лабораторной.
     */
    static Type getLaboratory();

    /**
     * @brief Сравнивает типы на равенство.
     * @param type другой тип пары.
     */
    bool operator==(const Type& type) const;

    /**
     * @brief Возвращает тэг типа в json файле.
     */
    QString tag() const;
    /**
     * @brief Возвращает человеко-читаемый текстовое представление типа.
     */
    QString text() const;

private:
    Type(const QString& tag, const QString& text);

private:
    //! Тэг типа в json файле.
    QString tag_;
    //! Человеко-читаемый текстовое представление типа.
    QString text_;
};

#endif // TYPE_H
