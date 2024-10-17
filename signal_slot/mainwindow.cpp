#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    bool isRunning = false;
    ui->setupUi(this);
    ui->circle->setText("Круг");
    ui->timeLabel->setText("0.0");
    ui->restart->setText("Сбросить");
    ui->start_stop->setText("Старт");
    connect(ui->start_stop, &QPushButton::clicked, this, [&]{
        ui->start_stop->setText("Старт");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
