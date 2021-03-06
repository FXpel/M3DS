#include <QApplication>
#include "MainWindow.h"

#include <iostream>
#include <QDir>
#include <QDebug>

#include <QSurfaceFormat>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

int main(int argc, char *argv[]) {

  QApplication application(argc,argv);
  QDir::setCurrent(QCoreApplication::applicationDirPath());

  QSurfaceFormat format;
  format.setVersion(3,3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);


  MainWindow mainWindow;
  mainWindow.show();


  int finished=application.exec();

  return finished;

}

