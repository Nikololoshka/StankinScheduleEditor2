#ifndef PARSEWORKER_H
#define PARSEWORKER_H

#include <QtWidgets>
#include "pair/pair.h"

#include "pooplerWrapper.h"
#include "tesseractWrapper.h"
#include "parseWorkerManager.h"
#include "parseFileException.h"

struct ParseCell {
    QString text;
    int number;
    int start;
    int end;
};

class ParseWorker : public QRunnable
{
public:
    ParseWorker(const QSharedPointer<ParseWorkerManager> &workerManager, int id);

    void run() override;

    const static QString PATTERN_TITLE;
    const static QString PATTERN_LECTURER;
    const static QString PATTERN_TYPE;
    const static QString PATTERN_SUBGROUP;
    const static QString PATTERN_CLASSROOM;
    const static QString PATTERN_DATE;
    const static QString PATTERN_DATE_RANGE;
    const static QString PATTERN_DATE_SINGLE;
    const static QString PATTERN_DIVIDER;

    static QString patternCommon();

private:
    void startParsing(const QString &pdfFilePath,
                      std::unique_ptr<PooplerWrapper> &poopler,
                      std::unique_ptr<TesseractWrapper> &tesseract);
    std::vector<Pair> parsePairs(const QString &data,
                                 const ParseCell &cell,
                                 const QMap<int, ParseCell> &timeCells);
    QString confuseLoop(ParseFileException &e,
                        const QString &data,
                        int number,
                        const QString &imagePath) const;

    QString parseTitle(const QString &titleMatch) const;
    QString parseLecturer(const QString &lecturerMatch) const;
    Type parseType(const QString &typeMatch) const;
    Subgroup parseSubgroup(const QString &subgroupMatch) const;
    QString parseClassroom(const QString &classroomMatch) const;
    Date parseDates(const QString &datesMatch) const;
    QDate parseDate(const QString &dateString) const;

    Time_ computeTime(const QMap<int, ParseCell> &timeCells, const ParseCell &cell) const;

private:    
    int id_;
    QSharedPointer<ParseWorkerManager> workerManager_;
};

#endif // PARSEWORKER_H
