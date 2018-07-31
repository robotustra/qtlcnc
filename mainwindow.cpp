#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QWidget>
#include <QDesktopWidget>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    QPixmap pm = QPixmap::grabWindow( QApplication::desktop()->winId());

    QMessageBox mb;
    mb.setText(QString (" The screen size ") +
               QString::number( pm.width() ) + "x" + QString::number( pm.height()) );
    mb.exec();


}


MainWindow::~MainWindow()
{
    delete ui;
}
