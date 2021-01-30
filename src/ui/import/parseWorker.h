#ifndef PARSEWORKER_H
#define PARSEWORKER_H

#include "stdafx.h"
#include "pair/pair.h"

#include "parseFileException.h"
#include "parseWorkerManager.h"
#include "popplerWrapper.h"
#include "tesseractWrapper.h"

/**
 * @brief Класс ячейки, которая была распознана в расписании.
*/
struct ParseCell {
    //! текст ячейки
    QString text;
    //! номер ячейки
    int number;
    //! начало ячейки (по времени пары)
    int start;
    //! конец ячейки (по времени пары)
    int end;
};

/**
 * @brief Класс исполнителя, который будет парсить расписание.
*/
class ParseWorker : public QRunnable {
public:
    /**
     * @brief Конструктор класса парсера.
     * @param workerManager менеджер для обмена данными.
     * @param id номер парсера.
    */
    ParseWorker(const QSharedPointer<ParseWorkerManager>& workerManager, int id);

    void run() override;

    //! Паттерн регулярного выражения названия
    const static QString PATTERN_TITLE;
    //! Паттерн регулярного выражения преподавателя
    const static QString PATTERN_LECTURER;
    //! Паттерн регулярного выражения типа
    const static QString PATTERN_TYPE;
    //! Паттерн регулярного выражения подгруппы
    const static QString PATTERN_SUBGROUP;
    //! Паттерн регулярного выражения аудитории
    const static QString PATTERN_CLASSROOM;
    //! Паттерн регулярного выражения даты
    const static QString PATTERN_DATE;
    //! Паттерн регулярного выражения диапозона даты
    const static QString PATTERN_DATE_RANGE;
    //! Паттерн регулярного выражения одиночной даты
    const static QString PATTERN_DATE_SINGLE;
    //! Паттерн регулярного выражения разделителя пар
    const static QString PATTERN_DIVIDER;

    /**
     * @brief Возвращает общий паттерн регулярного выражения.
    */
    static QString patternCommon();

private:
    /**
     * @brief Начинает парсинг заданного расписания.
     * @param pdfFilePath путь к PDF файлу расписания.
     * @param poopler интерфейс для работы с poopler.
     * @param tesseract интерфейс для работы с tesseract.
    */
    void startParsing(const QString& pdfFilePath,
        std::unique_ptr<PopplerWrapper>& poopler,
        std::unique_ptr<TesseractWrapper>& tesseract);

    /**
     * @brief Начинает парсить пары из текста.
     * @param data текст для парсинга.
     * @param cell исходная ячейка, откуда беруться данные.
     * @param timeCells ячейки с временем (для определения начала и продолжительности пары).
     * @return список распознаных пар.
    */
    std::vector<Pair> parsePairs(const QString& data,
        const ParseCell& cell,
        const QMap<int, ParseCell>& timeCells);

    /**
     * @brief Разделяет пары в тексте, каждую на новой строке.
     * @param text текст для разделения.
     * @return текст с разделенными парами
    */
    QString separateByLines(const QString& text) const;

    /**
     * @brief Цикл для обработки ошибок во времени работы парсера.
     * @param e ошибка парсера.
     * @param data текст, в распознавании которого произошла ошибка.
     * @param number номер ячейки в расписании.
     * @param imagePath путь к размеченному изображению расписания.
     * @return возвращает текст с исправленной ошибкой. 
    */
    QString confuseLoop(ParseFileException& e,
        const QString& data,
        int number,
        const QString& imagePath) const;

    /**
     * @brief Парсит название предмета из строки.
     * @param titleMatch текст с названием.
     * @return название предмета.
    */
    QString parseTitle(const QString& titleMatch) const;
    /**
     * @brief Парсит преподавателя из строки.
     * @param lecturerMatch текст с преподавателем.
     * @return преподавателя.
    */
    QString parseLecturer(const QString& lecturerMatch) const;
    /**
     * @brief Парсит тип пары.
     * @param typeMatch текст с типом пары.
     * @return тип пары.
    */
    Type parseType(const QString& typeMatch) const;
    /**
     * @brief Парсит подгруппу пары.
     * @param subgroupMatch текст с подгруппой.
     * @return подгруппа пары.
    */
    Subgroup parseSubgroup(const QString& subgroupMatch) const;
    /**
     * @brief Парсит аудиторию пары.
     * @param classroomMatch текст с аудиторией.
     * @return аудитория.
    */
    QString parseClassroom(const QString& classroomMatch) const;
    /**
     * @brief Парсит дату пары.
     * @param datesMatch текст с датой пары.
     * @return дата пары.
    */
    Date parseDates(const QString& datesMatch) const;
    /**
     * @brief Парсит текст с датой в класс даты.
     * @param dateString текст с датой.
     * @return объект даты.
    */
    QDate parseDate(const QString& dateString) const;

    /**
     * @brief Вычисляет время пары.
     * @param timeCells ячейки с временем в расписании.
     * @param cell исходная ячейка.
     * @return время пары.
    */
    Time_ computeTime(const QMap<int, ParseCell>& timeCells, const ParseCell& cell) const;

private:
    //! номер исполнителя
    int id_;
    //! менеджер для обмена данными.
    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // PARSEWORKER_H
