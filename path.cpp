#include "path.h"
#include <QDebug>
#include"layoutdata.h"



Path2D::Path2D(QStringList &sl, LayoutData *ld)
{
    p = sl;
    ld_local = ld;
}

void Path2D::drawPath(QPainter& painter, QString& pencolor, QString& bgcolor, QPoint * pos, QPoint * sz, int ucell, QPoint& loffset){
     //
    //qDebug() << "drawing" << p << " with pen " << pencolor << " on bground " << bgcolor;
    //qDebug() << "at pos = " << pos->x() * ucell << ", " << pos->y() * ucell;
    //qDebug() << "with size = " << sz->x() * ucell << ", " << sz->y() * ucell;

    painter.save();
    float x0 = (pos->x()-1) * ucell + loffset.x();
    float y0 = (pos->y()-1) * ucell + loffset.y();
    float sx = sz->x() * ucell;
    float sy = sz->y() * ucell;
    float sc_x = sx/100.0;
    float sc_y = sy/100.0;

    painter.setRenderHint(QPainter::Antialiasing); //improve graphics!

    QBrush brush = QBrush(QColor(bgcolor.replace("\"","")));
    painter.setBrush(brush);
    QPen pen = QPen(QColor(pencolor.replace("\"", "")), 2.0*ucell/100.0, Qt::SolidLine);
    painter.setPen(pen);


    // this is an area to which we draw the path.
    //painter.drawRect( x0, y0, sx, sy);

    //Mini parser to draw pathes !!! Must remake this parser if I want to add TEXT on buttons.
    QPainterPath path;
    for(int i = 0; i<p.size(); i++){
        QString p_str = p[i];
        p_str.remove(p_str.size()-1,1);
        p_str.remove(0,1);

        if(p_str.at(0) == QChar('M')){ // moveTo
            // look for the next space or tab of eol
            QString tmp = p_str;
            tmp.remove("M");
            QStringList crd = tmp.split(",");
            float x_coor = crd[0].toFloat() * sc_x;
            float y_coor = crd[1].toFloat() * sc_y;

            //qDebug()<< "Move to: x_coord = " << x_coor << "y_coord =" <<y_coor;
            // cut the beginning of the string
            path.moveTo(x0+x_coor, y0+y_coor);
        }
        if(p_str.at(0) == QChar('L')){ //lineTo
            // look for the next space or tab of eol
            QString tmp = p_str;
            tmp.remove("L");
            QStringList crd = tmp.split(",");
            float x_coor = crd[0].toFloat() * sc_x;
            float y_coor = crd[1].toFloat() * sc_y;
            //qDebug()<< "Line to: x_coord = " << x_coor << "y_coord =" <<y_coor;
            // cut the beginning of the string
            path.lineTo(x0+x_coor, y0+y_coor);
        }
        if(p_str.at(0) == QChar('A')){ //arcTo
            // look for the next space or tab of eol
            QString tmp = p_str;
            tmp.remove("A");
            QStringList crd = tmp.split(",");
            float x = crd[0].toFloat() * sc_x; // directing vector
            float y = crd[1].toFloat() * sc_y;
            float w = crd[2].toFloat() * sc_x;
            float h = crd[3].toFloat() * sc_y;
            float ab = crd[4].toFloat();
            float ae = crd[5].toFloat();

            //qDebug()<< "arc to: x=" << x << "y=" << y << "w=" << w << " h= " << h;
            // cut the beginning of the string
            path.arcTo(x0+x, y0+y, w, h, ab, ae);
            //http://doc.qt.io/qt-5/qtwidgets-painting-painterpaths-example.html
        }
        if(p_str.at(0) == QChar('R')){ //rectangle
            QString tmp = p_str;
            tmp.remove("R");
            QStringList crd = tmp.split(",");
            float x = crd[0].toFloat() * sc_x; // directing vector
            float y = crd[1].toFloat() * sc_y;
            float w = crd[2].toFloat() * sc_x;
            float h = crd[3].toFloat() * sc_y;

            path.addRect(x0+x, y0+y, w, h);

        }
        if(p_str.at(0) == QChar('P')){ //rounded rectangle
            QString tmp = p_str;
            tmp.remove("P");
            QStringList crd = tmp.split(",");
            float x = crd[0].toFloat() * sc_x; // directing vector
            float y = crd[1].toFloat() * sc_y;
            float w = crd[2].toFloat() * sc_x;
            float h = crd[3].toFloat() * sc_y;
            float rx = crd[4].toFloat() * sc_x;
            float ry = crd[5].toFloat() * sc_y;

            path.addRoundedRect(x0+x, y0+y, w, h,rx,ry);

        }

        if(p_str.at(0) == QChar('T')){ //rounded rectangle
            QString tmp = p_str;
            tmp.remove(0,1); // remove T
            // format "T<x>,<y>,<font_family>,<size>,<text>"
            QStringList crd = tmp.split(",");
            float x = crd[0].toFloat() * sc_x; // directing vector
            float y = crd[1].toFloat() * sc_y;
            QString ffont = crd[2];
            int h = crd[3].toInt() * sc_x;
            QString txt = crd[4];
            // test could be a name of variable, se we do look up
            if (ld_local !=NULL){
                int idx = ld_local->is_var_exist(txt);
                if(idx>= 0) {
                    txt = ld_local->get_string_value_by_name(txt);
                    removeQuotes(txt);
                }
            }
            QFont fnt = QFont(ffont,h, 1);

            path.addText(x0+x,y0+y,fnt, txt);

        }
        if(p_str.at(0) == QChar('X')){ //closeSubpath
            path.closeSubpath();
        }
    }
    painter.drawPath(path);

    painter.restore();
    /*
    # 1) "10,20M" means  move(10,20)
    # 2) "30,40L" means  lineTo(30,40)
    # 3) "85,5,10,10,0,90A"	 Arc, bounded with rectangular with the left top corner at (85,5), of size (10,10) and the angle from 0 to 90
    # 5) "1,2,30,40E" means closed circle or ellipse on the rectangle box
    # 6) "1,2,30,40R" means rectangle
    # 7) "1,2,4C"  means circle with the radius 4 and the center point (1,2)
    # 8) "1,2,30,40,5P" means rectangle with rounded corners of radius 5
    # 9) "X" means path close, useful in some cases.
    */

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

void Path2D::removeQuotes( QString & str){
    if (str.size() >= 2) {
        if ((str[str.size()-1] == QChar('\"')) || (str[str.size()-1] == QChar('\''))) str.remove(str.size()-1,1);
        if ((str[0] == QChar('\"')) || (str[0] == QChar('\''))) str.remove(0,1);
    }
}
