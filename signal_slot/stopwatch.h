#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>


 // В классе обязательно должны быть реализованы методы запуска, остановки, сброса секундомера.
 // у меня тут метода запуска остаоновки сброса и геттер времени секундомера

class Stopwatch
{
public:
    Stopwatch();
    ~Stopwatch();
    void start();
    void stop();
    auto time_getter();
    void end();

};

#endif // STOPWATCH_H
