#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

void UDPworker::InitReplySocket()
{
    // Сокет для ответа — новый
    recieveUdpSocket = new QUdpSocket(this);
    recieveUdpSocket->bind(QHostAddress::LocalHost, 0);
    connect(recieveUdpSocket, &QUdpSocket::readyRead,
            this, &UDPworker::onReplyReceived);
}

void UDPworker::onReplyReceived()
{
    while (recieveUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = recieveUdpSocket->receiveDatagram();

        QString sender = datagram.senderAddress().toString();
        int size = datagram.data().size();

        emit sig_sendReplyToGUI(sender, size);
    }
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}


void UDPworker::SendUserMessage(const QByteArray &data)
{
    recieveUdpSocket->writeDatagram(
        data,
        QHostAddress::LocalHost,
        recieveUdpSocket->localPort()
        );
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}
