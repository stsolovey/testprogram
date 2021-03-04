#include "MainWindow.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QLineSeries>
#include <QVXYModelMapper>
#include <QtMath>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
 {
     setupUi(this);
     initCore();
 }

void MainWindow::initCore()
 {
     connect(startBtn,&QPushButton::clicked,this,&MainWindow::start);
     connect(pauseBtn,&QPushButton::clicked,this,&MainWindow::pause);
     connect(breakBtn,&QPushButton::clicked,this,&MainWindow::cancel);
     connect(functionCombo,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::comboBoxChanged);

     table = new DataTableForm();

     connect(table,&DataTableForm::sendDataFromFile,this,&MainWindow::receiveDataFromFile);

    m_chart=new Chart();
    m_chart->setAnimationOptions(QChart::AllAnimations);
    m_chart->setMargins(QMargins(0,0,0,0));
    m_chart->legend()->hide();
    connect(m_chart,&Chart::clicked,this,&MainWindow::setChartClicked);

    m_axisX=new QValueAxis();
    m_axisY=new QValueAxis();

    m_chart->addAxis(m_axisX,Qt::AlignBottom);
    m_chart->addAxis(m_axisY,Qt::AlignLeft);

    m_series=new QLineSeries();
    m_chart->addSeries(m_series);

    chartView->setChart(m_chart);

    setMode(ConfiguringMode);
    return;
 }

void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (QMessageBox::question(this,tr("Confirmation"),tr("Exit program?"),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes)
        event->accept();
     else
        event->ignore();
     return;
 }

void MainWindow::start()
 {
     if (currentMode==InterruptedMode)
         setMode(ConfiguringMode);
     else
     {
        setMode(CalculatingMode);
        propertiesInitialisation();
        seriesAppending();
     }
    return;
 }

void MainWindow::pause(bool pressed)
 {

     m_paused=pressed;
     setMode(pressed ? PausedMode : CalculatingMode);
     return;
 }

