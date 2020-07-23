#ifndef STANKINSCHEDULEEDITOR2
#define STANKINSCHEDULEEDITOR2

#include <QtWidgets>
#include "view/scheduleHorizontalHeader.h"
#include "view/scheduleVerticalHeader.h"
#include "schedule/schedule.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StankinScheduleEditor2; }
QT_END_NAMESPACE

class StankinScheduleEditor2 : public QMainWindow
{
    Q_OBJECT

public:
    StankinScheduleEditor2(QWidget *parent = nullptr);
    ~StankinScheduleEditor2() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void initStatusBar();
    void initTable();

    void updateTable();
    void resizeTable();
    void updateStatusBarCoords(int row, int column);

private:
    Ui::StankinScheduleEditor2 *ui;

    ScheduleVerticalHeader *headerView_;
    QLabel *labelCoords_;

    Schedule schedule_;

};

#endif // STANKINSCHEDULEEDITOR2
