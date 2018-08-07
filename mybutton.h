#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QStringList>
#include "layoutobject.h"

class MyButton: public LayoutObject
{
public:
    MyButton(QStringList & sl);


private:

    QStringList elements;

};

#endif // MYBUTTON_H
