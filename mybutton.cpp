#include "mybutton.h"

MyButton::MyButton(QStringList & sl, LayoutData* ld)
{
    elements = sl;
    ld_local = ld;
    // parse all data and fill the structures for button to speed up drawing afterwards
    // setup default state
    current_state = -1;
    if (elements.size() > 0) current_state = 0;

}

void MyButton::drawLayoutObject(QPainter &painter){
// get actual data from the structure and do actual drawing of the button
    // 1) get current state
    if (current_state < 0 ){
        qDebug() << "button has no states, fix ini file";
        return;
    }
    // check if the element should appear on layout
    if (this->is_visible() && ld_local != NULL){
        //get the state configuration
        MyState* cs = ld_local->get_state_object_by_name( elements [current_state] );
        qDebug() << "got state " <<  elements [current_state] ;
        QPoint * pt = cs->get_Position(ld_local);
        qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
        pt = cs->get_Size(ld_local);
        qDebug() << "size x= " << pt->x() << "size y = " << pt->y();
        // get the unit size
        QString us = QString("ucell");
        int ucell = ld_local->get_int_value_by_name(us);
        qDebug() << "ucell= " << ucell;
        // get color
        QString bgcolor = cs->get_Color(ld_local, "BGCOL");
        qDebug() << "bgcol = " << bgcolor;

        //QString pencolor = cs->get_Color(ld_local, "PCOL");

        //qDebug() << "pcol = " << pencolor;


    }

}

// to define later
void MyButton::selectLayoutObject(){}
void MyButton::setLayoutObjectStatus(){}
void MyButton::getLayoutObjectStatus(){}
void MyButton::setLayoutObjectPropertiy(){}
void MyButton::getLayoutObjectPropertiy(){}

