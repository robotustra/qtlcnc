#include "gcodeview.h"

GCodeView::GCodeView(QStringList &sl, LayoutData *ld, int init_state)
{
    //...........to continue...
    /*
    elements = sl;
    ld_local = ld;
    // parse all data and fill the structures for button to speed up drawing afterwards
    // setup default state
    current_state = -1;
    if (elements.size() > 0) current_state = 0;
    if ( (init_state < elements.size()) && (init_state >= 0)) current_state = init_state;
    */
}

void GCodeView::drawLayoutObject(QPainter& p, QPoint &loffset){
    /*
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
            //qDebug() << "got state " <<  elements [current_state] ;
            QPoint * pt = cs->get_Position(ld_local);
            //qDebug() << "pt x= " << pt->x() << "pt y = " << pt->y();
            QPoint * sz = cs->get_Size(ld_local);
            //qDebug() << "size x= " << sz->x() << "size y = " << sz->y();
            // get the unit size
            QString us = QString("ucell");
            int ucell = ld_local->get_int_value_by_name(us);
            //qDebug() << "ucell= " << ucell;

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
            }
        }
    */
}

// to define later
void GCodeView::selectLayoutObject(){}
void GCodeView::setLayoutObjectStatus(){}
void GCodeView::getLayoutObjectStatus(){}
void GCodeView::setLayoutObjectPropertiy(){}
void GCodeView::getLayoutObjectPropertiy(){}
