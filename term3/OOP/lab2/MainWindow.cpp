#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QModelIndex>
#include <QTreeWidget>
#include <iostream>

// constructor / destructor
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow), _currentTask(new Ng::Task), _index(0) {
    this->_ui->setupUi(this);
    this->setWindowTitle("todo list");
    this->_calendarForm = new CalendarForm(this);
    this->_calendarForm->setModal(true);

    this->_taskListModel = new TaskListModel(this->_tasks, this);
    this->_taskListDelegate = new TaskListDelegate(this);
    this->_ui->newTask->setEnabled(false);

    this->initListsContainer();
    this->initTasksContainer();
    this->taskFormSetEnabled(false);
}

MainWindow::~MainWindow() {
    delete this->_ui;

    for (auto& [listName, tasks] : this->_tasks.toStdMap())
        for (auto& task : tasks)
            delete task;
}

// private slots
void MainWindow::on_dateButton_clicked() {
    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_calendarForm->setTask(*this->_currentTask);
    this->_calendarForm->show();
    this->_calendarForm->exec();

    if (this->_currentTask->date())
        this->_ui->dateButton->setText(QString::number(this->_currentTask->date()->day()));
}

void MainWindow::on_priority_activated(int index) {
    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_currentTask->setPriority(static_cast<Ng::Task::Priority>(index));
}

void MainWindow::on_newTask_returnPressed() {
    if (this->_ui->newTask->text().isEmpty())
        return;

    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_currentTask->setName(this->_ui->newTask->text());
    this->_tasks[this->_lists[this->_currentList]].append(this->_currentTask);

    this->addTask(this->_currentTask);
    this->clear();
}

void MainWindow::on_tasksContainer_clicked(const QModelIndex& index) {
    this->_currentTask = this->_taskListModel->getItem(index);

    if (!this->_currentTask)
        return;

    this->taskFormSetEnabled(true);

    if (this->_currentTask->time())
        this->_ui->taskTime->setTime(*this->_currentTask->time());
    else
        this->_ui->taskTime->setTime(QTime(0, 0));

    if (this->_currentTask->date())
        this->_ui->taskDate->setDate(*this->_currentTask->date());
    else
        this->_ui->taskDate->setDate(QDate(0, 1, 1));

    this->_ui->taskPriority->setCurrentIndex(static_cast<int>(this->_currentTask->priority()));
    this->_ui->taskName->setText(this->_currentTask->name());
    this->_ui->taskDescription->setText(this->_currentTask->description());
}

void MainWindow::on_taskTime_editingFinished() {
    this->_ui->taskTime->clearFocus();
    this->_currentTask->setTime(this->_ui->taskTime->time());
    this->_taskListModel->update();
}

void MainWindow::on_taskName_returnPressed() {
    this->_ui->taskName->clearFocus();
    this->_currentTask->setName(this->_ui->taskName->text());
    this->_taskListModel->update();
}

void MainWindow::on_taskDate_editingFinished() {
    this->_ui->taskDate->clearFocus();
    this->_currentTask->setDate(this->_ui->taskDate->date());
    this->_taskListModel->update();
}

void MainWindow::on_taskPriority_activated(int index) {
    this->_ui->taskPriority->clearFocus();
    this->_currentTask->setPriority(static_cast<Ng::Task::Priority>(index));
    this->_taskListModel->update();
}

void MainWindow::on_taskDescription_textChanged() {
    this->_currentTask->setDescription(this->_ui->taskDescription->toPlainText());
}

void MainWindow::on_newTask_textChanged(const QString& /* arg */){
    if (this->_currentTask)
        this->_currentTask = nullptr;
}

void MainWindow::on_listsContainer_itemClicked(QTreeWidgetItem* item, int /* column */) {
    this->_currentList = item->text(0);
    this->updateTasks();

    if (this->_currentList == "archive")
        this->_ui->newTask->setEnabled(false);
    else {
        this->_ui->newTask->setEnabled(true);
    }
}

void MainWindow::on_addList_clicked() {
    auto* item = new QTreeWidgetItem;

    item->setFlags(item->flags() | Qt::ItemIsEditable);
    this->_ui->listsContainer->addTopLevelItem(item);
    this->_ui->listsContainer->setCurrentItem(item);
    this->_ui->listsContainer->editItem(item, 0);
}

