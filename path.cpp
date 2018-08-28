#include "path.h"
#include <QDebug>



Path2D::Path2D(QStringList &sl)
{
    p = sl;
}

void Path2D::drawPath(QPainter& painter, QString& pencolor, QString& bgcolor, QPoint * pos, QPoint * sz, int ucell){
     //
    qDebug() << "drawing" << p << " with pen " << pencolor << " on bground " << bgcolor;
    qDebug() << "at pos = " << pos->x() * ucell << ", " << pos->y() * ucell;
    qDebug() << "with size = " << sz->x() * ucell << ", " << sz->y() * ucell;

    painter.save();

    QBrush brush = QBrush(QColor(bgcolor.replace("\"","")));
    painter.setBrush(brush);
    QPen pen = QPen(QColor(pencolor.replace("\"", "")), 2.0, Qt::SolidLine);
    painter.setPen(pen);

    float x0 = (pos->x()-1) * ucell;
    float y0 = (pos->y()-1) * ucell;
    float sx = sz->x() * ucell;
    float sy = sz->y() * ucell;

    // this is an area to which we draw the path.
    painter.drawRect( x0, y0, sx, sy);

    //Mini parser to draw pathes
    QPainterPath path;
    for(int i = 0; i<p.size(); i++){
        /*if(p[i].contains("M")){ // moveTo
            // look for the next space or tab of eol
            QString tmp = p[i].remove("\"");
            tmp.remove("M");
            QStringList crd = tmp.split(",");
            float x_coor = crd[0].toFloat();
            float y_coor = crd[1].toFloat();
            qDebug()<< "Move to: x_coord = " << x_coor << "y_coord =" <<y_coor;
            // cut the beginning of the string
            path.moveTo(x0+x_coor, y0+y_coor);
        }
        if(p[i].contains("L")){ //lineTo
            // look for the next space or tab of eol
            QString tmp = p[i].remove("\"");
            tmp.remove("L");
            QStringList crd = tmp.split(",");
            float x_coor = crd[0].toFloat();
            float y_coor = crd[1].toFloat();
            qDebug()<< "Line to: x_coord = " << x_coor << "y_coord =" <<y_coor;
            // cut the beginning of the string
            path.lineTo(x0+x_coor, y0+y_coor);
        }*/
        if(p[i].contains("A")){ //To
            // look for the next space or tab of eol
            QString tmp = p[i].remove("\"");
            tmp.remove("A");
            QStringList crd = tmp.split(",");
            float vx = crd[0].toFloat(); // directing vector
            float vy = crd[1].toFloat();
            float angle = crd[2].toFloat();
            float rad = crd[3].toFloat();
            float start_angle = 0;
            // directing vector
            if (vx == 1 && vy == 0) start_angle = 90.0;
            if (vx == -1 && vy == 0) start_angle = 270.0;
            if (vx == 0 && vy == 1) start_angle = 0.0;
            if (vx == 0 && vy == -1) start_angle = 180.0;

            qDebug()<< "arc to: vx=" << vx << "vy=" << vy << "a=" << angle << " rad= " <<rad;
            // cut the beginning of the string

            //path.arcTo(x0, y0, 2*rad, 2*rad, start_angle, start_angle + angle);
            //path.arcTo(100, 100, 50, 50, 0, -90);
            path.moveTo(80.0, 35.0);
            path.arcTo(70.0, 30.0, 10.0, 10.0, 0.0, 90.0);
            path.lineTo(25.0, 30.0);
            path.arcTo(20.0, 30.0, 10.0, 10.0, 90.0, 90.0);
            path.lineTo(20.0, 65.0);
            path.arcTo(20.0, 60.0, 10.0, 10.0, 180.0, 90.0);
            path.lineTo(75.0, 70.0);
            path.arcTo(70.0, 60.0, 10.0, 10.0, 270.0, 90.0);
            //http://doc.qt.io/qt-5/qtwidgets-painting-painterpaths-example.html
        }
    }
    painter.drawPath(path);

    painter.restore();

    /*
    static const QPoint points[4] = {
        QPoint(10, 10),
        QPoint(90, 10),
        QPoint(90, 90),
        QPoint(10, 90)
    };
    */
    // example
    //painter.drawRect(0, 0, 100,100);
    //QRect rect(10, 20, 80, 60);


    //QRect rect(0, 0, ucell-border, ucell-border);


    /*
    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QLinearGradient lg(0,0,100,100);
    */
    //brush = QBrush("gray", Qt::LinearGradientPattern);
    /*brush = QBrush(Qt::NoBrush);
    painter.setBrush(brush);
    */
    /*
    shape = Rect;
    painter.setRenderHint(QPainter::Antialiasing, true);
    */
    /*for (int x = 0; x < ggeom.width() * ucell; x += ucell) {
        for (int y = 0; y < ggeom.height() * ucell; y += ucell) {
            painter.save();
            painter.translate(loffset.x() + x, loffset.y() + y);
            / *
            if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }* /

            switch (shape) {
            case Line:
                painter.drawLine(rect.bottomLeft(), rect.topRight());
                break;
            case Points:
                //painter.drawPoints(points, 4);
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
    }*/


}
