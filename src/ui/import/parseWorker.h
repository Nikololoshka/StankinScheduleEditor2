#ifndef PARSEWORKER_H
#define PARSEWORKER_H

#include "stdafx.h"
#include "pair/pair.h"

#include "parseFileException.h"
#include "parseWorkerManager.h"
#include "popplerWrapper.h"
#include "tesseractWrapper.h"

/**
 * @brief ����� ������, ������� ���� ���������� � ����������.
*/
struct ParseCell {
    //! ����� ������
    QString text;
    //! ����� ������
    int number;
    //! ������ ������ (�� ������� ����)
    int start;
    //! ����� ������ (�� ������� ����)
    int end;
};

/**
 * @brief ����� �����������, ������� ����� ������� ����������.
*/
class ParseWorker : public QRunnable {
public:
    /**
     * @brief ����������� ������ �������.
     * @param workerManager �������� ��� ������ �������.
     * @param id ����� �������.
    */
    ParseWorker(const QSharedPointer<ParseWorkerManager>& workerManager, int id);

    void run() override;

    //! ������� ����������� ��������� ��������
    const static QString PATTERN_TITLE;
    //! ������� ����������� ��������� �������������
    const static QString PATTERN_LECTURER;
    //! ������� ����������� ��������� ����
    const static QString PATTERN_TYPE;
    //! ������� ����������� ��������� ���������
    const static QString PATTERN_SUBGROUP;
    //! ������� ����������� ��������� ���������
    const static QString PATTERN_CLASSROOM;
    //! ������� ����������� ��������� ����
    const static QString PATTERN_DATE;
    //! ������� ����������� ��������� ��������� ����
    const static QString PATTERN_DATE_RANGE;
    //! ������� ����������� ��������� ��������� ����
    const static QString PATTERN_DATE_SINGLE;
    //! ������� ����������� ��������� ����������� ���
    const static QString PATTERN_DIVIDER;

    /**
     * @brief ���������� ����� ������� ����������� ���������.
    */
    static QString patternCommon();

private:
    /**
     * @brief �������� ������� ��������� ����������.
     * @param pdfFilePath ���� � PDF ����� ����������.
     * @param poopler ��������� ��� ������ � poopler.
     * @param tesseract ��������� ��� ������ � tesseract.
    */
    void startParsing(const QString& pdfFilePath,
        std::unique_ptr<PopplerWrapper>& poopler,
        std::unique_ptr<TesseractWrapper>& tesseract);

    /**
     * @brief �������� ������� ���� �� ������.
     * @param data ����� ��� ��������.
     * @param cell �������� ������, ������ �������� ������.
     * @param timeCells ������ � �������� (��� ����������� ������ � ����������������� ����).
     * @return ������ ����������� ���.
    */
    std::vector<Pair> parsePairs(const QString& data,
        const ParseCell& cell,
        const QMap<int, ParseCell>& timeCells);

    /**
     * @brief ��������� ���� � ������, ������ �� ����� ������.
     * @param text ����� ��� ����������.
     * @return ����� � ������������ ������
    */
    QString separateByLines(const QString& text) const;

    /**
     * @brief ���� ��� ��������� ������ �� ������� ������ �������.
     * @param e ������ �������.
     * @param data �����, � ������������� �������� ��������� ������.
     * @param number ����� ������ � ����������.
     * @param imagePath ���� � ������������ ����������� ����������.
     * @return ���������� ����� � ������������ �������. 
    */
    QString confuseLoop(ParseFileException& e,
        const QString& data,
        int number,
        const QString& imagePath) const;

    /**
     * @brief ������ �������� �������� �� ������.
     * @param titleMatch ����� � ���������.
     * @return �������� ��������.
    */
    QString parseTitle(const QString& titleMatch) const;
    /**
     * @brief ������ ������������� �� ������.
     * @param lecturerMatch ����� � ��������������.
     * @return �������������.
    */
    QString parseLecturer(const QString& lecturerMatch) const;
    /**
     * @brief ������ ��� ����.
     * @param typeMatch ����� � ����� ����.
     * @return ��� ����.
    */
    Type parseType(const QString& typeMatch) const;
    /**
     * @brief ������ ��������� ����.
     * @param subgroupMatch ����� � ����������.
     * @return ��������� ����.
    */
    Subgroup parseSubgroup(const QString& subgroupMatch) const;
    /**
     * @brief ������ ��������� ����.
     * @param classroomMatch ����� � ����������.
     * @return ���������.
    */
    QString parseClassroom(const QString& classroomMatch) const;
    /**
     * @brief ������ ���� ����.
     * @param datesMatch ����� � ����� ����.
     * @return ���� ����.
    */
    Date parseDates(const QString& datesMatch) const;
    /**
     * @brief ������ ����� � ����� � ����� ����.
     * @param dateString ����� � �����.
     * @return ������ ����.
    */
    QDate parseDate(const QString& dateString) const;

    /**
     * @brief ��������� ����� ����.
     * @param timeCells ������ � �������� � ����������.
     * @param cell �������� ������.
     * @return ����� ����.
    */
    Time_ computeTime(const QMap<int, ParseCell>& timeCells, const ParseCell& cell) const;

private:
    //! ����� �����������
    int id_;
    //! �������� ��� ������ �������.
    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // PARSEWORKER_H
