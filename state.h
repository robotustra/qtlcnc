#ifndef STATE_H
#define STATE_H
#include <QStringList>
#include <QString>
#include "layoutdata.h"
#include <QPoint>


class MyState
{
public:
    MyState(QStringList & sl, LayoutData * ld);
    bool update_state_fields(LayoutData * ld); // parsing input string and try to set fields.

    QPoint* get_Position(LayoutData * ld);    // returns the position in the
    QPoint* get_Size(LayoutData * ld);    // returns the position in the
    QString get_bgColor(LayoutData * ld, int path_num);
    QString get_pColor(LayoutData * ld, int pathnum);


private:
    QStringList ss;
    //"sb_pos", "POS", "size_2x2", "SIZE", "bg_color", "BGCOL", "p_color", "PCOL", "sb_pic1", "PIX", "stop_cmd", "CMDL", "stop_status", "STATL" txt TXT
    QString pos_var_name;
    QString size_var_name;
    QStringList bgcolor_var_names; // a background which is used for the corresponding path
    QStringList pcolor_var_name; // a pen list is corresponding to path
    QStringList path_var_names; // all pathes are drawn
    QString cmd_var_name; // a command line which is send to interpreter when object (button) state is activated
    QString txt_var_name; // a scriptire which is displayer on the button
    QString status_var_name; // a status of the object which is activated
    LayoutData * ld_local;

};

#endif // STATE_H
