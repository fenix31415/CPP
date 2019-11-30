#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table_result->setHorizontalHeaderItem(0, new QTableWidgetItem("File", 0));
    ui->table_result->setHorizontalHeaderItem(1, new QTableWidgetItem("pos", 0));
    ui->table_result->setHorizontalHeaderItem(2, new QTableWidgetItem("context", 0));

    connect(ui->button_search, &QPushButton::clicked, this, [this] {
        ui->table_result->setRowCount(0);
        searcher.start_searching(ui->lineEdit_fileName->text(), ui->lineEdit_String->text());
        ui->statusBar->showMessage("Search started");
    });
    connect(ui->button_cancel, &QPushButton::clicked, this, [this] {
        ui->statusBar->showMessage(("Canceled. Found " + std::to_string(ui->table_result->rowCount()) + " items").c_str());
        searcher.stop();
    });
    connect(&searcher, &Searcher::result_changed, this, [this]
    {
        std::vector<Searcher::result::result_item> res;
        searcher.get_result(res);
        for (size_t row = 0; row < res.size() && ui->table_result->rowCount() < MAX_TABLE_SIZE; ++row) {
            ui->table_result->insertRow(0);
            QTableWidgetItem *newitem_file = new QTableWidgetItem(0);
            QTableWidgetItem *newitem_pos = new QTableWidgetItem(0);
            QTableWidgetItem *newitem_buf = new QTableWidgetItem(0);

            newitem_file->setText(res[row].file_name);
            newitem_pos->setText(QString(res[row].pos));
            newitem_buf->setText(res[row].buf);

            ui->table_result->setItem(0, 0, newitem_file);
            ui->table_result->setItem(0, 1, newitem_pos);
            ui->table_result->setItem(0, 2, newitem_buf);
        }
        QString status;
        if(searcher.incomplete()) {
            status = "";
        } else {
            status = "Search ended. ";
        }
        status = status + "Found " + std::to_string(ui->table_result->rowCount()).c_str() + " items";
        //ui->statusBar->showMessage(("Found " + std::to_string(ui->table_result->rowCount()) + " items").c_str());
        ui->statusBar->showMessage(status);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
