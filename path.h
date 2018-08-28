#ifndef PATH2D_H
#define PATH2D_H

#include <QStringList>
#include <QPainter>

/*
    A path to draw the button picture
*/
class Path2D
{
public:

    Path2D(QStringList & sl);
    void drawPath(QPainter& painter, QString& pencolor, QString& bgcolor, QPoint *pos, QPoint *sz, int ucell);



private:
    //path content
    QStringList p;
};

#endif // PATH_H
