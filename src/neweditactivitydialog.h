#ifndef NEWEDITACTIVITYDIALOG_H
#define NEWEDITACTIVITYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class NewEditActivityDialog;
}

class NewEditActivityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewEditActivityDialog(QSqlTableModel* mainModel, QWidget* parent = 0);
    ~NewEditActivityDialog();

public slots:
    QString getDescription();
    QString getCategory();
    void setDescription(const QString& description);
    void setCategory(const QString& category);

private slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::NewEditActivityDialog *ui;
    QSqlTableModel* model;
};

#endif // NEWEDITACTIVITYDIALOG_H
