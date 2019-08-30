#include "filtermodel.h"

#include <QDate>
#include <QTime>

FilterModel::FilterModel()
{

}

Qt::ItemFlags FilterModel::flags(const QModelIndex& index) const
{
    if(index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return QSortFilterProxyModel::flags(index);
}

QVariant FilterModel::data(const QModelIndex& idx, int role) const
{
    if(!idx.isValid())
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        if(idx.column() == E_COLUMN_DATE)
        {
            QDate date = QSortFilterProxyModel::data(idx, Qt::DisplayRole).toDate();
            return date.toString("dd.MM.yyyy");
        }
        if(idx.column() == E_COLUMN_TIME)
        {
            QTime time = QSortFilterProxyModel::data(idx, Qt::DisplayRole).toTime();
            return time.toString("hh:mm");
        }
        if(idx.column() == E_COLUMN_DURATION)
        {
            QModelIndex index = QSortFilterProxyModel::index(idx.row(), E_COLUMN_RUNNING);
            if(QSortFilterProxyModel::data(index, Qt::DisplayRole).toInt())
                return "*";
            else
            {
                QTime duration = QSortFilterProxyModel::data(idx, Qt::DisplayRole).toTime();
                return duration.toString("hh:mm:ss");
            }
        }
    }

    return QSortFilterProxyModel::data(idx, role);
}

bool FilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, E_COLUMN_DATE, sourceParent);

    return (sourceModel()->data(index).toDate() == QDate::currentDate());
}
