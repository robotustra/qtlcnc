#include "gcodeview.h"
#include "state.h"
#include <QPoint>
#include <QDebug>

GCodeView::GCodeView(QMainWindow* p, QStringList &sl, LayoutData *ld, int init_state)
{
    parent = p;
    g_list = new QListWidget(p);
    elements = sl;
    ld_local = ld;
    // parse all data and fill the structures for button to speed up drawing afterwards
    // setup default state
    current_state = -1;
    if (elements.size() > 0) current_state = 0;
    if ( (init_state < elements.size()) && (init_state >= 0)) current_state = init_state;

    //g_list->addItem(QString("line 1"));

}

GCodeView::~GCodeView(){
    if (g_list != NULL) delete g_list;
}

void GCodeView::drawLayoutObject(QPainter& painter, QPoint &loffset){
    // get actual data from the structure and do actual drawing of the gcode viewer
    // 1) get current state
    if (current_state < 0 ){
        qDebug() << "g code view has no states, fix ini file";
        return;
    }
    // check if the element should appear on layout
    if (this->is_visible() && ld_local != NULL){
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        //qDebug() << "got state " <<  elements [current_state] ;
        QPoint * pt = cs->get_Position(ld_local);
        //qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
        QPoint * sz = cs->get_Size(ld_local);
        //qDebug() << "size x= " << sz->x() << "size y = " << sz->y();
        // get the unit size
        QString us = QString("ucell");
        int ucell = ld_local->get_int_value_by_name(us);
        //qDebug() << "ucell= " << ucell;
        /*
        //loop over all pathes
        int path_num = cs->get_paths_number();
        for (int i=0; i<path_num; i++) {
            QString bgcolor = cs->get_bgColor(ld_local, i);
            //qDebug() << "bgcol = " << bgcolor;
            QString pencolor = cs->get_pColor(ld_local, i);
            //qDebug() << "pcol = " << pencolor;
            // get PIX
            Path2D * pix = cs->get_Pix(ld_local, i);
            pix->drawPath(painter, pencolor, bgcolor, pt, sz, ucell, loffset );
        }*/

        file_name = cs->get_file_name(ld_local);
        qDebug() << "file name = " << file_name;
        removeQuotes(file_name);
        if (isFileNameChanged(file_name)){
            g_list->clear();
            program.clear();
            if (!file_name.isEmpty()){
                QFile file(file_name);
                if (!file.open(QIODevice::ReadOnly)) {
                    // set filename to empty
                    file_name = "";
                    old_file_name = "";
                }
                else {
                    QTextStream in(&file);
                    while(!in.atEnd()){
                        QString tmp =  in.readLine();
                        program.push_back(tmp);
                    }
                    file.close();
                }
            }
        }

        if (g_list->count() == 0) {
            if(file_name.isEmpty()) g_list->addItem(QString("<no file>"));
            else{
                g_list->addItem( QString("(") + file_name + QString(")"));
                // loading file if it's valid
                if (program.size()>0){
                    for(int i =0; i<program.size(); i++) g_list->addItem(program[i]);
                }

            }
        }
        //draw list on top of this
        if (g_list != NULL){
            g_list->setGeometry( (pt->x()-1)*ucell+loffset.x(),  (pt->y()-1)*ucell + loffset.y(), sz->x()*ucell,sz->y()*ucell );
            QString us = QString("font_size");
            int fsize = ld_local->get_int_value_by_name(us);
            QString ff = QString("font_family");
            QString f_family = ld_local->get_string_value_by_name(ff);
            QFont gc_font(f_family,fsize); // make font autoload to state
            g_list->setFont(gc_font);
            g_list->setVisible(TRUE);

        }
    }
    if (!(this->is_visible())){
        g_list->setVisible(FALSE);

    }
}

void GCodeView::removeQuotes( QString & str){
    if (str.size() >= 2) {
        if ((str[str.size()-1] == QChar('\"')) || (str[str.size()-1] == QChar('\''))) str.remove(str.size()-1,1);
        if ((str[0] == QChar('\"')) || (str[0] == QChar('\''))) str.remove(0,1);
    }
}
bool GCodeView::isCursorOver(QPoint& pos){
    if (current_state < 0 ){
        qDebug() << "gcodeview has no states, fix ini file";
        return false;
    }
    if (this->is_visible() && ld_local != NULL){
        //qDebug() << "gcodeview is visible";
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        //qDebug() << "button state =" << cs;
        QPoint * pt = cs->get_Position(ld_local);
        //qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
        QPoint * sz = cs->get_Size(ld_local);
        //qDebug() << "size x= " << sz->x() << "size y = " << sz->y();
        QString ustr = QString("ucell");
        int ucell = ld_local->get_int_value_by_name(ustr);
        if ( (pos.x() >= (pt->x()-1)*ucell) && (pos.x() < (pt->x()+sz->x()-1) *ucell) &&
             (pos.y() >= (pt->y()-1)*ucell) && (pos.y() < (pt->y()+sz->y()-1) *ucell) )
            return true;
    }
    return false;
}

bool GCodeView::setState(int state){
    if ( (state >= 0) && (state < elements.size())) {
        current_state = state;
        return true;
    }
    return false;
}

bool GCodeView::isFileNameChanged(QString new_file_name){
    if (old_file_name.compare(new_file_name) == 0){
        return FALSE;
    }
    old_file_name = new_file_name;
    return TRUE;
}
// to define later
void GCodeView::selectLayoutObject(){}
void GCodeView::setLayoutObjectStatus(){}
void GCodeView::getLayoutObjectStatus(){}
void GCodeView::setLayoutObjectPropertiy(){}
void GCodeView::getLayoutObjectPropertiy(){}
