#ifndef PARAMETERS_H
#define PARAMETERS_H
//#include <QVector>
//#include <QPainter>
#include <QWidget>

class Parameters
{
public:
    Parameters();
    double A;
    double B;
    double C;

    double from;
    double to;
    double step;

    int currentIndex;
    int sizeOfSeries;
    int currentFunction;

    QList<QPointF> points;
};

#endif // PARAMETERS_H
