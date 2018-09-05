#ifndef SIMPLELAYOUT_H
#define SIMPLELAYOUT_H
#include <QString>
#include <QStringList>
#include "layoutobject.h"

class SimpleLayout : public MyLayoutObject
{
public:
    SimpleLayout(QStringList & sl);
    bool select_layout(bool new_state);


    bool selected;

public: // replace with private after debugging is finished
    QStringList elements;
};

#endif // SIMPLELAYOUT_H
