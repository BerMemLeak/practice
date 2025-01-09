#include "stopwatch.h"
#include <QDateTime>

Stopwatch::Stopwatch(QObject *parent)
    : QObject(parent), startTime(0), elapsedTime(0), lastLapTime(0), lapCount(0) {
    connect(&timer, &QTimer::timeout, this, &Stopwatch::updateTimer);
    timer.setInterval(100); // Обновление каждые 0.1 сек
}


void Stopwatch::start() {
    timer.start();
    isRunning = true;

}
bool Stopwatch::get_running_state() const{
    return isRunning;
};

void Stopwatch::stop() {
    timer.stop();
    isRunning = false;

}

void Stopwatch::reset() {
    timer.stop();
    elapsedTime = 0;
    lastLapTime = 0;
    lapCount = 0;
    emit timeUpdated("0.0");
    timer.start();

}

void Stopwatch::recordLap() {
    qint64 currentTime = elapsedTime;
    qint64 lapTime = currentTime - lastLapTime;  // Время прошедшее с последнего круга
    lastLapTime = currentTime;  // Обновляем время последнего круга

    lapCount++;  // Увеличиваем счетчик кругов
    emit lapRecorded(QString("Круг %1, время: %2 сек").arg(lapCount).arg(lapTime / 10.0, 0, 'f', 1));  // Отправляем сигнал
}


QString Stopwatch::getElapsedTime() const {
    return QString::number(elapsedTime / 1000.0, 'f', 1);
}

QString Stopwatch::getLapTime() const {
    return QString::number((elapsedTime - lastLapTime) / 1000.0, 'f', 1);
}

void Stopwatch::updateTimer() {
    elapsedTime += 1;  // Увеличиваем время на 0.1 секунды (10 для десятых долей)
    QString timeString = QString::number(elapsedTime / 10.0, 'f', 1);  // Форматируем строку для отображения (например, "0.0")
    emit timeUpdated(timeString);  // Отправляем обновленное время
}
