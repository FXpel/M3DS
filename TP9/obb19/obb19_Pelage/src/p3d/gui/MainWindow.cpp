#include <memory>


#include "MainWindow.h"

#include <QGridLayout>
#include <QGLFormat>
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>

#include <QDebug>

#include <iostream>

#include "Tools.h"
#include "GLApplication.h"


using namespace std;

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

MainWindow::~MainWindow() {
}

MainWindow::MainWindow() :QMainWindow(nullptr) {

  // build the GLApplication widget

  glApplication_=make_unique<GLApplication>();

  // application buttons in left panel
  leftPanelButton_.clear();
  for(QString &s:glApplication_->leftPanelMenu_) {
    leftPanelButton_.push_back(new QPushButton(s));
  }


  // set up for the main window : set a central widget (main container), and set a gridLayout to this central widget
  QWidget *central=new QWidget(this);
  this->setCentralWidget(central);
  QGridLayout *layout=new QGridLayout(central);

  // add widgets in layout
  int row=0;
  for(QPushButton *b:leftPanelButton_) {
    layout->addWidget(b,row,0);
    connect(b,SIGNAL(clicked()),this,SLOT(leftPanelSlot()));
    row++;
  }
  layout->addWidget(glApplication_.get(),0,1,20,20);

  QPushButton *snap=new QPushButton("snapshot");
  layout->addWidget(snap,19,0);
  connect(snap,SIGNAL(clicked()),this,SLOT(snapshotSlot()));

  // update GLWidget every 20ms
  QTimer *timer=new QTimer(this);
  connect(timer,SIGNAL(timeout()),glApplication_.get(),SLOT(updateData())); // calls GLView::update() (that will call paintGL() )
  timer->start(20);

//  if (!_leftPanelButton.empty()) _leftPanelButton[0]->click();

}


void MainWindow::leftPanelSlot() {
  QPushButton *b=dynamic_cast<QPushButton *>(QObject::sender());
  unsigned int clicked=0;
  for(auto &bb:leftPanelButton_) if (b==bb) break; else ++clicked;
  glApplication_->leftPanel(clicked,b->text().toStdString());
}

void MainWindow::snapshotSlot() {
  glApplication_->requestSnapshot();
}


// key events
void MainWindow::keyPressEvent(QKeyEvent *e) {
  if (e->key()==Qt::Key_Escape) {
    this->close();
  }
  else {
    QWidget::keyPressEvent(e); // send the event to the parent widget
  }
}


