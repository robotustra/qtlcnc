#ifndef LCS_H
#define LCS_H

#include <vector>
#include <QVector3D>
#include <QVector4D>

/* Local system of coordinates of objects. this coordinate system is transforming with the mesh.

*/

class LCS
{

public:
    LCS();
    LCS(QVector3D org0, QVector4D quad0 );
    //void add_lcs(QVector3D& org0, QVector4D& quad0);
    void set_quat(QVector4D quad0);
    void set_vec(QVector3D org0);
    //void rotate(QVector4D& quat); //rotate whole LCS with respect to the origin
    //void translate( QVector3D v);

protected:

    QVector3D org; // the coordinates of Origial coordinate system with respect to the body (mesh).
    QVector4D oquat; // the rotation of the system og coordinates with respect to the body (mesh)

    friend class ArenaLoader;
    friend class Mesh;
    friend class Arena;

};

#endif // LCS_H
