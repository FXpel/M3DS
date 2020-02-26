#pragma once

#include "glsupport.h"
#include <memory>
#include <QMainWindow>
#include <QPushButton>


/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

class GLApplication;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow();
  ~MainWindow();

  void keyPressEvent(QKeyEvent *e);

signals:
  
public slots:
  void leftPanelSlot();
  void snapshotSlot();
private:
  std::vector<QPushButton *> leftPanelButton_;
  std::unique_ptr<GLApplication> glApplication_;

};


