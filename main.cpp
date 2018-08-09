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
    QString fname = "";


    if (argc == 2){
        fname = argv[1];
    }else{
        fname = "../qtlcnc/qtlcnc_layouts.ini";
    }
    qDebug() << fname;
    int r = w.load_config(fname);
    if (r<0){
        perror("Can't load any layout file of file is incorrect\n");
        exit(1);
    }
    // after that we already have some data for screen.
    // first, check if we have to setup screen automatically.

    int nxcell = w.get_int_value_from_data("xgrid");
    //qDebug()<< "xgrid =" << nxcell;
    if (nxcell < 0) nxcell = 18; // if something wrong in config.

    int nycell = w.get_int_value_from_data("ygrid");
    //qDebug()<< "ygrid =" << nycell;
    if (nycell < 0) nycell = 12; //default

    int ucell = w.get_int_value_from_data("ucell");
    //qDebug()<< "ucell =" << ucell;
    if (ucell < 0) ucell = 80;

    int autoconf_display = w.get_int_value_from_data("lauto"); //the name of variable is fixed.
    if (autoconf_display == 1 ){
        // derive values from screen display
        // recalculate ucell from the smallest value of the
        // for now we do not overrade set parameters
    }
    w.set_ggeom(nxcell, nycell);

    // determine screen size.
    //QSize qs = w.get_screen_size();

    QRect ws = a.desktop()->availableGeometry();

    //int hdelta = qs.height() - ws.height(); // the sizes are different by the height of menu

    int xoffs = w.get_int_value_from_data("xoffs");
    if (xoffs < 0) xoffs = 2;
    int yoffs = w.get_int_value_from_data("yoffs");
    if (yoffs < 0) yoffs = 18;

    w.set_layout_params(QPoint(xoffs, yoffs), ucell);

    //w.set_window_size(QSize(ws.width(), ws.height()), QPoint(0, -hdelta));
    int wds = nxcell * ucell;
    int hds = nycell * ucell;
    int stat_bar = w.get_int_value_from_data("statoffs");
    //qDebug()<< "statoffs =" << stat_bar;
    if (stat_bar < 0) stat_bar = 38;

    // doing automatic placement of the window, ignoring geometry for now from ini file
    w.set_window_size(QSize(wds, hds+ stat_bar), QPoint((ws.width()-wds)/2, (ws.height()-hds)/2));

    return a.exec();
}
