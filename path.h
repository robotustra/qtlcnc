#ifndef PATH2D_H
#define PATH2D_H

#include <QStringList>

/*
    A path to draw the button picture
*/
class Path2D
{
public:

    Path2D(QStringList & sl);

private:
    //path content
    QStringList p;
};

#endif // PATH_H
