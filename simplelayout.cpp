#include "simplelayout.h"

SimpleLayout::SimpleLayout(QStringList & sl)
{
    selected = false;
    elements = sl;
}

bool SimpleLayout::select_layout(bool new_state){
    bool old_s = selected;
    selected = new_state;
    return old_s;
}
