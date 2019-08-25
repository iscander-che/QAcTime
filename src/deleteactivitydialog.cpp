#include "deleteactivitydialog.h"
#include "ui_deleteactivitydialog.h"

DeleteActivityDialog::DeleteActivityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteActivityDialog)
{
    ui->setupUi(this);
}

DeleteActivityDialog::~DeleteActivityDialog()
{
    delete ui;
}

void DeleteActivityDialog::on_OKButton_clicked()
{
    emit accept();
}

void DeleteActivityDialog::on_cancelButton_clicked()
{
    emit reject();
}
