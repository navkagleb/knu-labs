#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <iostream>

// constructor / destructor
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow), _currentTask(new Ng::Task) {
    this->_ui->setupUi(this);
    this->setWindowTitle("to do list");
    this->_calendarForm = new CalendarForm(this);
    this->_calendarForm->setModal(true);

    this->_lists["today"];
    this->_lists["tomorrow"];

    this->_taskListModel = new TaskListModel(this);
    this->_taskListDelegate = new TaskListDelegate(this);

    this->initListsContainer();
    this->initTasksContainer();
    this->taskFormSetEnabled(false);
}

MainWindow::~MainWindow() {
    delete this->_ui;

    for (auto& [listName, tasks] : this->_lists.toStdMap())
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
    this->_lists["today"].append(this->_currentTask);

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

    if (this->_currentTask->date())
        this->_ui->taskDate->setDate(*this->_currentTask->date());

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

void MainWindow::on_taskDescription_textChanged() {
    this->_currentTask->setDescription(this->_ui->taskDescription->toPlainText());
}

void MainWindow::on_newTask_textChanged(const QString& /* arg */){
    if (this->_currentTask)
        this->_currentTask = nullptr;
}

// private methods
void MainWindow::initListsContainer() {
    this->_ui->listsContainer->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    for (const auto& list : this->_lists.keys()) {
        QPushButton* button = new QPushButton(this);

        button->setText(list);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        button->setMinimumHeight(50);
        this->_ui->listsContainer->addWidget(button);
    }
}

void MainWindow::initTasksContainer() {    
    this->_ui->tasksContainer->setModel(this->_taskListModel);
    this->_ui->tasksContainer->verticalHeader()->setVisible(false);
    this->_ui->tasksContainer->horizontalHeader()->setVisible(false);
    this->_ui->tasksContainer->setItemDelegate(this->_taskListDelegate);
//    this->_ui->tasksContainer->setColumnWidth(0, this->width() * 0.75);
//    this->_ui->tasksContainer->horizontalHeader()->setStretchLastSection(true);
    this->_ui->tasksContainer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->_ui->tasksContainer->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->_ui->tasksContainer->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    this->_ui->tasksContainer->setSortingEnabled(true);

    for (const auto& task : this->_lists["today"])
        this->addTask(task);
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
    this->_ui->taskName->setEnabled(enabled);
    this->_ui->taskDescription->setEnabled(enabled);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    this->_ui->tasksContainer->setColumnWidth(0, this->width() * 0.75);
    this->_ui->tasksContainer->horizontalHeader()->setStretchLastSection(true);
    QMainWindow::resizeEvent(event);
    std::cout << "fuck this shit" << std::endl;
}
