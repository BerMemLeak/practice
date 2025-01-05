#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(10);
    ui->progressBar->setValue(0);
    ui->pushButton->setCheckable(true);
    ui->first_rb->setText("Раз");
    ui->second_rb->setText("Два");
    ui->groupBox->setTitle("Выберите один");
    for (auto i = 0; i < 10; i++){
        ui->chooseItemBox->addItem(QString::number(i));
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int value = ui->progressBar->value();
    if(value == 10){
        ui->progressBar->setValue(0);
    }
    else{
        value += 1;
        ui->progressBar->setValue(value);
    }

}

