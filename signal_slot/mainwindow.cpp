#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stopwatch.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stopwatch(new Stopwatch(this)) {
    ui->setupUi(this);

    // Изначальные состояния
    ui->timeLabel->setText("0.0");
    ui->restart->setText("Сбросить");
    ui->start_stop->setText("Старт");
    ui->circle->setText("Круг");


    // Подключение сигналов и слотов
    connect(stopwatch, &Stopwatch::timeUpdated, ui->timeLabel, &QLabel::setText);
    connect(stopwatch, &Stopwatch::lapRecorded, ui->circleInfo, &QTextBrowser::append);

    connect(ui->start_stop, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(ui->restart, &QPushButton::clicked, [this]() {
        stopwatch->reset();
        ui->circleInfo->clear();
    });
    connect(ui->circle, &QPushButton::clicked, stopwatch, &Stopwatch::recordLap);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onStartStopClicked() {
    if (stopwatch->get_running_state()) {
        stopwatch->stop();
        ui->start_stop->setText("Старт");
        ui->circle->setEnabled(false);
    } else {
        stopwatch->start();
        ui->start_stop->setText("Стоп");
        ui->circle->setEnabled(true);
    }
}
