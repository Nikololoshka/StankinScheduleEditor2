#ifndef PARSEWORKERMANAGER_H
#define PARSEWORKERMANAGER_H

#include "stdafx.h"

/**
 * @brief ������������ � ���������� ����������� ��� ��������.
*/
enum class WorkerStatus {
    //! ������� ������
    Stay,
    //! ��������
    Running,
    //! ������ �� ����� ������
    Confused,
    //! ������ ������, ����� ���������� ������
    ConfuseSolved,
    //! �������� ��� ������
    Complete,
    //! ���� � ����������� �������
    Error
};

/**
 * @brief ��������� ��� �����������, �������� �� ������ �����������.
 * 
 * ������������ � ��������� �� �������� ��������. ���� ������ �� ���������
 * �����������, �� � data ����� ��������� ����������� ������� �������� � ����������� ��������.
*/
struct ValidData {
    //! ������
    QString data;
    //! ��������� �� ������ �����������
    bool valid;
};

/**
 * @brief ���������� ��� ���� ������� ������.
*/
struct ConfuseInfo {
    //! ����� �����������, ��� ������
    int id;
    //! ����� ������ � ���������� � �������
    int cellNumber;
    //! ��� ������
    QString type;
    //! ��������� �������
    QString maybe;
    //! ������
    QString confuse;
    //! ��������, ��� �������� ������
    QString context;
    //! ���� � ������������ ����������� � �����������
    QString imagePath;
};

/**
 * @brief ���������� � ������� ���������.
*/
struct ProgressData {
    //! ������� ��������
    int current;
    //! ������������ ��������
    int total;
};

/**
 * @brief �������� ��� ��������������� ������������ �� �������� ����������.
*/
class ParseWorkerManager
{
public:
    /**
     * @brief ����������� ���������.
     * @param workerCount ���������� ������������.
    */
    ParseWorkerManager(int workerCount = 0);

    /**
     * @brief ���������� ���� � ���������� ���������� ��� ��������.
     * 
     * ���� ���������� ������ ���, �� ������������ ������ ������.
    */
    QString nextSchedule();
    /**
     * @brief ���������� ������� ������� �������� ����������.
    */
    ProgressData currentProgress() const;

    /**
     * @brief ��������� �������� ���� �� ������������.
    */
    ValidData hasTitle(const QString &title) const;
    /**
     * @brief ��������� ������������� ���� �� ������������.
    */
    ValidData hasLecturer(const QString &lecturer) const;
    /**
     * @brief ��������� ��������� ���� �� ������������.
    */
    ValidData hasClassroom(const QString &classroom) const;

    /**
     * @brief ���������� ������ �� ����� ���������� ���.
    */
    const QVector<QString>& titles() const;
    /**
     * @brief ���������� ������ �� ����� ���������������.
    */
    const QVector<QString>& lecturers() const;
    /**
     * @brief ���������� ������ �� ����� �����������.
    */
    const QVector<QString>& classrooms() const;

    /**
     * @brief ��������� ������� ������ ��� �������� ������.
     * @param titles ������ � ����������� ���.
     * @param lecturers ������ � ���������������.
     * @param classrooms ������ � �����������.
    */
    void updateSets(QVector<QString> &titles,
                    QVector<QString> &lecturers,
                    QVector<QString> &classrooms);

    /**
     * @brief ���������� ����������� ��� ����������� �������� ������ � ������ �������.
    */
    const QMap<QString, QString>& transitions() const;
    /**
     * @brief ���������� �������� ������ ��� �������� � ������.
    */
    void transitionData(QString &data) const;
    /**
     * @brief ��������� ������� ����������� ��� ����������� �������� ������.
     * @param transitions ����� �����������.
    */
    void updateTransition(QMap<QString, QString> &transitions);

    /**
     * @brief ���������� ������ � ���������� ����������, ������� ������ ��������. 
    */
    QVector<QString> progressTexts() const;
    /**
     * @brief ���������� ������ � ���������� ������������, ������� ������ ������.
    */
    QVector<int> progressValues() const;
    /**
     * @brief ���������� ������ � ����������� ������������.
    */
    QVector<WorkerStatus> workerStatuses() const;
    /**
     * @brief ���������� ������ � �������� ������������.
    */
    QVector<ConfuseInfo> confuseInfo() const;

    /**
     * @brief ���������� ������ ����������� �����������.
     * @param id ����� �����������.
    */
    WorkerStatus status(int id) const;

    /**
     * @brief ���������� ������� �������� ��� ����������� �����������.
     * @param id ����� �����������.
    */
    QString solve(int id);
    /**
     * @brief ������������� ������� ��� ����������� �����������.
     * @param id ����� �����������.
     * @param data ������� ��������.
    */
    void setSolve(int id, const QString &data);

