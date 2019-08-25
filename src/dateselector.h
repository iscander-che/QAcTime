#ifndef DATESELECTOR_H
#define DATESELECTOR_H

#include <QDialog>
#include <QStringList>
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDate>

namespace Ui {
class DateSelector;
}

/** @brief Класс формы выбора дат для формирования отчёта. */
class DateSelector : public QDialog
{
    Q_OBJECT

public:
    /** @brief Конструктор формы выбора дат.
     * @param QStringList& listDates - список дат, для которых зарегистрированы действия. */
    explicit DateSelector(QStringList& listDates, QWidget* parent = 0);

    /** @brief Деструктор формы выбора дат. */
    ~DateSelector();

    /** @brief Возвращение списка дат, выбранных для формирования отчёта.
     * @return QStringList - список выбранных дат. */
    QStringList getSelectedDates();

private slots:
    /** @brief Слот изменения состояния чекбокса элемента. */
    void changeStateCheckBox(QTreeWidgetItem* item, int column);

    /** @brief Слот кнопки "Select dates" для формирования списка дат для отчёта. */
    void on_selectPushButton_clicked();

private:
    Ui::DateSelector* ui; /**< Ссылка на форму выбора дат с элементами управления. */
    QTreeWidgetItem* rootItem; /**< Корневой элемент дерева с датами. */

    /** @brief Рекурсивное изменение состояния чекбоксов нижеследующих элементов. */
    void changeState(QTreeWidgetItem* item, Qt::CheckState state);
};

#endif // DATESELECTOR_H
