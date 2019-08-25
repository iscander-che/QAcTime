/** Программа учёта времени выполнения действий.
 *
 * @author Iscander Che <iscander.che@gmail.com>
 * @copyright GPL-3.0
 * @date 22.08.2019
 * @version v.0.1
 */

#include "qactime.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QAcTime w;
    w.show();

    return app.exec();
}
