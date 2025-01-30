#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject {
    Q_OBJECT

public:
    explicit Stopwatch(QObject *parent = nullptr);
    void start();
    void stop();
    void reset();
    QString recordLap();

    QString getElapsedTime() const;
    QString getLapTime() const;
    bool get_running_state() const;

signals:
    void timeUpdated(const QString &time); // Сигнал для обновления времени
    void resetting();


private slots:
    void updateTimer();


private:
    QTimer timer;
    qint64 startTime;       // Время запуска
    qint64 elapsedTime;     // Общее прошедшее время
    qint64 lastLapTime;     // Время последнего круга
    int lapCount;           // Количество кругов
    bool isRunning = false;

};

#endif // STOPWATCH_H
