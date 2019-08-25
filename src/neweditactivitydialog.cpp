#include "filtermodel.h"
#include "neweditactivitydialog.h"
#include "ui_neweditactivitydialog.h"
#include <QCompleter>
#include <QSqlRecord>
#include <QSharedPointer>

NewEditActivityDialog::NewEditActivityDialog(QSqlTableModel* mainModel, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NewEditActivityDialog),
    model(mainModel)
{
    ui->setupUi(this);

    model->select();

    int numberRows = model->rowCount();

    QStringList descriptionList;
    QStringList categoryList;

    for(int row = 0; row < numberRows; ++row)
    {
        descriptionList << model->record(row).value(FilterModel::E_COLUMN_DESCRIPTION).toString();
        categoryList << model->record(row).value(FilterModel::E_COLUMN_CATEGORY).toString();
    }

    descriptionList.removeDuplicates();
    categoryList.removeDuplicates();

    QCompleter* completerDescription = new QCompleter(descriptionList);
    QCompleter* completerCategories = new QCompleter(categoryList);
    ui->descriptionLineEdit->setCompleter(completerDescription);
    ui->categoryLineEdit->setCompleter(completerCategories);
    completerDescription = nullptr;
    delete completerDescription;
    completerCategories = nullptr;
    delete completerCategories;
}

NewEditActivityDialog::~NewEditActivityDialog()
{
    delete ui;
}

QString NewEditActivityDialog::getDescription()
{
    return ui->descriptionLineEdit->text();
}

QString NewEditActivityDialog::getCategory()
{
    return ui->categoryLineEdit->text();
}

void NewEditActivityDialog::setDescription(const QString& description)
{
    ui->descriptionLineEdit->setText(description);
}

void NewEditActivityDialog::setCategory(const QString& category)
{
    ui->categoryLineEdit->setText(category);
}

void NewEditActivityDialog::on_OKButton_clicked()
{
    emit accept();
}

void NewEditActivityDialog::on_cancelButton_clicked()
{
    emit reject();
}
