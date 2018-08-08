#ifndef LAYOUTOBJECT_H
#define LAYOUTOBJECT_H
#include <QPainter>

/*
    This is an abstract class from which are all layout objects a inherited
    like: MyButton, MyIndicator, MyGLView, etc...
*/


class MyLayoutObject
{
public:

    MyLayoutObject();
    // interface for all children
    void drawLayoutObject( ){ visible = true; }
    void selectLayoutObject(){}
    void setLayoutObjectStatus(){}
    void getLayoutObjectStatus(){}
    void setLayoutObjectPropertiy(){}
    void getLayoutObjectPropertiy(){}
    void hide(){ visible = false; }
    void show(){ visible = true ; }
    bool is_visible(){ return visible; }

private:
    bool visible;

};

#endif // LAYOUTOBJECT_H
