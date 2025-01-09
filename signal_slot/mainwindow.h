#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

namespace Ui {
class MainWindow;
}

// тут вся логика кроме плдсчета времени и коннекторы

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onStartStopClicked();

private:
    Ui::MainWindow *ui;
    Stopwatch *stopwatch; // Указатель на Stopwatch
};

#endif // MAINWINDOW_H
