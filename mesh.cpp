#include <QFile>
#include <QDataStream>
#include <QVector3D>
#include <algorithm>

#include <cmath>

#include "mesh.h"

////////////////////////////////////////////////////////////////////////////////

Mesh::Mesh(const Mesh& m)
{
    v_sum = 0.0;
    v_ind = 0;
    for (uint i=0; i<m.vertices.size(); i++){
        vertices.push_back(m.vertices[i]);
        v_sum += m.vertices[i];
    }
    for (uint i=0; i<m.indices.size(); i++){
        indices.push_back(m.indices[i]);
        v_ind += m.indices[i];
    }
    file_name = m.file_name;

    for (uint i=0; i<m.lcs.size(); i++) lcs.push_back(m.lcs[i]);
    //lcs.assign(m.lcs.begin(), m.lcs.end());

    nested.assign(m.nested.begin(),m.nested.end());
    hide = m.hide;
    //qDebug() << "check sum: " << v_sum << ", " << v_ind ;

}

void Mesh::reinitMesh(const Mesh& m)
{
    float v_sum0 = 0.0;
    int v_ind0 = 0;
    /*vertices.clear();
    indices.clear();
    vertices = m.vertices;
    indices = m.indices;
    */
    vertices.clear();
    indices.clear();
    for (uint i=0; i<m.vertices.size(); i++){
        vertices.push_back(m.vertices[i]);
        v_sum0 += m.vertices[i];
    }
    for (uint i=0; i<m.indices.size(); i++){
        indices.push_back(m.indices[i]);
        v_ind0 += m.indices[i];
    }
    /*
    vertices.clear();
    indices.clear();
    vertices.assign(m.vertices.begin(), m.vertices.end());
    indices.assign(m.indices.begin(), m.indices.end());
    */
    if (v_sum0 != v_sum) qDebug() << "Error checksum for vectors! ";
    if (v_ind0 != v_ind) qDebug() << "Error checksum for indexes! ";

}

float Mesh::min(size_t start) const
{
    if (start >= vertices.size())
    {
        return -1;
    }
    float v = vertices[start];
    for (size_t i=start; i < vertices.size(); i += 3)
    {
        v = fmin(v, vertices[i]);
    }
    return v;
}

float Mesh::max(size_t start) const
{
    if (start >= vertices.size())
    {
        return 1;
    }
    float v = vertices[start];
    for (size_t i=start; i < vertices.size(); i += 3)
    {
        v = fmax(v, vertices[i]);
    }
    return v;
}

bool Mesh::empty() const
{
    return vertices.size() == 0;
}

void Mesh::translate(QVector3D v){
    // translates a mesh to the vector.
    //qDebug() << "translate: number of vertices = " << vertices.size();
    for (uint i = 0; i < (vertices.size()/3); i++ ){
        vertices[3*i] += v.x();
        vertices[3*i+1] += v.y();
        vertices[3*i+2] += v.z();
    }
}

void Mesh::rotate(QVector4D v){
    // rotate a mesh to the vector angles.
    //qDebug() << "rotate: number of vertices = " << vertices.size();
    QMatrix4x4 mat;
    mat.rotate(v.w(), QVector3D(v.x(), v.y(), v.z())); // w = angle of rotation
    for (uint i = 0; i < (vertices.size()/3); i++ ){
        QVector3D vt(vertices[3*i], vertices[3*i+1], vertices[3*i+2]);
        QVector3D a = mat.inverted() * vt;
        vertices[3*i] = a.x();
        vertices[3*i+1] = a.y();
        vertices[3*i+2] = a.z();
    }
}

void Mesh::set_lcs(LCS * l){
    lcs.push_back(l);
}

LCS* Mesh::get_lcs(int n){
    if ((n>=0) && (n < lcs.size())) return lcs[n];
    return NULL;
}

void Mesh::move_nested(LCS * l){

    set_lcs(l);
    for (uint i = 0; i<this->nested.size(); i++){
        Mesh * m = this->nested[i];
        m->set_lcs(l);
    }
}

void Mesh::apply_transforms(){
    //Move this mesh and all nested to the destination
    if (lcs.size()>0){
        for (uint i = 0; i<lcs.size(); i++){
            if (lcs[i] == NULL) qDebug() << "NULL LCS!";
            else{
                translate( lcs[i]->org );
                rotate( lcs[i]->oquat );
            }
        }
    }
}

void Mesh::print(int size){
    qDebug( ) << "Mesh vert: ";
    for (uint i=0; i<size; i++){
        qDebug() << vertices[i];
    }
    qDebug( ) << "Mesh ind: ";
    for (uint i=0; i<size; i++){
        qDebug() << indices [i];
    }
}
