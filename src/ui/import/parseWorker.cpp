#include "parseWorker.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "pair/dateRange.h"
#include "pair/dateSingle.h"
#include "pair/time_.h"
#include "parseFileException.h"
#include "schedule.h"

using namespace cv;
using namespace std;

using Contour = vector<Point>;
using Contours = vector<Contour>;

ParseWorker::ParseWorker(
    const QSharedPointer<ParseWorkerManager>& workerManager, int id)
    : QRunnable()
    , id_(id)
    , workerManager_(workerManager)
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

QString ParseWorker::patternCommon()
{
    return QStringList({ PATTERN_TITLE, PATTERN_LECTURER, PATTERN_TYPE,
                           PATTERN_SUBGROUP, PATTERN_CLASSROOM, PATTERN_DATE })
        .join("\\s?");
}

void ParseWorker::run()
{
    auto poopler = make_unique<PopplerWrapper>(this->id_, workerManager_->dpi());
    auto tesseract = make_unique<TesseractWrapper>();

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

        } catch (std::exception& e) {
            workerManager_->setWorkerStatus(id_, WorkerStatus::Error);
            qCritical() << e.what() << path;
        }
    }

    workerManager_->setWorkerStatus(id_, WorkerStatus::Complete);
}

void ParseWorker::startParsing(const QString& pdfFilePath,
    std::unique_ptr<PopplerWrapper>& poopler,
    std::unique_ptr<TesseractWrapper>& tesseract)
{
    QString imageFilePath = poopler->print(pdfFilePath);

    Mat image = imread(imageFilePath.toStdString());
    if (image.data == nullptr) {
        qDebug() << "File not open or find:" << imageFilePath;
        throw std::logic_error("File not open or find: " + imageFilePath.toStdString());
    }

    auto height = image.rows;
    auto width = image.cols;
    auto maxArea = height * width / 20;
    auto minArea = maxArea / 40;

    Mat colorConvertImage;
    cvtColor(image, colorConvertImage, ColorConversionCodes::COLOR_BGR2GRAY);
    Mat thresholdConvertImage;
    threshold(colorConvertImage, thresholdConvertImage, 127, 255,
        ThresholdTypes::THRESH_BINARY);

    Contours contours;
    findContours(thresholdConvertImage, contours, RetrievalModes::RETR_TREE,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    const QString tempFilePath = QDir::temp().filePath(QString("sse2_tesseract_%1.jpg").arg(id_));
    const Scalar color(0, 255, 0);

    QImage sourseImage(imageFilePath);

    QVector<ParseCell> pairCells;
    QMap<int, ParseCell> timeCells;
    QStringList times = Time_::timeList();

    int number = 0;
    for (size_t i = 0; i < contours.size(); ++i) {
        auto& contour = contours[i];
        auto rect = minAreaRect(contour);
        auto area = rect.size.width * rect.size.height;

        if (area >= minArea && area <= maxArea) {
            auto bounding = rect.boundingRect();

            auto tempImage = sourseImage.copy(bounding.x, bounding.y, bounding.width,
                bounding.height);
            tempImage.save(tempFilePath);

            auto text = tesseract->imageToString(tempFilePath).simplified();

            bool timeFind = false;
            for (int j = 0; j < times.size(); ++j) {
                if (text == times[j]) {
                    timeCells[j] = { text, number, bounding.x,
                        bounding.x + bounding.width };
                    timeFind = true;
                    break;
                }
            }

            if (!timeFind) {
                pairCells.append(
                    { text, number, bounding.x, bounding.x + bounding.width });
            }

            drawContours(image, Contours({ contour }), 0, color, 2);
            putText(image, QString::number(number++).toStdString(), rect.center,
                HersheyFonts::FONT_HERSHEY_SIMPLEX, 3, color, 12);
        }

        if (i % 100 == 0) {
            workerManager_->setProgressValue(
                id_, static_cast<int>(i * 70.0 / contours.size()));
        }
    }

    imwrite(imageFilePath.toStdString(), image);


    if (timeCells.size() != Time_::startTime().size()) {
        throw std::logic_error("No found time cells!");
    }

    std::vector<Pair> schedulePairs;
    for (int i = 0; i < pairCells.size(); ++i) {
        auto& cell = pairCells[i];
        // пропуск ячееек с днями недели
        if (std::abs(cell.end - timeCells[0].start) < std::abs(cell.start < timeCells[0].start)) {
            continue;
        }

        QString data = cell.text;
        data = separateByLines(data);
        workerManager_->transitionData(data);
        while (true) {
            try {
                auto pairs = parsePairs(data, cell, timeCells);
                std::copy(pairs.begin(), pairs.end(),
                    std::back_inserter(schedulePairs));
                break;

            } catch (ParseFileException& e) {
                data = confuseLoop(e, data, cell.number, imageFilePath);

            } catch (std::exception& e) {
                qCritical() << e.what();
                throw e;
            }
        }

        workerManager_->setProgressValue(
            id_, static_cast<int>(i * 30.0 / pairCells.size() + 70));
    }

    Schedule schedule;
    for (auto& pair : schedulePairs) {
        schedule.addPair(pair);
    }

    // сохранение расписания
    QFile file(pdfFilePath.left(pdfFilePath.size() - 3) + "json");
    if (file.open(QIODevice::WriteOnly)) {
        auto json = schedule.toJson();
        QJsonDocument doc(json);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }

    // сохранение изображения
    QString filename = QFileInfo(pdfFilePath).baseName();
    QFile debugImage(imageFilePath);
    debugImage.rename(filename + ".jpg");

    workerManager_->setProgressValue(id_, 100);
}

std::vector<Pair> ParseWorker::parsePairs(
    const QString& data, const ParseCell& cell,
    const QMap<int, ParseCell>& timeCells)
{
    std::vector<Pair> pairs;

    auto it = QRegularExpression(PATTERN_DIVIDER).globalMatch(data);
    while (it.hasNext()) {
        auto pairPart = it.next();
        auto pairText = pairPart.captured();

        auto pairMatch = QRegularExpression(patternCommon()).match(pairText);
        if (!pairMatch.hasMatch()) {
            throw ParseFileException("Неправильная структура пары", "null", pairText);
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

        auto time = computeTime(timeCells, cell);

        pairs.emplace_back(title, lecturer, classroom, type, subgroup, time, dates);
    }

    return pairs;
}

QString ParseWorker::separateByLines(const QString& text) const
{
    QString result = "";
    auto it = QRegularExpression(PATTERN_DIVIDER).globalMatch(text);
    while (it.hasNext()) {
        result += it.next().captured() + "\n";
    }
    if (result.isEmpty()) {
        return text;
    }
    return result;
}

QString ParseWorker::confuseLoop(ParseFileException& e, const QString& data,
    int number, const QString& imagePath) const
{
    ConfuseInfo info = { id_, number, e.type(), e.maybe(),
        e.confuse(), data, imagePath };
    workerManager_->setConfuseInfo(id_, info);

    while (true) {
        if (workerManager_->status(id_) == WorkerStatus::ConfuseSolved) {
            return workerManager_->solve(id_);
        }
        QThread::sleep(1);
    }

    qCritical() << "Unknown error (ConfuseLoop)";
    return "";
}

QString ParseWorker::parseTitle(const QString& titleMatch) const
{
    auto title = titleMatch.left(titleMatch.size() - 1).trimmed();
    auto info = workerManager_->hasTitle(title);
    if (!info.valid) {
        throw ParseFileException("Неизвесная дисциплина", info.data, title);
    }

    return info.data;
}

QString ParseWorker::parseLecturer(const QString& lecturerMatch) const
{
    auto lecturer = lecturerMatch.left(lecturerMatch.size() - 1).trimmed();
    if (lecturer.isEmpty()) {
        return "";
    }

    auto info = workerManager_->hasLecturer(lecturer);
    if (!info.valid) {
        throw ParseFileException("Неизвесный преподаватель", info.data, lecturer);
    }

    return info.data;
}

Type ParseWorker::parseType(const QString& typeMatch) const
{
    auto typeString = typeMatch.left(typeMatch.size() - 1).trimmed().toLower();

    if (typeString == "лекции") {
        return Type::getLecture();
    } else if (typeString == "семинар") {
        return Type::getSeminar();
    } else if (typeString == "лабораторные занятия") {
        return Type::getLaboratory();
    }

    throw ParseFileException("Неизвесный тип", "null", typeString);
}

Subgroup ParseWorker::parseSubgroup(const QString& subgroupMatch) const
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

    throw ParseFileException("Неизвесная подгруппа", "null", subgroupMatch);
}

QString ParseWorker::parseClassroom(const QString& classroomMatch) const
{
    auto classroom = classroomMatch.left(classroomMatch.size() - 1).trimmed();
    if (classroom.isEmpty()) {
        return classroom;
    }

    auto info = workerManager_->hasClassroom(classroom);
    if (!info.valid) {
        throw ParseFileException("Неизвесная аудитория", info.data, classroom);
    }

    return info.data;
}

Date ParseWorker::parseDates(const QString& datesMatch) const
{
    auto datesText = datesMatch.trimmed().toLower();
    datesText.replace('[', "");
    datesText.replace(']', "");

    Date dates;

    auto dateStrings = datesText.split(',');
    for (const auto& dateString : dateStrings) {
        // диапозон
        QRegularExpression regRange(PATTERN_DATE_RANGE);
        auto rangeMatch = regRange.match(dateString);
        if (rangeMatch.hasMatch()) {
            auto frequency = Frequency::getOnce();
            
            auto frequencyString = rangeMatch.captured(4).toLower().replace("..", ".");
            if (frequencyString == "к.н.") {
                frequency = Frequency::getEvery();
            } else if (frequencyString == "ч.н.") {
                frequency = Frequency::getThroughout();
            } else {
                throw ParseFileException("Неизвестная периодичность", "null",
                    frequencyString);
            }

            auto start = rangeMatch.captured(2);
            auto end = rangeMatch.captured(3);
            unique_ptr<DateItem> date = make_unique<DateRange>(parseDate(start), parseDate(end), frequency);
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

        throw ParseFileException("Неизвестная дата", "null", dateString);
    }

    return dates;
}

QDate ParseWorker::parseDate(const QString& dateString) const
{
    int year = QDate::currentDate().year() - 2;
    return QDate::fromString(dateString.trimmed() + "." + QString::number(year),
        "dd.MM.yyyy");
}

Time_ ParseWorker::computeTime(const QMap<int, ParseCell>& timeCells,
    const ParseCell& cell) const
{
    const int TIME_COUNT = Time_::startTime().size();

    int diffStart = qAbs(cell.start - timeCells.first().start);
    int diffEnd = qAbs(cell.end - timeCells.first().end);
    int start = 0;
    int end = 0;

    for (int i = 0; i < TIME_COUNT; ++i) {
        int diff = qAbs(cell.start - timeCells[i].start);
        if (diff < diffStart) {
            diffStart = diff;
            start = i;
        }

        diff = qAbs(cell.end - timeCells[i].end);
        if (diff < diffEnd) {
            diffEnd = diff;
            end = i;
        }
    }

    return Time_(Time_::startTime()[start], Time_::endTime()[end]);
}
