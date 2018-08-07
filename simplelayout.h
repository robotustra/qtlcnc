#ifndef SIMPLELAYOUT_H
#define SIMPLELAYOUT_H
#include <QString>
#include <QStringList>

class SimpleLayout
{
public:
    SimpleLayout(QStringList & sl);
    bool select_layout(bool new_state);


    bool selected;

public: // replace with private after debugging is finished
    QStringList elements;
};

#endif // SIMPLELAYOUT_H
