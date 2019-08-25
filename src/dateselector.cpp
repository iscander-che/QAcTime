#include "dateselector.h"
#include "ui_dateselector.h"

DateSelector::DateSelector(QStringList& listDates, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DateSelector)
{
    ui->setupUi(this);

    ui->datesTreeWidget->setHeaderHidden(true);

    rootItem = new QTreeWidgetItem(ui->datesTreeWidget);
    rootItem->setText(0, tr("Выберите даты"));
    rootItem->setFlags(rootItem->flags() | Qt::ItemIsUserCheckable);
    rootItem->setCheckState(0, Qt::Checked);

    QStringList years;
    QString str = listDates.at(0);
    QStringList items = str.split("-");
    QString year = items.at(0);
    years << year;
    foreach(QString s, listDates)
    {
        if(!s.isEmpty() && !s.contains(year))
        {
            items = s.split("-");
            year = items.at(0);
            years << year;
        }
    }

    QStringList yearsMonths;
    str = listDates.at(0);
    items = str.split("-");
    QStringList temp;
    temp << items.at(0) << items.at(1);
    QString yearMonth = temp.join("-");
    yearsMonths << yearMonth;
    foreach(QString s, listDates)
    {
        if(!s.isEmpty() && !s.contains(yearMonth))
        {
            items = s.split("-");
            QStringList temp;
            temp << items.at(0) << items.at(1);
            yearMonth = temp.join("-");
            yearsMonths << yearMonth;
        }
    }

    QTreeWidgetItem* yearItem;
    QTreeWidgetItem* yearMonthItem;
    QTreeWidgetItem* dateItem;

    foreach(QString sy, years)
    {
        yearItem = new QTreeWidgetItem(rootItem);
        yearItem->setText(0, sy);
        yearItem->setFlags(yearItem->flags() | Qt::ItemIsUserCheckable);
        yearItem->setCheckState(0, Qt::Checked);
        foreach(QString sym, yearsMonths)
        {
            if(sym.contains(sy))
            {
                yearMonthItem = new QTreeWidgetItem(yearItem);
                QStringList ym;
                ym = sym.split("-");
                yearMonthItem->setText(0, ym.at(1));
                yearMonthItem->setFlags(yearMonthItem->flags() |
                                        Qt::ItemIsUserCheckable);
                yearMonthItem->setCheckState(0, Qt::Checked);
                foreach(QString sdt, listDates)
                {
                    if(sdt.contains(sym))
                    {
                        dateItem = new QTreeWidgetItem(yearMonthItem);
                        QDate date = QDate::fromString(sdt, Qt::ISODate);
                        dateItem->setText(0, date.toString("dd.MM.yyyy - ddd"));
                        dateItem->setFlags(dateItem->flags() |
                                           Qt::ItemIsUserCheckable);
                        dateItem->setCheckState(0, Qt::Checked);
                    }
                }
            }
        }
    }

    connect(ui->datesTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(changeStateCheckBox(QTreeWidgetItem*,int)));
}

DateSelector::~DateSelector()
{
    delete ui;
}

QStringList DateSelector::getSelectedDates()
{
    QStringList datesList;
    for(int i = 0; i < rootItem->childCount(); ++i) // пробегаемся по годам
    {
        QTreeWidgetItem* yearItem = rootItem->child(i);
        for(int j = 0; j < yearItem->childCount(); ++j) // пробегаемся по месяцам
        {
            QTreeWidgetItem* monthItem = yearItem->child(j);
            for(int k = 0; k < monthItem->childCount(); ++k) // пробегаемся по датам
            {
                QTreeWidgetItem* dateItem = monthItem->child(k);
                int column = 0;
                if(dateItem->checkState(column))
                {
                    QString dateStr = dateItem->text(column);
                    QStringList dateLst = dateStr.split(" - ");
                    dateStr = dateLst.at(0);
                    QDate date = QDate::fromString(dateStr, "dd.MM.yyyy");
                    datesList << date.toString(Qt::ISODate);
                }
            }
        }
    }

    return datesList;
}

void DateSelector::changeState(QTreeWidgetItem* item, Qt::CheckState state)
{
    for(int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem* childItem = item->child(i);
        childItem->setCheckState(0, state);
        changeState(childItem, state);
    }
}

void DateSelector::changeStateCheckBox(QTreeWidgetItem* item, int column)
{
    changeState(item, item->checkState(column));
}

void DateSelector::on_selectPushButton_clicked()
{
    emit accept();
}
