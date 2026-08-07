#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendToGUI, this, &MainWindow::DisplayTime);
    connect(ui->pushDiagram,&QPushButton::clicked, this,[this]{
        auto text = ui->lineEditMess->text();
        if(text.isEmpty()){return;}
        udpWorker->SendDatagram(text.toUtf8());
    });

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QString sender, int size)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    // ui->te_result->append("Текущее время: " + data.toString() + ". "
    //             "Принято пакетов " + QString::number(counterPck));
    ui->te_result->append("Принято сообщение от " +sender+ ". "
                ", размер сообщения(байт)   " + QString::number(size));


}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

