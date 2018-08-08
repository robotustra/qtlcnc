#include "layoutdata.h"
#include "layoutobject.h"
#include "state.h"
#include "mybutton.h"

LayoutData::LayoutData(QString filename)
{
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

int LayoutData::is_var_exist(LayoutData* ld, QString var){
    int i=-1;
    for (uint j = 0; j<ld->var_name.size(); j++){
        int x = QString::compare(ld->var_name[j], var, Qt::CaseSensitive);
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
void LayoutData::draw_layout(QPainter &painter){
    if (current_layout < 0 && var_slayout.size() > 0){
        current_layout = 0;
        var_slayout[0]->selected = true;
        qDebug() << "setting default layout to 0";
    }

    qDebug() << "num simple layouts: " << var_slayout.size();

    SimpleLayout * sl = NULL;
    for(uint i=0; i<var_slayout.size(); i++){
        if (var_slayout[i]->selected == true) {
            sl = var_slayout[i];
            break;
        }
    }

    if (sl == NULL) {
        qDebug() << "no active layout to draw. Add layouts in the .ini file";
        return;
    }

    qDebug()<< "layout elments to display: " << sl->elements ;
    for (int i=0; i < sl->elements.size(); i++){
        int t;
        MyLayoutObject * lo = get_layout_object_by_name(sl->elements[i], &t);
        if (t == BUTTON){
            MyButton * mb = (MyButton*) lo;
            mb->drawLayoutObject(painter);

        }
    }



    /*
    static const QPoint points[4] = {
        QPoint(10, 10),
        QPoint(90, 10),
        QPoint(90, 90),
        QPoint(10, 90)
    };
    */
    // example
    //painter.drawRect(0, 0, 100,100);
    //QRect rect(10, 20, 80, 60);


    //QRect rect(0, 0, ucell-border, ucell-border);


    /*
    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    int startAngle = 20 * 16;
    int arcLength = 120 * 16;

    QLinearGradient lg(0,0,100,100);
    */
    //brush = QBrush("gray", Qt::LinearGradientPattern);
    /*brush = QBrush(Qt::NoBrush);
    painter.setBrush(brush);
    */
    /*
    shape = Rect;
    painter.setRenderHint(QPainter::Antialiasing, true);
    */
    /*for (int x = 0; x < ggeom.width() * ucell; x += ucell) {
        for (int y = 0; y < ggeom.height() * ucell; y += ucell) {
            painter.save();
            painter.translate(loffset.x() + x, loffset.y() + y);
            / *
            if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }* /

            switch (shape) {
            case Line:
                painter.drawLine(rect.bottomLeft(), rect.topRight());
                break;
            case Points:
                //painter.drawPoints(points, 4);
                break;
            case Polyline:
                painter.drawPolyline(points, 4);
                break;
            case Polygon:
                painter.drawPolygon(points, 4);
                break;
            case Rect:
                painter.drawRect(rect);
                break;
            case RoundedRect:
                painter.drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
                break;
            case Ellipse:
                painter.drawEllipse(rect);
                break;
            case Arc:
                painter.drawArc(rect, startAngle, arcLength);
                break;
            case Chord:
                painter.drawChord(rect, startAngle, arcLength);
                break;
            case Pie:
                painter.drawPie(rect, startAngle, arcLength);
                break;
            case Path:
                painter.drawPath(path);
                break;
            case Text:
                painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nNokia"));
                break;
            case Pixmap:
                painter.drawPixmap(10, 10, pixmap);
            }
            painter.restore();
        }
    }*/


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
    int idx = is_var_exist(this, obj_name);
    if (-1 == idx) return NULL; // name is not found
    //we have the name of layout element now, verufy the type of it
    int t = var_type[idx];
    *type = t;
    switch (t) {
    case BUTTON:
        l_obj = var_mybutton[val_index[idx]];
        qDebug() << "have button: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << l_obj;
        break;
    case LABEL:
        l_obj = var_label[val_index[idx]];
        qDebug() << "have lavel: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case LIND:
         l_obj = var_indicator[val_index[idx]];
        qDebug() << "have indicator: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case KNOB:
         l_obj = var_knob[val_index[idx]];
        qDebug() << "have knob: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GLVIEW:
        l_obj = var_glview[val_index[idx]]; // contains data for opengl element
        qDebug() << "have glview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GCODEVIEW:
        l_obj = var_gcodeview[val_index[idx]];
        qDebug() << "have codeview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;
    case GCODEEDIT:
        l_obj = var_gcodeedit[val_index[idx]];
        qDebug() << "have codeview: idx =" << val_index[idx] << "name = " << var_name[idx];
        break;

    default:
        qDebug() << "object idx =" << val_index[idx] << "is not a type of object for layout element";
        break;
    }

    return l_obj;
}

MyState * LayoutData::get_state_object_by_name(QString& obj_name){
    MyState * s_obj = NULL;
    int idx = is_var_exist(this, obj_name);
    if (-1 == idx) return NULL; // name is not found
    //we have the name of state element now, verufy the type of it
    int t = var_type[idx];
    if (t == STATE){
        s_obj = var_state [val_index[idx]];
        qDebug() << "have state: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << s_obj;
    }
     return s_obj; // if it's not a state object, return NULL;
}

QPoint * LayoutData::get_pos_var_by_name(QString & pos_name){
    QPoint * pt = NULL;
    int idx = is_var_exist(this, pos_name);
    if (-1 == idx) return NULL; // name is not found
    int t = var_type[idx];
    if (t == IVEC2){
        pt = var_ivec2 [val_index[idx]];
        qDebug() << "have point: idx =" << val_index[idx] << "name = " << var_name[idx] << "addres =" << pt;
    }
    return pt; // it's a reference to the point;
}

int LayoutData::get_int_value_by_name(QString& int_name){
    qDebug() << "looking for var: " << int_name;
    int val_idx = is_var_exist(this, int_name);
    qDebug() << " var idx: " << val_idx;
    if ( val_idx >= 0 ){ // have variable
        if (this->var_type[val_idx] == INTN){
            qDebug() << " var type: " << var_type[val_idx];
            return var_int_number[this->val_index[val_idx]];
        }
    }else{
        qDebug() << "error, variable " << int_name << " does not exist, fix the config file";
    }
    return 0;
}

