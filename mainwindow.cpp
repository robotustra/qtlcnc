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
    /*static const QPoint points[4] = {
            QPoint(10, 80),
            QPoint(20, 10),
            QPoint(80, 30),
            QPoint(90, 70)
        };*/

    static const QPoint points[4] = {
                QPoint(10, 10),
                QPoint(90, 10),
                QPoint(90, 90),
                QPoint(10, 90)
            };

        //painter.drawRect(0, 0, 100,100);

        //QRect rect(10, 20, 80, 60);
        QRect rect(0, 0, 95, 95);

        QPainterPath path;
        path.moveTo(20, 80);
        path.lineTo(20, 30);
        path.cubicTo(80, 0, 50, 50, 80, 80);

        int startAngle = 20 * 16;
        int arcLength = 120 * 16;

        QPainter painter(this);
        pen = QPen("darkgreen");
        painter.setPen(pen);

        QLinearGradient lg(0,0,100,100);

        brush = QBrush("gray", Qt::LinearGradientPattern);
        painter.setBrush(brush);

        shape = Rect;
        painter.setRenderHint(QPainter::Antialiasing, true);

        transformed = false;
        for (int x = 0; x < width(); x += 100) {
            for (int y = 0; y < height(); y += 100) {
                painter.save();
                painter.translate(x, y);
                if (transformed) {
                    painter.translate(50, 50);
                    painter.rotate(60.0);
                    painter.scale(0.6, 0.9);
                    painter.translate(-50, -50);
                }

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
