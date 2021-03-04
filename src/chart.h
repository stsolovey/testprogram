#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QChart>
#include <QMouseEvent>

QT_CHARTS_USE_NAMESPACE

class Chart : public QChart
{
    Q_OBJECT
public:
    Chart();
signals:
    void clicked();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CHART_H
