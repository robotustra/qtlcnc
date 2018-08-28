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
    if ( pos_var_name.isEmpty()){
        //trying to parse and
        qDebug() << ".........error in here....state string" << ss ;
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare("POS", ss[i], Qt::CaseSensitive )){

                pos_var_name = ss[i-1];
                qDebug()<< " position string " << pos_var_name;
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
    if ( size_var_name.isEmpty()){
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare("SIZE", ss[i], Qt::CaseSensitive )){
                pos_var_name = ss[i-1];
                 qDebug()<< " size string " << pos_var_name;
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
            qDebug() << "index of states is more than the number of bg colors";
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
            qDebug() << "index of states is more than the number of bg colors";
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
            qDebug() << "index of states is more than the number of pathes";
            return ld->get_path_value_by_name(path_var_names[0]); // just return the first path
        }
    }
    return NULL;
}
