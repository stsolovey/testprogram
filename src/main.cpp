#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
 {
 QApplication App(argc, argv);

 MainWindow MainWnd;
 MainWnd.show();
 return App.exec();
 }
