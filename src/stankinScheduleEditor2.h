#ifndef STANKINSCHEDULEEDITOR2
#define STANKINSCHEDULEEDITOR2

#include <QMainWindow>
#include "schedule/schedule.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StankinScheduleEditor2; }
QT_END_NAMESPACE

class StankinScheduleEditor2 : public QMainWindow
{
    Q_OBJECT

public:
    StankinScheduleEditor2(QWidget *parent = nullptr);
    ~StankinScheduleEditor2();

private:
    Ui::StankinScheduleEditor2 *ui;
};

#endif // STANKINSCHEDULEEDITOR2
