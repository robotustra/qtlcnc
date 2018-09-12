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
    Path2D* get_Pix(LayoutData * ld, int path_num);
    QString get_command(LayoutData * ld); // return the command string associated with this state when activated
    QString get_update_command(LayoutData * ld); //command which is send to lcnc to get value;
    QString get_update_group_command(LayoutData * ld); // group update command
    QString get_peek_mask(LayoutData * ld); // if the group command is not empty peek mask if used to extract value.
    QString get_value(LayoutData * ld); // look for value variable to be displayed, usually it's a path variable
    void    set_value(LayoutData * ld, QString value);
    QString get_update_mode(LayoutData * ld); // mode of update
    QString get_file_name(LayoutData * ld);
    QString get_change_state_script(LayoutData * ld); // executed when the new value is got from outside


    int get_paths_number();


private:
    QStringList ss;
    //"sb_pos", "POS", "size_2x2", "SIZE", "bg_color", "BGCOL", "p_color", "PCOL", "sb_pic1", "PIX", "stop_cmd", "CMDL", "stop_status", "STATL" txt TXT
    // UCMD - update command, the command which is executed to get data for update
    // UGCMD - update group command
    // PMASK - the mask to select value from result.
    // VALUE - the string which is updated accordingly to the polled data
    // UPDMODE - the mode of the value
    // FILE - file name for view element as an input
    // STCMD - state change script


    QString pos_var_name;
    QString size_var_name;
    QStringList bgcolor_var_names; // a background which is used for the corresponding path
    QStringList pcolor_var_name; // a pen list is corresponding to path
    QStringList path_var_names; // all pathes are drawn
    QString cmd_var_name; // a command line which is send to interpreter when object (button) state is activated
    QString txt_var_name; // a scriptire which is displayer on the button
    QString status_var_name; // a status of the object which is activated
    QString upd_cmd_name; // update command name
    QString upd_group_cmd_name; // update group command name - the command is used to get parameters
    QString peek_cmd_name; // command is to used to peek up value from group command reply
    QString value_name;     // value to be displayed on indicator
    QString update_mode_name; // modes: continuous, group, ocasional
    QString file_name;
    QString change_state_sctipt;

    LayoutData * ld_local;

};

#endif // STATE_H
