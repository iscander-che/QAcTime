#ifndef DELETEACTIVITYDIALOG_H
#define DELETEACTIVITYDIALOG_H

#include <QDialog>

namespace Ui {
class DeleteActivityDialog;
}

class DeleteActivityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteActivityDialog(QWidget *parent = 0);
    ~DeleteActivityDialog();

private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::DeleteActivityDialog *ui;
};

#endif // DELETEACTIVITYDIALOG_H
