#include "qactime.h"
#include "ui_qactime.h"
#include "neweditactivitydialog.h"
#include "deleteactivitydialog.h"
#include "dateselector.h"
#include <QSharedPointer>
#include <QCloseEvent>
#include <QDate>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSqlRecord>

QAcTime::QAcTime(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::QAcTime)
{
    ui->setupUi(this);

    database = new DataBase();

    sqlModel = new QSqlTableModel();
    sqlModel->setTable(ActivityTable::nameTable);
    sqlModel->select();

    int numberRows = sqlModel->rowCount();

    for(int row = 0; row < numberRows; ++row)
    {
        qDebug() << sqlModel->record(row).value("act_date");
    }

    filterModel = new FilterModel();
    filterModel->setSourceModel(sqlModel);

    filterModel->setHeaderData(FilterModel::E_COLUMN_TIME, Qt::Horizontal, tr("Время"));
    filterModel->setHeaderData(FilterModel::E_COLUMN_DESCRIPTION, Qt::Horizontal, tr("Описание"));
    filterModel->setHeaderData(FilterModel::E_COLUMN_CATEGORY, Qt::Horizontal, tr("Категория"));
    filterModel->setHeaderData(FilterModel::E_COLUMN_DURATION, Qt::Horizontal, tr("Длительность"));

    ui->tableView->setModel(filterModel);

    ui->tableView->setColumnHidden(FilterModel::E_COLUMN_DATE, true);
    ui->tableView->setColumnHidden(FilterModel::E_COLUMN_ID, true);
    ui->tableView->setColumnHidden(FilterModel::E_COLUMN_RUNNING, true);

    ui->tableView->resizeColumnToContents(FilterModel::E_COLUMN_TIME);
    ui->tableView->resizeColumnToContents(FilterModel::E_COLUMN_DURATION);

    ui->tableView->setColumnWidth(FilterModel::E_COLUMN_DESCRIPTION, 175);
    ui->tableView->setColumnWidth(FilterModel::E_COLUMN_CATEGORY, 125);

    updateTableView();

    connect(ui->tableView, &QTableView::clicked, this, &QAcTime::updateButtons);
    connect(ui->tableView, &QTableView::doubleClicked, this, &QAcTime::on_editButton_clicked);

    ui->todayLabel->setText(tr("Сегодня: ") + QDate::currentDate().toString("dd.MM.yyyy"));

    ui->mainToolBar->addAction(ui->newAction);
    ui->mainToolBar->addAction(ui->pauseAction);
    ui->mainToolBar->addAction(ui->continueAction);
    ui->mainToolBar->addAction(ui->editAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->deleteAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->reportAction);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->aboutAction);
    ui->mainToolBar->addAction(ui->aboutQtAction);

    connect(ui->newAction, &QAction::triggered, this, &QAcTime::on_newActivityButton_clicked);
    connect(ui->pauseAction, &QAction::triggered, this, &QAcTime::on_pauseButton_clicked);
    connect(ui->continueAction, &QAction::triggered, this, &QAcTime::on_continueButton_clicked);
    connect(ui->editAction, &QAction::triggered, this, &QAcTime::on_editButton_clicked);
    connect(ui->deleteAction, &QAction::triggered, this, &QAcTime::on_deleteButton_clicked);
    connect(ui->reportAction, &QAction::triggered, this, &QAcTime::createReport);
    connect(ui->aboutAction, &QAction::triggered, this, &QAcTime::about);
    connect(ui->aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

QAcTime::~QAcTime()
{
    delete ui;
}

void QAcTime::closeEvent(QCloseEvent* event)
{
    int numberRows = filterModel->rowCount();
    for(int row = 0; row < numberRows; ++row)
    {
        QModelIndex idIndex = filterModel->index(row, FilterModel::E_COLUMN_ID);
        QModelIndex runningIndex = filterModel->index(row, FilterModel::E_COLUMN_RUNNING);
        int id = filterModel->data(idIndex, Qt::DisplayRole).toInt();
        bool isRunning = filterModel->data(runningIndex, Qt::DisplayRole).toBool();
        if(isRunning)
            database->pauseActivity(id);
    }
    event->accept();
}

void QAcTime::updateTableView()
{
    int numberRows = filterModel->rowCount();
    int rowHeight = 20;
    for(int row = 0; row < numberRows; ++row)
        ui->tableView->setRowHeight(row, rowHeight);
    if(numberRows)
    {
        int row = 0;
        QModelIndex currentIndex = filterModel->index(row, FilterModel::E_COLUMN_TIME);
        ui->tableView->setCurrentIndex(currentIndex);
        updateButtons(currentIndex);
    }
    else
    {
        disableButtons();

        if(sqlModel->rowCount() == 0)
            ui->reportAction->setDisabled(true);
    }

    ui->tableView->scrollToBottom();
}

void QAcTime::updateButtons(QModelIndex index)
{
    QModelIndex runningIndex = filterModel->index(index.row(), FilterModel::E_COLUMN_RUNNING);
    ui->continueButton->setDisabled(filterModel->data(runningIndex, Qt::DisplayRole).toBool());
    ui->continueAction->setDisabled(filterModel->data(runningIndex, Qt::DisplayRole).toBool());
    ui->pauseButton->setEnabled(filterModel->data(runningIndex, Qt::DisplayRole).toBool());
    ui->pauseAction->setEnabled(filterModel->data(runningIndex, Qt::DisplayRole).toBool());
    ui->editButton->setEnabled(true);
    ui->editAction->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->reportAction->setEnabled(true);
}

void QAcTime::disableButtons()
{
    ui->continueButton->setDisabled(true);
    ui->pauseButton->setDisabled(true);
    ui->deleteButton->setDisabled(true);
    ui->editButton->setDisabled(true);

    ui->continueAction->setDisabled(true);
    ui->pauseAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->editAction->setDisabled(true);
}

void QAcTime::on_newActivityButton_clicked()
{
    QSharedPointer<NewEditActivityDialog> dialog(new NewEditActivityDialog(sqlModel));
    if(dialog->exec() == QDialog::Accepted)
    {
        QString description = dialog->getDescription();
        QString category = dialog->getCategory();
        if(!description.isEmpty())
        {
            database->newActivity(description, category);
            sqlModel->select();
            updateTableView();

            int numberRows = filterModel->rowCount();
            QModelIndex currentIndex = filterModel->index(numberRows - 1, FilterModel::E_COLUMN_TIME);
            ui->tableView->setCurrentIndex(currentIndex);

            updateButtons(currentIndex);
        }
    }
}

void QAcTime::on_pauseButton_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    QModelIndex index = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_ID);
    int id = filterModel->data(index, Qt::DisplayRole).toInt();
    database->pauseActivity(id);
    sqlModel->select();

    disableButtons();
}

