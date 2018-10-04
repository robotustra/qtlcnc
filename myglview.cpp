#include "myglview.h"
#include "glwidget.h"
#include "state.h"
#include "mainwindow.h"
#include <QPoint>
#include <QDebug>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>

MyGLView::MyGLView(MainWindow* p, QStringList &sl, LayoutData *ld, int init_state)
{
    parent = p;
    //g_list = new QListWidget(p);
    elements = sl;
    ld_local = ld;
    // parse all data and fill the structures for button to speed up drawing afterwards
    // setup default state
    current_state = -1;
    if (elements.size() > 0) current_state = 0;
    if ( (init_state < elements.size()) && (init_state >= 0)) current_state = init_state;

    //g_list->addItem(QString("line 1"));

    glWidget = new GLWidget(parent);

    QHBoxLayout *container = new QHBoxLayout;
    container->setContentsMargins(0,0,0,0); // remove border around glview
    container->addWidget(glWidget);
    p->pcwt->setLayout(container);
    p->setCentralWidget(p->pcwt);

}

MyGLView::~MyGLView(){
    if (g_list != NULL) delete g_list;
    if (glWidget != NULL) delete glWidget;

}

QSlider* MyGLView::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MyGLView::drawLayoutObject(QPainter& painter, QPoint &loffset){
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
        float x0 = (pt->x()-1) * ucell + loffset.x();
        float y0 = (pt->y()-1) * ucell + loffset.y();
        float sx = sz->x() * ucell;
        float sy = sz->y() * ucell;

        // set geometry of the glview
        if (parent != NULL && parent->pcwt != NULL ){
            parent->pcwt->setGeometry(x0,y0,sx,sy); // to set position I break layout of the main window
        }

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

        /*
        file_name = cs->get_file_name(ld_local);
        //qDebug() << "file name = " << file_name;
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
        }*/
        /*
        if (g_list->count() == 0) {
            if(file_name.isEmpty()) g_list->addItem(QString("<no file>"));
            else{
                g_list->addItem( QString("(") + file_name + QString(")"));
                // loading file if it's valid
                if (program.size()>0){
                    for(int i =0; i<program.size(); i++) g_list->addItem(program[i]);
                }

            }
        }*/
        //draw list on top of this
        /*
        if (g_list != NULL){
            g_list->setGeometry( (pt->x()-1)*ucell+loffset.x(),  (pt->y()-1)*ucell + loffset.y(), sz->x()*ucell,sz->y()*ucell );
            QString us = QString("font_size");
            int fsize = ld_local->get_int_value_by_name(us);
            QString ff = QString("font_family");
            QString f_family = ld_local->get_string_value_by_name(ff);
            QFont gc_font(f_family,fsize); // make font autoload to state
            g_list->setFont(gc_font);
            g_list->setVisible(TRUE);

        }*/
    }
    /*
    if (!(this->is_visible())){
        g_list->setVisible(FALSE);

    }*/
}

void MyGLView::removeQuotes( QString & str){
    if (str.size() >= 2) {
        if ((str[str.size()-1] == QChar('\"')) || (str[str.size()-1] == QChar('\''))) str.remove(str.size()-1,1);
        if ((str[0] == QChar('\"')) || (str[0] == QChar('\''))) str.remove(0,1);
    }
}
bool MyGLView::isCursorOver(QPoint& pos){
    if (current_state < 0 ){
        qDebug() << "MyGLView has no states, fix ini file";
        return false;
    }
    if (this->is_visible() && ld_local != NULL){
        //qDebug() << "MyGLView is visible";
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

bool MyGLView::setState(int state){
    if ( (state >= 0) && (state < elements.size())) {
        current_state = state;
        return true;
    }
    return false;
}

bool MyGLView::isFileNameChanged(QString new_file_name){
    if (old_file_name.compare(new_file_name) == 0){
        return FALSE;
    }
    old_file_name = new_file_name;
    return TRUE;
}
// to define later
void MyGLView::selectLayoutObject(){}
void MyGLView::setLayoutObjectStatus(){}
void MyGLView::getLayoutObjectStatus(){}
void MyGLView::setLayoutObjectPropertiy(){}
void MyGLView::getLayoutObjectPropertiy(){}
