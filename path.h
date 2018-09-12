#ifndef PATH2D_H
#define PATH2D_H

#include <QStringList>
#include <QPainter>

class LayoutData;

/*
    A path to draw the button picture
*/
class Path2D
{
public:

    Path2D(QStringList & sl, LayoutData * ld);
    void drawPath(QPainter& painter, QString& pencolor, QString& bgcolor, QPoint *pos, QPoint *sz, int ucell, QPoint &loffset);
    void removeQuotes( QString & str);


private:
    //path content
    QStringList p;
    LayoutData * ld_local;
};

#endif // PATH_H
