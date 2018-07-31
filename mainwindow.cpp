#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ggeom.setWidth(0);
    ggeom.setHeight(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSize MainWindow::get_screen_size(){

    QPixmap pm = QPixmap::grabWindow( QApplication::desktop()->winId());

    /*
    QMessageBox mb;
    mb.setText(QString (" The screen size ") +
               QString::number( pm.width() ) + "x" + QString::number( pm.height()) );
    mb.exec();*/

    return QSize(pm.width(), pm.height());
}

void MainWindow::set_window_size(QSize s, QPoint pt){
    this->resize(s);
    this->move(pt);

    screen_size = QSize(s.width(),s.height()+pt.y()); //save size for future.
}

void MainWindow::paintEvent(QPaintEvent * e){
    e = e;
    static const QPoint points[4] = {
        QPoint(10, 10),
        QPoint(90, 10),
        QPoint(90, 90),
        QPoint(10, 90)
    };
    // example
    //painter.drawRect(0, 0, 100,100);
    //QRect rect(10, 20, 80, 60);

    QRect rect(0, 0, ucell-4, ucell-4);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QPainter painter(this);
    //pen = QPen("darkgreen");
    pen = QPen(QColor("black"), 1.5);

    painter.setPen(pen);

    QLinearGradient lg(0,0,100,100);

    //brush = QBrush("gray", Qt::LinearGradientPattern);
    brush = QBrush(Qt::NoBrush);
    painter.setBrush(brush);

    shape = Rect;
    painter.setRenderHint(QPainter::Antialiasing, true);

    transformed = false;
    for (int x = 0; x < ggeom.width() * ucell; x += ucell) {
        for (int y = 0; y < ggeom.height() * ucell; y += ucell) {
            painter.save();
            painter.translate(loffset.x() + x, loffset.y() + y);
            /*
            if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }*/

            switch (shape) {
            case Line:
                painter.drawLine(rect.bottomLeft(), rect.topRight());
                break;
            case Points:
                painter.drawPoints(points, 4);
                break;
            case Polyline:
                painter.drawPolyline(points, 4);
                break;
            case Polygon:
                painter.drawPolygon(points, 4);
                break;
            case Rect:
                painter.drawRect(rect);
                break;
            case RoundedRect:
                painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
                break;
            case Ellipse:
                painter.drawEllipse(rect);
                break;
            case Arc:
                painter.drawArc(rect, startAngle, arcLength);
                break;
            case Chord:
                painter.drawChord(rect, startAngle, arcLength);
                break;
            case Pie:
                painter.drawPie(rect, startAngle, arcLength);
                break;
            case Path:
                painter.drawPath(path);
                break;
            case Text:
                painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));
                break;
            case Pixmap:
                painter.drawPixmap(10, 10, pixmap);
            }
            painter.restore();
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

}

void MainWindow::set_ggeom(int w, int h){
    ggeom.setWidth(w);
    ggeom.setHeight(h);
}

void MainWindow::set_layout_params(QPoint p, int uc){
    loffset = p;
    ucell = uc;
}
