#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void InitReplySocket(void);
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
    void SendUserMessage(const QByteArray &data);



private slots:
    void readPendingDatagrams(void);
    void onReplyReceived(void);


private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* recieveUdpSocket;


signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendReplyToGUI(const QString &sender, int size);
};

#endif // UDPWORKER_H
