#include "state.h"
#include <QDebug>

MyState::MyState(QStringList &sl, LayoutData *ld)
{
    ss = sl;
    ld_local = ld; //save for paint time.
    qDebug()<< "state object::" << ss;
    // Complex objects are interpreted during execution time.
    // The variable name is defined in front of it's type specifier.
    // special functions can extract parameters and use it for drawing or changing.
    // use layout data to parse and load data of the states

    update_state_fields(ld);

}

bool MyState::update_state_fields(LayoutData * ld){
   return true;
}

QPoint * MyState::get_Position(LayoutData * ld){
    QPoint * point = NULL;

    pos_var_name.clear(); // reparse every time.
    if ( pos_var_name.isEmpty()){
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare("POS", ss[i], Qt::CaseSensitive )){

                pos_var_name = ss[i-1];
                //qDebug()<< " position string " << pos_var_name;
                break;
            }
        }
    }
    // now we shoud have the position variable name.
    if (!pos_var_name.isEmpty()){
        // get variable data
        point = ld->get_pos_var_by_name(pos_var_name);
    }
    return point;
}

QPoint* MyState::get_Size(LayoutData * ld){
    QPoint * point = NULL;
    size_var_name.clear(); //reparse every time for now.
    if ( size_var_name.isEmpty()){
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare("SIZE", ss[i], Qt::CaseSensitive )){
                pos_var_name = ss[i-1];
                //qDebug()<< " size string " << pos_var_name;
                break;
            }
        }
    }
    // now we shoud have the position variable name.
    if (!pos_var_name.isEmpty()){
        // get variable data
        point = ld->get_pos_var_by_name(pos_var_name);
    }
    return point;

}

//ctype = "BGCOL" or "PCOL"
QString MyState::get_bgColor(LayoutData * ld, int state_idx){
    if (bgcolor_var_names.size() == 0) //is a string list!!!!
    {
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "BGCOL" , ss[i], Qt::CaseSensitive )){
                bgcolor_var_names.push_back( ss[i-1] );
            }
        }
    }
    // now we shoud have the position variable name.
    if (bgcolor_var_names.size()>0){
        // get variable data
        if (state_idx >=0 && (state_idx < bgcolor_var_names.size()))
            return ld->get_string_value_by_name(bgcolor_var_names[state_idx]);
        else
        {
            //qDebug() << "index of states is more than the number of bg colors";
            return ld->get_string_value_by_name(bgcolor_var_names[0]); // just return the first coler
        }

    }
    return QString();
}

QString MyState::get_pColor(LayoutData * ld, int state_idx){
    if (pcolor_var_name.size() == 0) //is a string list!!!!
    {
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "PCOL" , ss[i], Qt::CaseSensitive )){
                pcolor_var_name.push_back( ss[i-1] );
            }
        }
    }
    // now we shoud have the position variable name.
    if (pcolor_var_name.size()>0){
        // get variable data
        if (state_idx >=0 && (state_idx < pcolor_var_name.size()))
            return ld->get_string_value_by_name(pcolor_var_name[state_idx]);
        else
        {
            //qDebug() << "index of states is more than the number of bg colors";
            return ld->get_string_value_by_name(pcolor_var_name[0]); // just return the first coler
        }
    }
    return QString();
}

// Could be a path to draw or a picture. For now Path only.
Path2D * MyState::get_Pix(LayoutData * ld, int path_num){
    if (path_var_names.size() == 0) //string list is empty
    {
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "PIX" , ss[i], Qt::CaseSensitive )){
                path_var_names.push_back( ss[i-1] );
            }
        }
    }
    // now we shoud have the position variable name.
    if (path_var_names.size()>0){
        // get variable data
        if (path_num >=0 && (path_num < path_var_names.size()))
            return ld->get_path_value_by_name(path_var_names[path_num]);
        else
        {
            //qDebug() << "index of states is more than the number of pathes";
            return ld->get_path_value_by_name(path_var_names[0]); // just return the first path
        }
    }
    return NULL;
}