void MainWindow::on_listsContainer_itemChanged(QTreeWidgetItem* item, int column) {
    if (item->text(0).isEmpty())
        return;

    if (this->_index == -1) {
        this->_lists[item->text(column)] = this->_lists.count();
    } else {
        this->_lists[item->text(column)] = this->_index;
        this->_index = -1;
    }

    this->_currentList = item->text(column);
    this->updateTasks();

    std::cout << "!!!!!!!" << std::endl;
    for (auto list : this->_lists.keys())
        std::cout << "list " << list.toStdString() << ", " << this->_lists[list] << std::endl;
    std::cout << std::endl;
}

void MainWindow::on_listsContainer_itemDoubleClicked(QTreeWidgetItem *item, int column) {
    QString list = item->text(column);
    this->_index = this->_lists[list];
}

void MainWindow::on_removeList_clicked() {
    auto* current = this->_ui->listsContainer->currentItem();
    int index = this->_lists[current->text(0)];

    if (current->text(0) == "archive") {
        QMessageBox messageBox;

        messageBox.setText("Cannot remove 'archive' list");
        messageBox.exec();
        return;
    }

    this->_lists.remove(current->text(0));
    this->_tasks.remove(index);

    for (auto& [list, i] : this->_lists.toStdMap())
        if (i > index)
            --i;

    for (auto& i : this->_tasks.keys())
        if (i > index)
            --i;

    delete current;

    if (this->_ui->listsContainer->topLevelItemCount() == 0) {
        this->_currentList = "";
        this->_taskListModel->clear();
        this->_taskListModel->update();
    } else {
        this->_currentList = this->_ui->listsContainer->currentItem()->text(0);
        this->updateTasks();

        if (this->_currentList == "archive")
            this->_ui->newTask->setEnabled(false);
        else {
            this->_ui->newTask->setEnabled(true);
        }
    }
}

// private methods
void MainWindow::initListsContainer() {
    this->_ui->listsContainer->setHeaderHidden(true);

    this->_lists["archive"] = 0;
    this->_tasks[0];
    this->_currentList = "archive";

    for (int i = 0; i < this->_lists.count(); ++i) {
        auto* item = new QTreeWidgetItem(this->_ui->listsContainer);

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setText(0, this->_lists.keys()[i]);
        this->_ui->listsContainer->addTopLevelItem(item);
    }

    this->_ui->listsContainer->setCurrentItem(this->_ui->listsContainer->topLevelItem(0));
}

void MainWindow::initTasksContainer() {    
    this->_ui->tasksContainer->setModel(this->_taskListModel);
    this->_ui->tasksContainer->verticalHeader()->setVisible(false);
    this->_ui->tasksContainer->horizontalHeader()->setVisible(false);
    this->_ui->tasksContainer->setItemDelegate(this->_taskListDelegate);
    this->_ui->tasksContainer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->_ui->tasksContainer->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->_ui->tasksContainer->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    this->_ui->tasksContainer->setSortingEnabled(true);
    this->_ui->tasksContainer->setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QTableView::item:selected {background: rgba(0, 0, 0, 1%);}");
}

void MainWindow::addTask(Ng::Task* task) { this->_taskListModel->setItem(task); }

void MainWindow::clear() {
    this->_ui->newTask->clear();
    this->_ui->dateButton->setText("date");
    this->_ui->priority->setCurrentIndex(0);
    this->_currentTask = nullptr;
    this->_calendarForm->clear();
}

void MainWindow::taskFormSetEnabled(bool enabled) {
    this->_ui->taskTime->setEnabled(enabled);
    this->_ui->taskDate->setEnabled(enabled);
    this->_ui->taskPriority->setEnabled(enabled);
    this->_ui->taskName->setEnabled(enabled);
    this->_ui->taskDescription->setEnabled(enabled);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    this->_ui->tasksContainer->setColumnWidth(0, this->width() * 0.75);
    this->_ui->tasksContainer->horizontalHeader()->setStretchLastSection(true);
    QMainWindow::resizeEvent(event);
}

void MainWindow::updateTasks() {
    this->_taskListModel->clear();
    this->_taskListModel->update();

    for (auto& task : this->_tasks[this->_lists[this->_currentList]])
        this->_taskListModel->setItem(task);
}
