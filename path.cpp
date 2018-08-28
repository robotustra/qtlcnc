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

}
