#include "layoutdata.h"
#include "layoutobject.h"
#include "state.h"
#include "mybutton.h"
#include <QFileDialog>

LayoutData::LayoutData(QString filename)
{
    is_parsing = false;
    this->filename = filename;
    current_layout = -1;
}


/*Display all content of arena*/
void LayoutData::print_layout(){
    qDebug() << "layout has Vars: " << var_name.size();
    //qDebug() << "Vectors: " << var_vec.size();
    //qDebug() << "Numbers: " << var_number.size();
    //qDebug() << "Meshes: " << var_mesh_arr.size();
    //qDebug() << "LCS: " << var_lcs.size();
    //qDebug() << "Quats: " << var_quat.size();

}

int LayoutData::is_var_exist(QString var){
    int i=-1;
    for (uint j = 0; j<var_name.size(); j++){
        int x = QString::compare(var_name[j], var, Qt::CaseSensitive);
        if ( x == 0 ) {
            i = j;
            return i;
        }
    }
    return i;
}

/*
    This function look through the whole data list, looking for the active layout elements,
    like buttons, labels, knoba, glviews, layouts and so on, and draws them.

*/
void LayoutData::draw_layout(QPainter &painter, QPoint& loffset){
    if (current_layout < 0 && var_slayout.size() > 0){
        current_layout = 0;
        var_slayout[0]->selected = true;
        qDebug() << "setting default layout to 0";
    }

    //qDebug() << "num simple layouts: " << var_slayout.size();

    SimpleLayout * sl = NULL;
    SimpleLayout * hl = NULL; // hidden layout
    for(uint i=0; i<var_slayout.size(); i++){
        if (var_slayout[i]->selected == true) {
            sl = var_slayout[i];
            //break;
        }else{
            //hide deselected elements
            hl = var_slayout[i];
            if (hl != NULL){
                //qDebug() << "other layouts = " << hl->elements.size();
                for (int i=0; i < hl->elements.size(); i++){
                    // extract init state of element
                    QString el_name = hl->elements[i];
                    QStringList complex_element;
                    complex_element = el_name.split(":"); // try to select the init state
                    int t;
                    MyLayoutObject * lo = get_layout_object_by_name(complex_element[0], &t);
                    int init_state = -1;
                    if (complex_element.size()>1) {
                        init_state = complex_element[1].toInt();
                    }
                    if (t == BUTTON){
                        MyButton * mb = (MyButton*) lo;
                        mb->hide();
                        //mb->drawLayoutObject(painter, loffset);
                    }
                    if (t == GCODEVIEW){
                        GCodeView * cv = (GCodeView*) lo;
                        cv->hide();
                        cv->drawLayoutObject(painter, loffset);
                    }
                }
            }
        }
    }


    if (sl == NULL) {
        qDebug() << "no active layout to draw. Add layouts in the .ini file";
        return;
    }
    //qDebug()<< "layout elments to display: " << sl->elements ;
    for (int i=0; i < sl->elements.size(); i++){
        // extract init state of element
        QString el_name = sl->elements[i];
        QStringList complex_element;
        complex_element = el_name.split(":"); // try to select the init state
        int t;
        MyLayoutObject * lo = get_layout_object_by_name(complex_element[0], &t);
        int init_state = -1;
        if (complex_element.size()>1) {
            init_state = complex_element[1].toInt();
        }
        if (t == BUTTON){
            MyButton * mb = (MyButton*) lo;
            mb->show();
            mb->drawLayoutObject(painter, loffset);
        }
        if (t == GCODEVIEW){
            GCodeView * cv = (GCodeView*) lo;
            cv->show();
            cv->drawLayoutObject(painter, loffset);
        }
    }

}
// look up all all layout elements and finds which one is at the position
MyLayoutObject * LayoutData::get_layout_object_at_mouse_pos(QPoint pos, int * type){
    MyLayoutObject * mlo = NULL;
    if (current_layout < 0 && var_slayout.size() > 0){
        current_layout = 0;
        var_slayout[0]->selected = true;
        qDebug() << "setting default layout to 0";
    }
    SimpleLayout * sl = NULL;
    for(uint i=0; i<var_slayout.size(); i++){
        if (var_slayout[i]->selected == true) {
            sl = var_slayout[i];
            break;
        }
    }
    if (sl == NULL) {
        qDebug() << "no active layout. Add layouts in the .ini file";
        return NULL;
    }
    //qDebug()<< "layout elments to display: " << sl->elements ;
    for (int i=0; i < sl->elements.size(); i++){
        QString el_name = sl->elements[i];
        QStringList complex_element;
        complex_element = el_name.split(":"); // try to select the init state
        int t;
        MyLayoutObject * lo = get_layout_object_by_name(complex_element[0], &t);
        if (t == BUTTON){
            //qDebug() << "button found " << sl->elements[i];
            MyButton * mb = (MyButton*) lo;
            if (mb->isCursorOver(pos)) {
                mlo = lo;
                *type = t;
                break;
            }
        }
        if (t == GCODEVIEW){
            //qDebug() << "gcode view found " << sl->elements[i];
            GCodeView * cv = (GCodeView*) lo;
            if (cv->isCursorOver(pos)) {
                mlo = lo;
                *type = t;
                break;
            }
        }
        // .. add other objects
    }

    return mlo;
}

