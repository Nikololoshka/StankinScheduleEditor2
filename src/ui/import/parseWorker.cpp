#include "parseWorker.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "pair/dateRange.h"
#include "pair/dateSingle.h"
#include "pair/time_.h"

#include "parseFileException.h"


using namespace cv;
using namespace std;

using Contour = vector<Point>;
using Contours = vector<Contour>;


ParseWorker::ParseWorker(const QSharedPointer<ParseWorkerManager> &workerManager, int id)
    : QRunnable(),
      id_(id),
      workerManager_(workerManager)
{
}

const QString ParseWorker::PATTERN_TITLE = "([а-яА-Яa-zA-Z0-9\\s\\,\\-\\(\\)\\/\\:]+?\\.)";
const QString ParseWorker::PATTERN_LECTURER = "([а-яА-Я\\_]+\\s([а-яА-я]\\.?){1,2})?";
const QString ParseWorker::PATTERN_TYPE = "([а-яА-я\\s]+?\\.)";
const QString ParseWorker::PATTERN_SUBGROUP = "(\\([абАБ]\\)\\.)?";
const QString ParseWorker::PATTERN_CLASSROOM = "([^\\[\\]]+?\\.)";
const QString ParseWorker::PATTERN_DATE = "(\\[((\\,)|(\\s?"
                                          "(\\d{2}\\.\\d{2})\\-(\\d{2}\\.\\d{2})\\s*?([чкЧК]\\.[нН]\\.{1,2})|"
                                          "(\\s?(\\d{2}\\.\\d{2}))"
                                          "))+\\])";
const QString ParseWorker::PATTERN_DATE_RANGE = "(\\s?(\\d{2}\\.\\d{2})\\-(\\d{2}\\.\\d{2})\\s*?([чкЧК]\\.[нН]\\.{1,2}))";
const QString ParseWorker::PATTERN_DATE_SINGLE = "(\\s?(\\d{2}\\.\\d{2}))";
const QString ParseWorker::PATTERN_DIVIDER = "(.*?\\])";

void ParseWorker::run()
{
    auto poopler = make_unique<PooplerWrapper>(workerManager_->pooplerPath(),
                                               workerManager_->dpi());
    auto tesseract = make_unique<TesseractWrapper>(workerManager_->tesseractPath());

    workerManager_->setWorkerStatus(id_, WorkerStatus::Running);
    // Цикл обработки задач
    while (true) {
        QString path = workerManager_->nextSchedule();
        if (path.isEmpty()) {
            break;
        }

        try {
            workerManager_->setProgressText(id_, QFileInfo(path).baseName());
            startParsing(path, poopler, tesseract);

        } catch (std::exception &e) {
            workerManager_->setWorkerStatus(id_, WorkerStatus::Error);
            qCritical() << e.what();
        }
    }

    workerManager_->setWorkerStatus(id_, WorkerStatus::Complete);
}