void MainWindow::cancel()
 {
     m_canceled=true;
     setMode(InterruptedMode);
     return;
 }

 void MainWindow::propertiesInitialisation()
 {

     m_canceled=false;
     m_paused=false;

     m_series->clear();

     parameters.A=aSpin->value();
     parameters.B=bSpin->value();
     parameters.C=cSpin->value();

     parameters.from=fromSpin->value();
     parameters.to=toSpin->value();
     parameters.step=stepSpin->value();

     parameters.currentFunction=functionCombo->currentIndex();

     m_axisX->setRange(parameters.from,parameters.to);
     if (!m_series->attachedAxes().contains(m_axisX))
        m_series->attachAxis(m_axisX);

     min = 65535;
     max = -65535;

     totalStepsCount = 0;
     currentStep = 0;
     totalStepsCalculation();
 }
 void MainWindow::receiveDataFromFile(Parameters parametersFromFile)
 {
     parameters = parametersFromFile;
     setParametersFromFile();
 }

 void MainWindow::setParametersFromFile()
 {
     aSpin->setValue(parameters.A);
     bSpin->setValue(parameters.B);
     cSpin->setValue(parameters.C);

     fromSpin->setValue(parameters.from);//+parameters.step*(parameters.points.size()+1));
     toSpin->setValue(parameters.to);
     stepSpin->setValue(parameters.step);

     totalStepsCalculation();

     functionCombo->setCurrentIndex(parameters.currentFunction);

     m_axisX->setRange(parameters.from,parameters.to);
     if (!m_series->attachedAxes().contains(m_axisX))
         m_series->attachAxis(m_axisX);

     min = 65535;
     max = -65535;

     for (auto a : qAsConst(parameters.points))
     {
         min=(min<a.y()) ? min : a.y();
         max=(max>a.y()) ? max : a.y();
     }

     if (m_series->attachedAxes().contains(m_axisY))
         m_series->detachAxis(m_axisY);

     m_axisY->setRange(min,max);
     m_series->attachAxis(m_axisY);

     m_series->clear();
     for (auto a : qAsConst(parameters.points))
         m_series->append(a);

     m_paused=false;
     m_canceled=false;
     parameters.from=parameters.from+parameters.step*(parameters.points.size()+1);

     setMode(PausedMode);
     m_paused=true;
     pauseBtn->setChecked(true);


     currentStep = parameters.points.size();

     makingProgressTextForStartButton();
     seriesAppending();
 }

 void MainWindow::seriesAppending()
 {
     for (X=parameters.from;X<parameters.to;X+=parameters.step)
     {
         while (m_paused)
             qApp->processEvents();

         if (m_canceled)
         {
             //setMode(InterruptedMode);
             break;
         }

         funcionsCalculation();
         renewAxisY();

         m_series->append(X,Y);

         makingProgressTextForStartButton();

         quint64 now=QDateTime::currentMSecsSinceEpoch();
         while (QDateTime::currentMSecsSinceEpoch()-now<1)
             qApp->processEvents();
     }
 }
 void MainWindow::totalStepsCalculation()
 {
     double lenght(abs(parameters.to-parameters.from));

     totalStepsCount = ceil(lenght / parameters.step);

 }
 void MainWindow::makingProgressTextForStartButton()
 {
    currentStep++;
    double calculationProgress=static_cast<double>(currentStep)/static_cast<double>(totalStepsCount)*100;
    progress = "Progress %1";
    startBtn->setText(progress.arg(calculationProgress, 0, 'f', 1)+"%");
 }

 void MainWindow::funcionsCalculation()
 {
     switch (functionCombo->currentIndex())
     {
         case 0:
         {
             Y=parameters.A*(X*X) + parameters.B*X + parameters.C;
             break;
         }
         case 1:
         {
             Y=parameters.A * sin(X) + parameters.B * cos( parameters.C*X );
             break;
         }
         case 2:
         {
             Y=parameters.A*log( parameters.B*X );
             break;
         }
         case 3:
         {
             Y=parameters.A / ( sin(X*X) * parameters.B );
             break;
         }
         default:
         {
             Y=X;
             break;
         }
     }
 }

 void MainWindow::renewAxisY()
 {
     min=(min<Y) ? min : Y;
     max=(max>Y) ? max : Y;

     if (min!=m_axisY->min() || max!=m_axisY->max())
     {
         m_series->detachAxis(m_axisY);
         m_axisY->setRange(min,max);
     }

     if (!m_series->attachedAxes().contains(m_axisY))
         m_series->attachAxis(m_axisY);
 }

 void MainWindow::setChartClicked()
 {
     QTableWidgetItem item;

     table->setParameters(parameters);
     table->setPoints(m_series->points());
     table->createDataTable();
     table->show();
 }

 void MainWindow::setMode(Mode mode)
 {
     currentMode = mode;

     if (mode == ConfiguringMode) {
         m_series->clear();
         functionCombo->setEnabled(true);
         aSpin->setEnabled(true);
         bSpin->setEnabled(true);
         cSpin->setEnabled(true);

         fromSpin->setEnabled(true);
         toSpin->setEnabled(true);
         stepSpin->setEnabled(true);

         startBtn->setEnabled(true);
         pauseBtn->setEnabled(false);
         breakBtn->setEnabled(false);

         startBtn->setText(tr("Start"));
     } else if (mode == CalculatingMode) {
         functionCombo->setEnabled(false);
         aSpin->setEnabled(false);
         bSpin->setEnabled(false);
         cSpin->setEnabled(false);

         fromSpin->setEnabled(false);
         toSpin->setEnabled(false);
         stepSpin->setEnabled(false);

         startBtn->setEnabled(false);
         pauseBtn->setEnabled(true);
         breakBtn->setEnabled(true);

         startBtn->setText(tr("% done"));
         pauseBtn->setText(tr("Pause"));
     } else if (mode == PausedMode) {
         functionCombo->setEnabled(false);
         aSpin->setEnabled(false);
         bSpin->setEnabled(false);
         cSpin->setEnabled(false);

         fromSpin->setEnabled(false);
         toSpin->setEnabled(false);
         stepSpin->setEnabled(false);

         startBtn->setEnabled(false);
         pauseBtn->setEnabled(true);
         breakBtn->setEnabled(true);

         pauseBtn->setText(tr("Continue"));
     } else if (mode == InterruptedMode) {
         functionCombo->setEnabled(false);
         aSpin->setEnabled(false);
         bSpin->setEnabled(false);
         cSpin->setEnabled(false);

         fromSpin->setEnabled(false);
         toSpin->setEnabled(false);
         stepSpin->setEnabled(false);

         startBtn->setEnabled(true);
         pauseBtn->setEnabled(false);
         breakBtn->setEnabled(false);

         startBtn->setText(tr("New"));
     }

 }

 void MainWindow::comboBoxChanged()
 {
     parameters.currentFunction = functionCombo->currentIndex();
 }
