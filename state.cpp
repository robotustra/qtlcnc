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
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare("POS", ss[i], Qt::CaseSensitive )){
                pos_var_name = ss[i-1];
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
QString MyState::get_Color(LayoutData * ld, const char* ctype){
    //
    bgcolor_var_names is a string list!!!!

    if (  .isEmpty()){
        //trying to parse and
        for (int i=1; i < ss.size(); i++) {
            if (0 == QString::compare(QString(ctype), ss[i], Qt::CaseSensitive )){
                pos_var_name = ss[i-1];
                break;
            }
        }
    }
    // now we shoud have the position variable name.
    if (!pos_var_name.isEmpty()){
        // get variable data
        return ld->get_string_value_by_name(pos_var_name);

    }
    return QString();

}
