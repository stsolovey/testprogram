#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "chart.h"
//class QValueAxis;
#include <QValueAxis>
#include <QLineSeries>
#include "parameters.h"
#include "datatableform.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
 {
 Q_OBJECT
 public:
  MainWindow(QWidget *parent = 0);
  /* Режим работы программы */
  enum Mode {
      /* Ввод пользователем параметров */
      ConfiguringMode,
      /* В процессе расчета */
      CalculatingMode,
      /* На паузе */
      PausedMode,
      /* Расчет прекращен, результаты доступны */
      InterruptedMode,
      };

 protected:
  void initCore();
  void initIface();
  void closeEvent(QCloseEvent *event);
  void setChartClicked();
  void saveToFile();
  void totalStepsCalculation();


private:
  bool m_paused;
  bool m_canceled;


  double min;
  double max;

  double Y;
  double X;

  double calculationProgress;

  int totalStepsCount;
  int currentStep;

  QString progress;
  Chart *m_chart;
  QValueAxis *m_axisX;
  QValueAxis *m_axisY;
  QLineSeries *m_series;
  Mode currentMode;
  Parameters parameters;
  DataTableForm *table;


protected slots:

  void start();
  void pause(bool);
  void cancel();

  void comboBoxChanged();

  void propertiesInitialisation();
  void setParametersFromFile();
  void funcionsCalculation();
  void renewAxisY();
  void seriesAppending();
  void makingProgressTextForStartButton();
  void setMode(Mode mode);


  public slots:
    void receiveDataFromFile(Parameters parametersFromFile);


 };

#endif // MAINWINDOW_H
