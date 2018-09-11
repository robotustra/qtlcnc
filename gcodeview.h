#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include "layoutobject.h"
#include <QListWidget>
#include <QMainWindow>


class LayoutData;
class MyState;

class GCodeView: public MyLayoutObject
{
public:
    GCodeView(QMainWindow * parent, QStringList & sl, LayoutData *ld, int init_state);
    ~GCodeView();
    void drawLayoutObject(QPainter& painter, QPoint &loffset);
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
    QListWidget * g_list;
    QMainWindow * parent;
    QString file_name;
    int line_pos; // the line of g-code to be executed.


};

#endif // GCODEVIEW_H