void QAcTime::on_continueButton_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    QModelIndex index = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_ID);
    int id = filterModel->data(index, Qt::DisplayRole).toInt();
    database->continueActivity(id);
    sqlModel->select();

    disableButtons();
}

void QAcTime::on_editButton_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    QModelIndex idIndex = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_ID);
    QModelIndex descriptionIndex = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_DESCRIPTION);
    QModelIndex categoryIndex = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_CATEGORY);
    currentIndex = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_TIME);

    int id = filterModel->data(idIndex, Qt::DisplayRole).toInt();
    QString description = filterModel->data(descriptionIndex, Qt::DisplayRole).toString();
    QString category = filterModel->data(categoryIndex, Qt::DisplayRole).toString();

    QSharedPointer<NewEditActivityDialog> dialog(new NewEditActivityDialog(sqlModel));

    dialog->setDescription(description);
    dialog->setCategory(category);

    if(dialog->exec() == QDialog::Accepted)
    {
        QString newDescription = dialog->getDescription();
        QString newCategory = dialog->getCategory();
        if(!newDescription.isEmpty() && (newDescription != description || newCategory != category))
        {
            database->editActivity(id, newDescription, newCategory);
            sqlModel->select();

            disableButtons();
        }
    }
}

void QAcTime::on_deleteButton_clicked()
{
    QSharedPointer<DeleteActivityDialog> dialog(new DeleteActivityDialog());

    QModelIndex currentIndex = ui->tableView->currentIndex();
    QModelIndex idIndex = filterModel->index(currentIndex.row(), FilterModel::E_COLUMN_ID);
    int id = filterModel->data(idIndex, Qt::DisplayRole).toInt();

    if(dialog->exec() == QDialog::Accepted)
    {
        database->deleteActivity(id);
        sqlModel->select();
        updateTableView();
    }
}

