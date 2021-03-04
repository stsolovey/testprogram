#include "datatableform.h"
#include "ui_datatableform.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

DataTableForm::DataTableForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTableForm)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose,true);
    connect(ui->saveBtn ,&QPushButton::clicked,this,&DataTableForm::saveToFile);
    connect(ui->loadBtn ,&QPushButton::clicked,this,&DataTableForm::loadFromFile);
}

void DataTableForm::createDataTable()
{
    int c = 0;
    QList<QPointF>::iterator i;
    ui->dataTbl->setRowCount(points.size());
    for (i = points.begin(); i != points.end(); ++i)
    {
        ui->dataTbl->setItem(c, 0, new QTableWidgetItem(QString::number(i->x())));
        ui->dataTbl->setItem(c, 1, new QTableWidgetItem(QString::number(i->y())));
        c++;
    }
}

DataTableForm::~DataTableForm()
{
    delete ui;
}
void DataTableForm::saveToFile()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Select file"),qApp->applicationDirPath(),tr("Text files (*.txt)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox::critical(this,tr("Error"),tr("Can't open file"),QMessageBox::Ok);
        return;
    }

    QTextStream stream(&file);
    stream << parameters.currentFunction << "\n";
    stream << parameters.A<<"\n";
    stream << parameters.B<<"\n";
    stream << parameters.C<<"\n";
    stream << parameters.from<<"\n";
    stream << parameters.to<<"\n";
    stream << parameters.step<<"\n";
    stream << parameters.points.size()<<"\n";
    QList<QPointF>::iterator i;
    for (i = points.begin(); i != points.end(); ++i)
        stream<<i->x()<<";"<<i->y()<<"\n";
    file.close();

    return;
}

void DataTableForm::loadFromFile()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("Select file"),qApp->applicationDirPath(),tr("Text files (*.txt)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,tr("Error"),tr("Can't open file"),QMessageBox::Ok);
        return;
    }

    QByteArray data;
    data=file.readLine();

    parameters.currentFunction = data.toInt();
    data=file.readLine();

    parameters.A = data.toFloat();
    data=file.readLine();

    parameters.B = data.toFloat();
    data=file.readLine();

    parameters.C = data.toFloat();

    data=file.readLine();

    parameters.from = data.toFloat();
    data=file.readLine();

    parameters.to = data.toFloat();
    data=file.readLine();

    parameters.step = data.toFloat();

    data=file.readLine();

    parameters.sizeOfSeries = data.toInt();

    QList<QByteArray> list;
    float X = 0;
    float Y = 0;
    QPoint point;
    ui->dataTbl->clear();
    while (!file.atEnd())
    {
        data=file.readLine();

        list.clear();
        list << data.trimmed().split(';');

        if (list.size()>0)
            X=list.at(0).toFloat();
        if (list.size()>1)
            Y=list.at(1).toFloat();
        point.setX(X);
        point.setY(Y);
        parameters.points.append(point);
        ui->dataTbl->setRowCount(ui->dataTbl->rowCount()+1);
        int currentRow=ui->dataTbl->rowCount()-1;
        ui->dataTbl->setItem(currentRow, 0, new QTableWidgetItem(QString::number(X)));
        ui->dataTbl->setItem(currentRow, 1, new QTableWidgetItem(QString::number(Y)));
    }

    file.close();
    emit sendDataFromFile(parameters);
    return;
}
void DataTableForm::setPoints(const QList<QPointF> &value)
{
    points = value;
}

void DataTableForm::setParameters(const Parameters &value)
{
    parameters = value;
}