int MyState::get_paths_number(){
    int numb = 0;
    for (int i=1; i < ss.size(); i++) {
        if (0 == QString::compare( "PIX" , ss[i], Qt::CaseSensitive )){
            numb++;
        }
    }
    return numb;
}

QString MyState::get_command(LayoutData * ld){
    if (cmd_var_name.isEmpty())
    {
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "CMDL" , ss[i], Qt::CaseSensitive )){
                cmd_var_name = ss[i-1];
                return ld->get_string_value_by_name(cmd_var_name);
            }
        }
    }
    // now we shoud have variable name.
    if (!cmd_var_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(cmd_var_name);
    }
    return QString();
}

//command which is send to lcnc to get value
QString MyState::get_update_command(LayoutData * ld){
    if (upd_cmd_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "UPDCMD" , ss[i], Qt::CaseSensitive )){
                upd_cmd_name = ss[i-1];
                break;
            }
        }
    }
    // now we shoud have update variable name.
    if (!upd_cmd_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(upd_cmd_name);
    }
    return QString();
}

// group update command
QString MyState::get_update_group_command(LayoutData * ld){
    if (upd_group_cmd_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "UPDGRCMD" , ss[i], Qt::CaseSensitive )){
                upd_group_cmd_name = ss[i-1];
                return ld->get_string_value_by_name(upd_group_cmd_name);
            }
        }
    }
    if (!upd_group_cmd_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(upd_group_cmd_name);
    }
    return QString();
}

// if the group command is not empty peek command if used to extract value.
QString MyState::get_peek_mask(LayoutData * ld){
    if (peek_cmd_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "PMASK" , ss[i], Qt::CaseSensitive )){
                peek_cmd_name = ss[i-1];
                return ld->get_string_value_by_name(ss[i-1]);
            }
        }
    }
    if (!peek_cmd_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(peek_cmd_name);
    }
    return QString();
}

QString MyState::get_change_state_script(LayoutData * ld){
    if (change_state_sctipt.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "STCMD" , ss[i], Qt::CaseSensitive )){
                change_state_sctipt = ss[i-1];
                return ld->get_string_value_by_name(ss[i-1]);
            }
        }
    }
    if (!change_state_sctipt.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(change_state_sctipt);
    }
    return QString();
}

QString MyState::get_file_name(LayoutData * ld){
    if (file_name.isEmpty()) {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "FILE" , ss[i], Qt::CaseSensitive )){
                file_name = ss[i-1];
                return ld->get_string_value_by_name(file_name);
            }
        }
    }
    if (!file_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(file_name);
    }
    return QString();
}

// look for value variable to be displayed, usually it's a path variable
QString MyState::get_value(LayoutData * ld){
    //!! UPDATE value all the time
    if (value_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "VALUE" , ss[i], Qt::CaseSensitive )){
                value_name = ss[i-1];
                //qDebug()<< "value name= " << value_name;
                return ld->get_string_value_by_name(value_name);
            }
        }
    }
    if (!value_name.isEmpty()){
        // get variable data
        //qDebug()<< "value name= " << value_name;
        return ld->get_string_value_by_name(value_name);
    }
    return QString();
}

void MyState::set_value(LayoutData * ld, QString value){
    if (value_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "VALUE" , ss[i], Qt::CaseSensitive )){
                value_name = ss[i-1];
                ld->set_string_value_by_name(value_name, value);
                return;
            }
        }
    }
    if (!value_name.isEmpty()){
        // get variable data
        ld->set_string_value_by_name(value_name, value);
    }
}


// mode of update
QString MyState::get_update_mode(LayoutData * ld){
    if (update_mode_name.isEmpty())
    {
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare( "UPDMODE" , ss[i], Qt::CaseSensitive )){
                update_mode_name = ss[i-1];
            }
        }
    }
    return ld->get_string_value_by_name(update_mode_name);
}