bool LayoutData::is_the_same_file(QString fn){
    if (QString::compare(fn, filename, Qt::CaseSensitive) == 0) return TRUE;
    return FALSE;
}

/*
    Function lookup the all tree and get the type of the object. If it's a layout object it return it's pointer.
*/
MyLayoutObject* LayoutData::get_layout_object_by_name(QString& obj_name, int * type){
    MyLayoutObject * l_obj = NULL;
    int idx = is_var_exist(obj_name);
    if (-1 == idx) return NULL; // name is not found
    //we have the name of layout element now, verufy the type of it
    int t = var_type[idx];
    *type = t;
    switch (t) {
    case LAYOUT:
        l_obj = var_slayout[val_index[idx]];
        //qDebug() << "have simple layout: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << l_obj;
        break;
    case BUTTON:
        l_obj = var_mybutton[val_index[idx]];
        //qDebug() << "have button: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << l_obj;
        break;
    case LABEL:
        l_obj = var_label[val_index[idx]];
        //qDebug() << "have lavel: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case LIND:
         l_obj = var_indicator[val_index[idx]];
        //qDebug() << "have indicator: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case KNOB:
         l_obj = var_knob[val_index[idx]];
        //qDebug() << "have knob: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GLVIEW:
        l_obj = var_glview[val_index[idx]]; // contains data for opengl element
        //qDebug() << "have glview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GCODEVIEW:
        l_obj = var_gcodeview[val_index[idx]];
        //qDebug() << "have codeview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GCODEEDIT:
        l_obj = var_gcodeedit[val_index[idx]];
        //qDebug() << "have codeview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;

    default:
        //qDebug() << "object idx =" << val_index[idx] << "is not a type of object for layout element";
        break;
    }

    return l_obj;
}

MyState * LayoutData::get_state_object_by_name(QString& obj_name){
    MyState * s_obj = NULL;
    int idx = is_var_exist(obj_name);
    if (-1 == idx) return NULL; // name is not found
    //we have the name of state element now, verufy the type of it
    int t = var_type[idx];
    if (t == STATE){
        s_obj = var_state [val_index[idx]];
        //qDebug() << "have state: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << s_obj;
    }
     return s_obj; // if it's not a state object, return NULL;
}

QPoint * LayoutData::get_pos_var_by_name(QString & pos_name){
    QPoint * pt = NULL;
    int idx = is_var_exist(pos_name);
    if (-1 == idx) return NULL; // name is not found
    int t = var_type[idx];
    if (t == IVEC2){
        pt = var_ivec2 [val_index[idx]];
        //qDebug() << "have point: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << pt;
    }
    return pt; // it's a reference to the point;
}

