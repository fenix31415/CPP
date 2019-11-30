#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <searcher.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static const size_t MAX_TABLE_SIZE = 200;

private:
    Ui::MainWindow *ui;
    Searcher searcher;
};

#endif // MAINWINDOW_H
