#include "state.h"
#include <QDebug>

State::State(QStringList &sl)
{
    ss = sl;
    qDebug()<< "state object::" << ss;
    // Complex objects are interpreted during execution time.
    // The variable name is defined in front of it's type specifier.
    // special functions can extract parameters and use it for drawing or changing.
}

bool State::update_state_fields(){
   return true;
}
