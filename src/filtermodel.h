#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QSortFilterProxyModel>

class FilterModel : public QSortFilterProxyModel
{
public:

    enum EColumn
    {
        E_COLUMN__BEGIN,
        E_COLUMN_ID = E_COLUMN__BEGIN,
        E_COLUMN_DATE,
        E_COLUMN_TIME,
        E_COLUMN_DESCRIPTION,
        E_COLUMN_CATEGORY,
        E_COLUMN_DURATION,
        E_COLUMN_RUNNING,
        E_COLUMN__END
    };

    FilterModel();

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& idx, int role) const override;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

    //bool filterAcceptsColumn(int source_column, const QModelIndex& source_parent) const override;
};

#endif // FILTERMODEL_H
