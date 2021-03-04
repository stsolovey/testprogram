#include "chart.h"

Chart::Chart()
{

}

void Chart::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QChart::mousePressEvent(event);
}
