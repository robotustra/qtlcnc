#include <QDebug>
#include <QMatrix4x4>
#include "lcs.h"

LCS::LCS()
{
    org = QVector3D(0.0f,0.0f,0.0f); // the coordinates of Origial coordinate system with respect to the body (mesh).
    oquat = QVector4D(0.0f,0.0f,0.0f, 0.0f); // the rotation of the system og coordinates with respect to the body (mesh)
}

LCS::LCS(QVector3D org0, QVector4D quad0 )
{
    org = org0; // the coordinates of Origial coordinate system with respect to the body (mesh).
    oquat = quad0; // the rotation of the system og coordinates with respect to the body (mesh)
}

void LCS::set_quat(QVector4D quad0){
    oquat = quad0;
}
void LCS::set_vec(QVector3D org0){
    org = org0;
}

