#ifndef PARSEWORKERMANAGER_H
#define PARSEWORKERMANAGER_H

#include "stdafx.h"

/**
 * @brief Перечисление с состоянием исполнителя для парсинга.
*/
enum class WorkerStatus {
    //! ожидает начала
    Stay,
    //! работает
    Running,
    //! ошибка во время работы
    Confused,
    //! ошибка решена, можно продолжить работу
    ConfuseSolved,
    //! завершмл все задачи
    Complete,
    //! упал с неизвестной ошибкой
    Error
};

/**
 * @brief Структура для определения, являются ли данные правильными.
 * 
 * Используется в сравнении со списками проверки. Если данные не являються
 * правильными, то в data будет храниться максимально похожее значение к правильному значению.
*/
struct ValidData {
    //! данные
    QString data;
    //! являються ли данные правильными
    bool valid;
};

/**
 * @brief Информация для окна решения ошибок.
*/
struct ConfuseInfo {
    //! номер исполнителя, где ошибка
    int id;
    //! номер ячейки в расписании с ошибкой
    int cellNumber;
    //! тип ошибки
    QString type;
    //! возможное решение
    QString maybe;
    //! ошибка
    QString confuse;
    //! контекст, где возникла ошибка
    QString context;
    //! путь к размеченному изображению с расписанием
    QString imagePath;
};

/**
 * @brief Информация о текущем прогрессе.
*/
struct ProgressData {
    //! текущие значение
    int current;
    //! максимальное значение
    int total;
};

/**
 * @brief Менеджер для контролирования исполнителей по парсингу расписаний.
*/
class ParseWorkerManager
{
public:
    /**
     * @brief Конструктор менеджера.
     * @param workerCount количество исполнителей.
    */
    ParseWorkerManager(int workerCount = 0);

    /**
     * @brief Возвращает путь к следующему расписанию для парсинга.
     * 
     * Если расписаний больше нет, то возвращается пустая строка.
    */
    QString nextSchedule();
    /**
     * @brief Возвращает текущий процесс парсинга расписаний.
    */
    ProgressData currentProgress() const;

    /**
     * @brief Проверяет название пары на правильность.
    */
    ValidData hasTitle(const QString &title) const;
    /**
     * @brief Проверяет преподавателя пары на правильность.
    */
    ValidData hasLecturer(const QString &lecturer) const;
    /**
     * @brief Проверяет аудиторию пары на правильность.
    */
    ValidData hasClassroom(const QString &classroom) const;

    /**
     * @brief Возвращает список со всеми названиями пар.
    */
    const QVector<QString>& titles() const;
    /**
     * @brief Возвращает список со всеми преподавателями.
    */
    const QVector<QString>& lecturers() const;
    /**
     * @brief Возвращает список со всеми аудиториями.
    */
    const QVector<QString>& classrooms() const;

    /**
     * @brief Обновляет текущие списки для проверки данных.
     * @param titles список с названиямии пар.
     * @param lecturers список с преподавателями.
     * @param classrooms список с аудиториями.
    */
    void updateSets(QVector<QString> &titles,
                    QVector<QString> &lecturers,
                    QVector<QString> &classrooms);

    /**
     * @brief Возвращает отображение для исправления типичных ошибок в работе парсера.
    */
    const QMap<QString, QString>& transitions() const;
    /**
     * @brief Исправляет типичные ошибки при парсинге в тексте.
    */
    void transitionData(QString &data) const;
    /**
     * @brief Обновляет текущие отображение для исправления типичных ошибок.
     * @param transitions новое отображение.
    */
    void updateTransition(QMap<QString, QString> &transitions);

    /**
     * @brief Возвращает список с названиями расписаний, которые сейчас парсятся. 
    */
    QVector<QString> progressTexts() const;
    /**
     * @brief Возвращает список с прогрессом исполнителей, которые сейчас парсят.
    */
    QVector<int> progressValues() const;
    /**
     * @brief Возвращает список с состояниями исполнителей.
    */
    QVector<WorkerStatus> workerStatuses() const;
    /**
     * @brief Возвращает список с ошибками исполнителей.
    */
    QVector<ConfuseInfo> confuseInfo() const;

    /**
     * @brief Возвращает стутус конкретного исполнителя.
     * @param id номер исполнителя.
    */
    WorkerStatus status(int id) const;

    /**
     * @brief Возвращает решение проблемы для конкретного исполнителя.
     * @param id номер исполнителя.
    */
    QString solve(int id);
    /**
     * @brief Устанавливает решения для конкретного исполнителя.
     * @param id номер исполнителя.
     * @param data решение проблемы.
    */
    void setSolve(int id, const QString &data);