    /**
     * @brief ������������ ����������, ������� ����� �����������.
     * @param id ����� �����������.
     * @param scheduleName �������� ����������.
    */
    void setProgressText(int id, const QString &scheduleName);
    /**
     * @brief ������������ ������� �������� �����������.
     * @param id ����� �����������.
     * @param value ��������.
    */
    void setProgressValue(int id, int value);
    /**
     * @brief ������������� ������ �����������.
     * @param id ����� �����������.
     * @param status ������.
    */
    void setWorkerStatus(int id, const WorkerStatus status);
    /**
     * @brief �������������, ��� � ����������� ��������� ������.
     * @param id ����� �����������.
     * @param info ���������� �� ������.
    */
    void setConfuseInfo(int id, const ConfuseInfo &info);

    /**
     * @brief ������������� ������ ���������� ��� ��������.
    */
    void setSchedules(const QStringList &schedules);

    /**
     * @brief ���������� ���� �� poppler'�.
    */
    QString pooplerPath() const;
    /**
     * @brief ������������� ���� �� poppler'�.
    */
    void setPooplerPath(const QString &pooplerPath);

    /**
     * @brief ���������� ���� �� tesseract'�.
     * @deprecated ���� �� tesseract ��� �� ���������. TesseractWrapper �������� ��� ����.
    */
    QString tesseractPath() const;
    /**
     * @brief ������������� ���� �� tesseract'�.
     * @deprecated ���� �� tesseract ��� �� ���������. TesseractWrapper �������� ��� ����.
    */
    void setTesseractPath(const QString &tesseractPath);

    /**
     * @brief ���������� DPI ��� ����������� �����������.
    */
    int dpi() const;
    /**
     * @brief ������������� DPI ��� ����������� �����������.
    */
    void setDpi(int dpi);

private:
    /**
     * @brief ��������� ������ ��� ��������.
     * @param filePath ���� � �����.
     * @param data ����������� ������.
    */
    void readData(const QString &filePath, QVector<QString> &data) const;
    /**
     * @brief ��������� ����������� � �������������.
     * @param filePath ���� � �����.
     * @param data ����������� ������ ��� �����������.
    */
    void readData(const QString &filePath, QMap<QString, QString> &data) const;

    /**
     * @brief ��������� ������ ��� ��������.
     * @param filePath ���� ��� ����������.
     * @param data ������ ��� ��������.
    */
    void saveData(const QString &filePath, const QVector<QString> &data) const;
    /**
     * @brief �������� ������������ � �������������.
     * @param filePath ���� ��� ����������.
     * @param data ������ ��� �����������.
    */
    void saveData(const QString &filePath, const QMap<QString, QString> &data) const;

    /**
     * @brief ���������� ��� ������ � ���������� ������ �� ��������.
     * @param first ������ ������.
     * @param second ������ ������.
     * @return ����������� ��������. ��� ����� � 1, ��� ����� ������������.
    */
    double compare(const QString &first, const QString &second) const;
    /**
     * @brief ���������, ���� �� ���������� ������ � ������ ��������.
     * 
     * ��������� ���� ������ �������� �� ��������� ������������ ��������.
     * ���� ������ �� ���������, �� ��������� ��� �������� � ���������� ������.
     * � ���������� "���������" �������� �� ������ ������������ ��� "�� ����������",
     * �� ����������� ������������ � ������.
     * 
     * @param value ����������� ������.
     * @param values ������ ��������.
     * @return ������ �������� �� ������������.
    */
    ValidData hasValid(const QString &value, const QVector<QString> &values) const;

private:
    //! ���������� ������������
    int workerCount_;

    //! ������ � ����������� ���������� ���
    QVector<QString> titles_;
    //! ������ � ����������� ��������������� ���
    QVector<QString> lecturers_;
    //! ������ � ����������� �����������
    QVector<QString> classrooms_;

    //! ����������� ��� ����������� �������� ������
    QMap<QString, QString> transitions_;

    //! ����������� � �������� ������������, ������� ������ �����������
    QMap<int, QString> progressText_;
    //! ����������� � ���������� ������������
    QMap<int, int> progressValue_;
    //! ����������� � �������� ������������
    QMap<int, WorkerStatus> workerStatuses_;
    //! ����������� � �������� ������������
    QMap<int, ConfuseInfo> confuseInfo_;

    //! ����������� � �������� ������ ��� ������������
    QMap<int, QString> solves_;

    //! ������ � ������������ ��� ��������
    QStringList schedules_;
    //! ���������� ����������
    int currentCount_;
    //! ����� ����������
    int totalCount_;

    //! ���� � poopler'�
    QString pooplerPath_;
    //! �������� dpi ��� ����������� �����������
    int dpi_;

    //! ������ �������������
    mutable std::mutex mutex_;
    //! ������ ������������� (�� ������)
    mutable std::shared_mutex sharedMutex_;
};

#endif // PARSEWORKERMANAGER_H
