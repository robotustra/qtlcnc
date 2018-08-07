#include "layoutdata.h"

LayoutData::LayoutData(QString filename)
{
    this->filename = filename;
}


/*Display all content of arena*/
void LayoutData::print_layout(){
    qDebug() << "layout has Vars: " << var_name.size();
    //qDebug() << "Vectors: " << var_vec.size();
    //qDebug() << "Numbers: " << var_number.size();
    //qDebug() << "Meshes: " << var_mesh_arr.size();
    //qDebug() << "LCS: " << var_lcs.size();
    //qDebug() << "Quats: " << var_quat.size();

}

int LayoutData::is_var_exist(LayoutData* ld, QString var){
    int i=-1;
    for (uint j = 0; j<ld->var_name.size(); j++){
        int x = QString::compare(ld->var_name[j], var, Qt::CaseSensitive);
        if ( x == 0 ) {
            i = j;
            return i;
        }
    }
    return i;
}

/*
    This function look through the whole data list, looking for the active layout elements,
    like buttons, labels, knoba, glviews, layouts and so on, and draws them.

*/
void LayoutData::draw_layout(QPainter &painter){
    SimpleLayout * sl = NULL;
    for(int i=0; i<var_slayout.size(); i++){
        if (var_slayout[i]->selected == true) {
            sl = var_slayout[i];
            break;
        }
    }

    if (sl == NULL) {
        qDebug() << "no active layout to draw. Modify ini file";
        return;
    }

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
            /*
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

bool LayoutData::is_the_same_file(QString fn){
    if (QString::compare(fn, filename, Qt::CaseSensitive) == 0) return TRUE;
    return FALSE;
}
