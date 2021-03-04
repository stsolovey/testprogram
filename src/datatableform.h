#ifndef DATATABLEFORM_H
#define DATATABLEFORM_H

#include <QWidget>
#include "parameters.h"

namespace Ui {
class DataTableForm;
}

class DataTableForm : public QWidget
{
    Q_OBJECT
    QList<QPointF> points;
    Parameters parameters;


public:
    explicit DataTableForm(QWidget *parent = nullptr);
    ~DataTableForm();

    void saveToFile();
    void loadFromFile();
    void setPoints(const QList<QPointF> &value);
    void setParameters(const Parameters &value);
    void createDataTable();

private:
    Ui::DataTableForm *ui;
signals:
    void sendDataFromFile(Parameters parametersFromFile);

};

#endif // DATATABLEFORM_H
