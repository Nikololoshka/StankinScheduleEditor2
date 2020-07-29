#ifndef STANKINSCHEDULEEDITOR2
#define STANKINSCHEDULEEDITOR2

#include <QtWidgets>
#include "scheduleHorizontalHeader.h"
#include "scheduleVerticalHeader.h"
#include "schedule.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StankinScheduleEditor2; }
QT_END_NAMESPACE

class StankinScheduleEditor2 : public QMainWindow
{
    Q_OBJECT

public:
    StankinScheduleEditor2(QWidget *parent = nullptr);
    ~StankinScheduleEditor2() override;

public slots:
    void onTableCellDoubleClicked(int row, int column);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initStatusBar();
    void initTable();

    void updateTable();
    void resizeTable();
    void updateStatusBarCoords(int row, int column);
    static QFont fontForText(const QString &text, int width, int height,
                             unsigned int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap);

private:
    Ui::StankinScheduleEditor2 *ui;

    ScheduleVerticalHeader *headerView_;
    QLabel *labelCoords_;

    QSharedPointer<Schedule> schedule_;

};

#endif // STANKINSCHEDULEEDITOR2
