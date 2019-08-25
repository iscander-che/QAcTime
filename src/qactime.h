#ifndef QACTIME_H
#define QACTIME_H

#include "database.h"
//#include "sqlmodel.h"
#include "filtermodel.h"
#include <QSqlTableModel>
#include <QMainWindow>

namespace Ui {
class QAcTime;
}

class QAcTime : public QMainWindow
{
    Q_OBJECT

public:
    explicit QAcTime(QWidget* parent = 0);
    ~QAcTime();

    void closeEvent(QCloseEvent* event);

private slots:
    void on_newActivityButton_clicked();
    void on_pauseButton_clicked();
    void on_continueButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void about();
    void createReport();

private:
    Ui::QAcTime* ui;
    DataBase* database;
    QSqlTableModel* sqlModel;
    FilterModel* filterModel;

    void updateTableView();
    void updateButtons(QModelIndex index);
    void disableButtons();
};

#endif // QACTIME_H
