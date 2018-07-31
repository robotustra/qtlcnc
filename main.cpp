#include <QApplication>
#include <QMessageBox>
#include <QWidget>
#include <QDesktopWidget>
#include <QSize>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.set_ggeom(24,16);

    // load config


    // determine screen size.
    QSize qs = w.get_screen_size();

    
    /*QMessageBox mb;
    mb.setText(QString (" The screen size ") + QString::number( qs.width() ) + "x" + QString::number( qs.height()) );
    mb.exec();
    w.set_window_size(qs);
    */

    QRect ws = a.desktop()->availableGeometry();

    int hdelta = qs.height() - ws.height(); // the sizes are different by the height of menu

    w.set_layout_params(QPoint(10,18), 80);

    w.set_window_size(QSize(ws.width(), ws.height()), QPoint(0, -hdelta));

    return a.exec();
}