    /**
     * @brief Устанвливает расписание, которым занят исполнитель.
     * @param id номер исполнителя.
     * @param scheduleName название расписания.
    */
    void setProgressText(int id, const QString &scheduleName);
    /**
     * @brief Устанвливает текущий прогресс исполнителя.
     * @param id номер исполнителя.
     * @param value прогресс.
    */
    void setProgressValue(int id, int value);
    /**
     * @brief Устанавливает стутус исполнителя.
     * @param id номер исполнителя.
     * @param status статус.
    */
    void setWorkerStatus(int id, const WorkerStatus status);
    /**
     * @brief Устанавливает, что у исполнителя произошла ошибка.
     * @param id номер исполнителя.
     * @param info информация об ошибке.
    */
    void setConfuseInfo(int id, const ConfuseInfo &info);

    /**
     * @brief Устанавливает список расписаний для парсинга.
    */
    void setSchedules(const QStringList &schedules);

    /**
     * @brief Возвращает путь до poppler'а.
    */
    QString pooplerPath() const;
    /**
     * @brief Устанавливает путь до poppler'а.
    */
    void setPooplerPath(const QString &pooplerPath);

    /**
     * @brief Возвращает путь до tesseract'а.
     * @deprecated Путь до tesseract уже не трубуется. TesseractWrapper работает без него.
    */
    QString tesseractPath() const;
    /**
     * @brief Устанавливает путь до tesseract'а.
     * @deprecated Путь до tesseract уже не трубуется. TesseractWrapper работает без него.
    */
    void setTesseractPath(const QString &tesseractPath);

    /**
     * @brief Возвращает DPI для конвертации изображения.
    */
    int dpi() const;
    /**
     * @brief Устанавливает DPI для конвертации изображения.
    */
    void setDpi(int dpi);

    /**
     * @brief Возвращает год даты расписания.
    */
    int year() const;
    /**
     * @brief Устанавливает год даты расписания.
    */
    void setYear(int year);

private:
    /**
     * @brief Загружает список для проверки.
     * @param filePath путь к файлу.
     * @param data загружаемый список.
    */
    void readData(const QString &filePath, QVector<QString> &data) const;
    /**
     * @brief Загружает отображание с исправлениями.
     * @param filePath путь к файлу.
     * @param data загруженные данные для исправления.
    */
    void readData(const QString &filePath, QMap<QString, QString> &data) const;

    /**
     * @brief Сохраняет список для проверки.
     * @param filePath путь для сохранения.
     * @param data список для проверки.
    */
    void saveData(const QString &filePath, const QVector<QString> &data) const;
    /**
     * @brief Сохраняе тотображание с исправлениями.
     * @param filePath путь для сохранения.
     * @param data данные для исправления.
    */
    void saveData(const QString &filePath, const QMap<QString, QString> &data) const;

    /**
     * @brief Сравнивает две строки и возвращает оценку их схожести.
     * @param first первая строка.
     * @param second вторая строка.
     * @return коэффициент схожести. Чем ближе к 1, тем более эквивалентны.
    */
    double compare(const QString &first, const QString &second) const;
    /**
     * @brief Проверяет, есть ли правильные данные в списке проверки.
     * 
     * Проверяет весь список проверки на вхождение проверяемого значения.
     * Если такого не оказалось, то вычисляет его схожесть с элементами списка.
     * В последстие "возможное" значение из списка возвращается как "не правильное",
     * но максимально приближенное к истине.
     * 
     * @param value проверяемая строка.
     * @param values список проверки.
     * @return объект проверки на правильность.
    */
    ValidData hasValid(const QString &value, const QVector<QString> &values) const;

private:
    //! количество исполнителей
    int workerCount_;

    //! список с правильными названиями пар
    QVector<QString> titles_;
    //! список с правильными преподавателями пар
    QVector<QString> lecturers_;
    //! список с правильными аудиториями
    QVector<QString> classrooms_;

    //! отображение для исправления типичных ошибок
    QMap<QString, QString> transitions_;

    //! отображение с текущими расписаниями, которые парсят исполнители
    QMap<int, QString> progressText_;
    //! отображение с прогрессом исполнителей
    QMap<int, int> progressValue_;
    //! отображение с статусом исполнителей
    QMap<int, WorkerStatus> workerStatuses_;
    //! отображение с ошибками исполнителей
    QMap<int, ConfuseInfo> confuseInfo_;

    //! отображение с решением ошибок для исполнителей
    QMap<int, QString> solves_;

    //! список с расписаниями для парсинга
    QStringList schedules_;
    //! расписаний распарщено
    int currentCount_;
    //! всего расписаний
    int totalCount_;

    //! год даты расписания
    int year_;
    //! значение dpi для конвертации изображения
    int dpi_;

    //! объект синхранизации
    mutable std::mutex mutex_;
    //! объект синхранизации (на чтение)
    mutable std::shared_mutex sharedMutex_;
};

#endif // PARSEWORKERMANAGER_H