void QAcTime::about()
{
    QMessageBox::about(this, tr("О приложении"),
            tr("<center>"
               "QAcTime - приложение для учёта времени, затрачиваемого на действия<br />"
               "Copyright &copy;2019 Iscander Che "
               "&lt;<a href=\"mailto:iscander.che@gmail.com\">iscander.che@gmail.com</a>&gt;"
               "</center>"));
}

void QAcTime::createReport()
{
    int numberRowsFilterModel = filterModel->rowCount();
    int idCurrentActivity = 0;
    int rowCurrentActivity = 0;
    for(int row = 0; row < numberRowsFilterModel; ++row)
    {
        QModelIndex idIndex = filterModel->index(row, FilterModel::E_COLUMN_ID);
        QModelIndex runningIndex = filterModel->index(row, FilterModel::E_COLUMN_RUNNING);
        int id = filterModel->data(idIndex, Qt::DisplayRole).toInt();
        bool isRunning = filterModel->data(runningIndex, Qt::DisplayRole).toBool();
        if(isRunning)
        {
            database->pauseActivity(id);
            idCurrentActivity = id;
            rowCurrentActivity = row;
        }
    }

    QStringList dateList;
    int numberRowsSQLModel = sqlModel->rowCount();

    for(int row = 0; row < numberRowsSQLModel; ++row)
    {
        QModelIndex index = sqlModel->index(row, FilterModel::E_COLUMN_DATE);
        dateList << sqlModel->data(index).toDate().toString(Qt::ISODate);
    }

    dateList.removeDuplicates();

    QSharedPointer<DateSelector> dialog(new DateSelector(dateList));

    if(dialog->exec() == QDialog::Accepted)
    {
        QStringList selectedDatesList;
        selectedDatesList = dialog->getSelectedDates();

        if(!selectedDatesList.isEmpty())
        {
            QStringList rowModelList;
            for(QString selectedDate : selectedDatesList)
            {
                for(int row = 0; row < numberRowsSQLModel; ++row)
                {
                    QStringList cellList;
                    QModelIndex index = sqlModel->index(row, FilterModel::E_COLUMN_DATE);
                    QString date = sqlModel->data(index).toDate().toString(Qt::ISODate);

                    if(selectedDate == date)
                    {
                        index = sqlModel->index(row, FilterModel::E_COLUMN_TIME);
                        QString time = sqlModel->data(index).toTime().toString("hh:mm");
                        index = sqlModel->index(row, FilterModel::E_COLUMN_DESCRIPTION);
                        QString description = sqlModel->data(index).toString();
                        index = sqlModel->index(row, FilterModel::E_COLUMN_CATEGORY);
                        QString category = sqlModel->data(index).toString();
                        index = sqlModel->index(row, FilterModel::E_COLUMN_DURATION);
                        QString duration = sqlModel->data(index).toTime().toString("hh:mm:ss");
                        cellList << date << time << description << category << duration;
                        QString item = cellList.join("\t");
                        rowModelList << item;
                    }
                }
            }

            QString line = rowModelList.join("\n");
            QString fileName = "";
            if(selectedDatesList.first() == selectedDatesList.last())
                fileName = "report_" + selectedDatesList.first();
            else
                fileName = "report_" + selectedDatesList.first() + "_" +
                                       selectedDatesList.last();
            QString strFilter;
            fileName = QFileDialog::getSaveFileName(0,
                                                    tr("Сохранить отчёт"),
                                                    fileName,
                                                    "*.dat",
                                                    &strFilter);
            if(!fileName.isEmpty())
            {
                QFile file(fileName);
                if(file.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QTextStream stream(&file);

                    stream << line;

                    file.close();
                    qDebug() << "Report is writing";
                }
                else
                    qDebug() << "Error report writing";
            }
        }
    }

    if(numberRowsFilterModel > 0 && idCurrentActivity > 0)
    {
        database->continueActivity(idCurrentActivity);
        sqlModel->select();
        QModelIndex currentIndex = filterModel->index(rowCurrentActivity, FilterModel::E_COLUMN_TIME);
        ui->tableView->setCurrentIndex(currentIndex);
        updateButtons(currentIndex);
    }
}
