#include "simplelayout.h"

SimpleLayout::SimpleLayout(QStringList & sl)
{
    selected = false;
    elements = sl;
}

bool SimpleLayout::select_layout(bool new_state){
    selected = new_state;
    return true;
}