void ParseWorker::startParsing(const QString &pdfFilePath,
                               std::unique_ptr<PooplerWrapper> &poopler,
                               std::unique_ptr<TesseractWrapper> &tesseract)
{
    QString imageFilePath = poopler->print(pdfFilePath);

    Mat image = imread(imageFilePath.toStdString());
    if (image.data == nullptr) {
        qDebug() << "File not open or find:" << imageFilePath;
    }

    auto height = image.rows;
    auto width = image.cols;
    auto maxArea = height * width / 20;
    auto minArea = maxArea / 40;

    Mat colorConvertImage;
    cvtColor(image, colorConvertImage, ColorConversionCodes::COLOR_BGR2GRAY);
    Mat thresholdConvertImage;
    threshold(colorConvertImage, thresholdConvertImage, 127, 255, ThresholdTypes::THRESH_BINARY);

    Contours contours;
    findContours(thresholdConvertImage,
                 contours,
                 RetrievalModes::RETR_TREE,
                 ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    const QString tempFilePath = QDir::temp().filePath(QString("sse2_tesseract_%1.jpg").arg(id_));
    const Scalar color(0, 255, 0);

    QImage sourseImage(imageFilePath);

    QVector<ParseCell> pairCells;
    QMap<int, ParseCell> timeCells;
    QStringList times = Time_::timeList();

    int number = 0;
    for (int i = 0; i < contours.size(); ++i) {
        auto &contour = contours[i];
        auto rect = minAreaRect(contour);
        auto area = rect.size.width * rect.size.height;

        if (area >= minArea && area <= maxArea) {
            auto bounding = rect.boundingRect();

            auto tempImage = sourseImage.copy(bounding.x, bounding.y, bounding.width, bounding.height);
            tempImage.save(tempFilePath);

            auto text = tesseract->imageToString(tempFilePath).trimmed();

            bool timeFind = false;
            for (int i = 0; i < times.size(); ++i) {
                if (text == times[i]) {
                    timeCells[i] = { text, bounding.x, bounding.x + bounding.width };
                    timeFind = true;
                    break;
                }
            }

            if (!timeFind) {
                pairCells.append({ text, bounding.x, bounding.x + bounding.width });
            }

            drawContours(image, Contours({ contour }), 0, color, 2);
            putText(image, QString::number(number++).toStdString(),
                    rect.center, HersheyFonts::FONT_HERSHEY_SIMPLEX, 3, color, 12);
        }

        if (i % 100 == 0) {
            workerManager_->setProgressValue(id_, i * 70.0 / contours.size());
        }
    }

    qDebug() << timeCells.size();

    imwrite(imageFilePath.replace(".jpg", "2.jpg").toStdString(), image);
}

QVector<Pair> ParseWorker::parsePairs(const QString &text)
{
    QRegularExpression regDivider(PATTERN_DIVIDER);

    auto it = regDivider.globalMatch(text);
    while (it.hasNext()) {
        auto pairPart = it.next();
        auto pairText = pairPart.captured();

        auto pairMatch = QRegularExpression(patternCommon()).match(pairText);
        if (!pairMatch.hasMatch()) {
            throw ParseFileException("Неправильная структура пары",
                                     "null",
                                     pairText);
        }

        auto titleMatch = pairMatch.captured(1);
        auto title = parseTitle(titleMatch);

        auto lecturerMatch = pairMatch.captured(2);
        auto lecturer = parseLecturer(lecturerMatch);

        auto typeMatch = pairMatch.captured(4);
        auto type = parseType(typeMatch);

        auto subgroupMatch = pairMatch.captured(5);
        auto subgroup = parseSubgroup(subgroupMatch);

        auto classroomMatch = pairMatch.captured(6);
        auto classroom = parseClassroom(classroomMatch);

        auto datesMatch = pairMatch.captured(7);
        auto dates = parseDates(datesMatch);

        // Pair pair(title, lecturer, classroom, type, subgroup, time);
        // pair.setDates(dates);
    }

    return {};
}

QString ParseWorker::patternCommon() const
{
    return QStringList({ PATTERN_TITLE,
                         PATTERN_LECTURER,
                         PATTERN_TYPE,
                         PATTERN_SUBGROUP,
                         PATTERN_CLASSROOM,
                         PATTERN_DATE }).join("\\s?");
}

QString ParseWorker::parseTitle(const QString &titleMatch) const
{
    auto title = titleMatch.left(titleMatch.size() - 1).trimmed();
    if (!workerManager_->hasTitle(title)) {
        throw ParseFileException("Неизвесная дисциплина",
                                 "null",
                                 title);
    }

    return title;
}

QString ParseWorker::parseLecturer(const QString &lecturerMatch) const
{
    auto lecturer = lecturerMatch.left(lecturerMatch.size() - 1).trimmed();
    if (!workerManager_->hasLecturer(lecturer)) {
        throw ParseFileException("Неизвесный преподаватель",
                                 "null",
                                 lecturer);
    }

    return lecturer;
}

Type ParseWorker::parseType(const QString &typeMatch) const
{
    auto typeString = typeMatch.left(typeMatch.size() - 1).trimmed().toLower();

    if (typeString == "лекция") {
        return Type::getLecture();
    } else if (typeString == "семинар") {
        return Type::getSeminar();
    } else if (typeString == "лабораторные занятия") {
        return Type::getLaboratory();
    }

    throw ParseFileException("Неизвесный тип",
                             "null",
                             typeString);
}

Subgroup ParseWorker::parseSubgroup(const QString &subgroupMatch) const
{
    if (subgroupMatch.isEmpty()) {
        return Subgroup::getCommon();
    } else {
        auto subgroup = subgroupMatch.left(subgroupMatch.size() - 1).trimmed().toUpper();
        if (subgroup == "(А)") {
            return Subgroup::getA();
        } else if (subgroup == "(Б)") {
            return Subgroup::getB();
        }
    }

    throw ParseFileException("Неизвесная подгруппа",
                             "null",
                             subgroupMatch);
}

QString ParseWorker::parseClassroom(const QString &classroomMatch) const
{
    auto classroom = classroomMatch.left(classroomMatch.size() - 1).trimmed();
    if (classroom.isEmpty()) {
        return classroom;
    }

    if (!workerManager_->hasClassroom(classroom)) {
        throw ParseFileException("Неизвесная аудитория",
                                 "null",
                                 classroom);
    }

    return classroom;
}

Date ParseWorker::parseDates(const QString &datesMatch) const
{
    auto datesText = datesMatch.trimmed().toLower();
    datesText.replace('[', "");
    datesText.replace(']', "");

    Date dates;

    auto dateStrings = datesText.split(',');
    for (const auto &dateString : dateStrings) {
        // диапозон
        QRegularExpression regRange(PATTERN_DATE_RANGE);
        auto rangeMatch = regRange.match(dateString);
        if (rangeMatch.hasMatch()) {

            auto frequency = Frequency::getOnce();

            auto frequencyString = rangeMatch.captured(4).toLower();
            if (frequencyString == "к.н.") {
                frequency = Frequency::getEvery();
            } else if (frequencyString == "ч.н.") {
                frequency = Frequency::getThroughout();
            } else {
                throw ParseFileException("Неизвестная периодичность",
                                         "null",
                                         frequencyString);
            }

            auto start = rangeMatch.captured(2);
            auto end = rangeMatch.captured(3);
            unique_ptr<DateItem> date = make_unique<DateRange>(parseDate(start),
                                                               parseDate(end),
                                                               frequency);
            dates.addDate(date);
            continue;
        }

        // простая
        QRegularExpression regSingle(PATTERN_DATE_SINGLE);
        auto singleMatch = regSingle.match(dateString);
        if (singleMatch.hasMatch()) {
            auto value = singleMatch.captured(1);
            unique_ptr<DateItem> date = make_unique<DateSingle>(parseDate(value));
            dates.addDate(date);
            continue;
        }

        throw ParseFileException("Неизвестная дата",
                                 "null",
                                 dateString);
    }

    return dates;
}

QDate ParseWorker::parseDate(const QString &dateString) const
{
    int year = QDate::currentDate().year();
    return QDate::fromString(dateString + "." + QString::number(year), "dd.MM.yyyy");
}
