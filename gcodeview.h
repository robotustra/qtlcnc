#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include "layoutobject.h"


class LayoutData;
class MyState;

class GCodeView: public MyLayoutObject
{
public:
    GCodeView(QStringList & sl, LayoutData *ld, int init_state);
    void drawLayoutObject(QPainter& p, QPoint &loffset);
    void selectLayoutObject();
    void setLayoutObjectStatus();
    void getLayoutObjectStatus();
    void setLayoutObjectPropertiy();
    void getLayoutObjectPropertiy();
    bool isCursorOver(QPoint &pos);
    bool setState(int state);
    MyState* getState(LayoutData* ld, QString state_name);
    QString get_active_state_command();

private:
    QString  g_code_file;
    QStringList elements;   // this list contains the states button can have.
    int current_state;      // if the more than one state possible this index keep it.
    LayoutData * ld_local;  // save the pointer to the data while initializing;


};

#endif // GCODEVIEW_H