int LayoutData::get_int_value_by_name(QString& int_name){
    //qDebug() << "looking for var: " << int_name;
    int val_idx = is_var_exist(int_name);
    //qDebug() << " var idx: " << val_idx;
    if ( val_idx >= 0 ){ // have variable
        if (this->var_type[val_idx] == INTN){
            //qDebug() << " var type: " << var_type[val_idx];
            return var_int_number[this->val_index[val_idx]];
        }
    }else{
        qDebug() << "error, variable " << int_name << " does not exist, fix the config file";
    }
    return 0;
}


QString  LayoutData::get_string_value_by_name(QString& str_name){
    //qDebug() << "looking for var: " << str_name;
    int val_idx = is_var_exist(str_name);
    //qDebug() << " var idx: " << val_idx;
    if ( val_idx >= 0 ){ // have variable
        if (this->var_type[val_idx] == STRI){
            //qDebug() << " var type: " << var_type[val_idx];
            return var_string[this->val_index[val_idx]];
        }
    }else{
        qDebug() << "error, variable " << str_name << " does not exist, fix the config file";
    }
    return QString();
}

Path2D * LayoutData::get_path_value_by_name(QString &str_name){
    //qDebug() << "looking for var: " << str_name;
    int val_idx = is_var_exist( str_name);
    //qDebug() << " var idx: " << val_idx;
    if ( val_idx >= 0 ){ // have variable
        if (this->var_type[val_idx] == PATH){
            //qDebug() << " var type: " << var_type[val_idx];
            return var_path[this->val_index[val_idx]];
        }
    }else{
        qDebug() << "error, variable " << str_name << " does not exist, fix the config file";
    }
    return NULL;

}
/*
*   This function receive the command from layout element and can modify any state of the window.
*   It can send the messages to linuxcnc system (later).
*/
void LayoutData::processCommand(QString & cmd){
    /*
        Command consisnt of name of variables in the layout data and can modify it.
        For button the number means the state.
        "button:1" means that the button state will be switched to state 1.
        Command also can have different elements like
        lcnc:
        openfile:
        which has own syntax or specific commands like
        lcnc:"set estop off" etc.


    */
    // extracting special commands
    int n = cmd.indexOf("lcnc:",0);
    if (n >= 0){
        //extracting substing form command
        int quote_start = cmd.indexOf("\"", n);
        int quote_end = cmd.indexOf("\"", quote_start+1);

        QString lcnc_cmd = cmd.mid(quote_start, quote_end - quote_start+1);
        qDebug() << "lcnc command: [" << lcnc_cmd << "]";
        cmd.remove("lcnc:");
        cmd.remove(lcnc_cmd);
        lcnc_cmd.remove(0,1);
        lcnc_cmd.remove(lcnc_cmd.size()-1,1);
        //save command to lcnc variable
        set_string_value_by_name(QString("lcnc"), lcnc_cmd);
    }

    QStringList cmd_list = cmd.split(" ");

    for (int i=0; i < cmd_list.size(); i++){
        // select the name of object
        QString tmp = cmd_list[i];
        if (!tmp.isEmpty() && tmp.contains(":")){
            QStringList obj = tmp.split(":");

            QString obj_name = obj[0];
            qDebug() << "object name " << obj_name;
            QString obj_param = obj[1];
            qDebug() << "obj_param " << obj_param;
            int t;
            MyLayoutObject * lo = get_layout_object_by_name(obj_name, &t);
            if (t == BUTTON){
                MyButton * mb = (MyButton*) lo;
                int new_state = obj_param.toInt();
                if ( !(mb->setState(new_state)) ){
                    qDebug() << "cannot change batton state" ;
                }
            }
            if (t == GCODEVIEW){
                GCodeView * cv = (GCodeView*) lo;
                int new_state = obj_param.toInt();
                if ( !(cv->setState(new_state)) ){
                    qDebug() << "cannot change view state" ;
                }
            }
            if (t == LAYOUT){
                SimpleLayout * sl = (SimpleLayout*) lo;
                int new_state = obj_param.toInt();
                if (sl!=NULL){
                    if ( !(sl->select_layout((new_state)?TRUE:FALSE) ) ){
                        qDebug() << "cannot change layout state" ;
                    }
                }
            }
            if (lo == NULL ){
                //not a layout element, it could be a variable
                int idx = is_var_exist(obj_name);
                if ( idx >=0 ) {
                    qDebug() << "variable found: " << obj_name << " with param " << obj_param;
                    //looking for subcommand
                    if(obj_param.contains("fileopen")){
                        //open file dialog and if not empty assign to the string variable
                        QString fileName = QFileDialog::getOpenFileName(NULL, QString("Open G-code"), QString(), QString("G-code (*.ngc);;All Files (*)"));

                        if (!fileName.isEmpty()) {
                            set_string_value_by_name(obj_name, fileName);
                        }
                    }
                }
            }

        }

    }

}
/*
    Looking for UPDCMD element for active statuses of layout elements and execute them.
*/
void LayoutData::send_update_layout_elements(QTcpSocket * socket){
    // look through all buttons because there are a lot of elements in general.
    // indicators are also buttons
    for(unsigned int i=0; i< var_mybutton.size(); i++){
        //looking for active state of the button
        //qDebug()<< "i= " << i << " size= " <<var_mybutton.size();
        MyButton * mb = var_mybutton[i];
        if (mb == NULL) continue;
        QString upd_cmd = mb->get_update_command();
        if (!upd_cmd.isEmpty()){
            //send it to socket
            socket->write(upd_cmd.toUtf8().constData());
            socket->write("\r\n");
            socket->flush();
        }
    }
}

