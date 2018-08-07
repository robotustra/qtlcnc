#ifndef LAYOUTOBJECT_H
#define LAYOUTOBJECT_H

/*
    This is an abstract class from which are all layout objects a inherited
    like: MyButton, MyIndicator, MyGLView, etc...
*/

class LayoutObject
{
public:
    LayoutObject();
    void drawLayoutObject(){};
    void selectLayoutObject(){};
    void setLayoutObjectStatus(){};
    void getLayoutObjectStatus(){};
    void setLayoutObjectPropertiy(){};
    void getLayoutObjectPropertiy(){};

};

#endif // LAYOUTOBJECT_H