void LayoutData::parseReply(QString rLine){
    // update indicators, feedrate, button states, spindle encoder, file line, opened file
    // joints
    // units
    // file name
    // state of machine, on/off, auto, manual, mdi
    for(unsigned int i=0; i< var_mybutton.size(); i++){
        //looking for active state of the button
        //qDebug()<< "i= " << i << " size= " <<var_mybutton.size();
        MyButton * mb = var_mybutton[i];
        if (mb == NULL) continue;
        QString p_mask = mb->get_peek_mask();
        if (!p_mask.isEmpty()){
            // prepare p_mask. We are looking to extract parameter in front of $ sign.
            //peek up the value from reply

            QStringList p_list = p_mask.split(" ");
            QStringList r_list = rLine.split(" ");
            QString val;
            qDebug()<< "comparing " << p_list << " and "<< r_list;
            if (p_list.size() == r_list.size()){
                // compare 2 lists to extract value in front of $
                // the beginning of the list should match otherwise it's not our case.
                int fit_cnt = 0;
                for (int j=0; j<p_list.size(); j++){
                    if((QString::compare(p_list[j], r_list[j]) == 0) || (p_list[j] == QString("_")) ) fit_cnt++;
                    if((fit_cnt == j )&& (p_list[j] == QString("$"))){
                        //got the value
                        qDebug()<< "value = " << r_list[j]; // dont need to continue
                        val = r_list[j];
                        break;
                    }
                }
                // process value, it's suppose to be a float number
                bool ok = FALSE;
                float f_val = val.toFloat(&ok);
                if(ok){
                    // modify existing value
                    QString old_val = mb->get_value();
                    qDebug() << "value to modify" << old_val << " new  val " << f_val;
                    QString sign = "+";
                    if (f_val < 0) sign="";
                    mb->set_value(sign + QString::number(f_val,'f',4));
                }

            }

        }
    }


}

void LayoutData::set_string_value_by_name(QString str_name, QString& value){
    int v_idx = is_var_exist(str_name);
    if (-1 != v_idx && (var_type[v_idx] == STRI)){
        var_string[val_index[v_idx]] = value;
        qDebug()<< str_name << " = " << value;

    }else{
       qDebug()<< "variable name do not exist:" << str_name;
       exit (-1);
    }
    return;
}
