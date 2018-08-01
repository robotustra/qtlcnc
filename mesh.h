#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QtOpenGL/QtOpenGL>

#include <vector>
#include "lcs.h"

class Mesh;

class Mesh
{
public:

    Mesh(const Mesh& m);

    void translate(QVector3D v);
    void rotate(QVector4D v);
    void set_lcs(LCS * l);
    LCS* get_lcs(int n);
    void move_nested(LCS * l);
    void apply_transforms();
    void print(int size);
    void reinitMesh(const Mesh& m);

    float min(size_t start) const;
    float max(size_t start) const;

    float xmin() const { return min(0); }
    float ymin() const { return min(1); }
    float zmin() const { return min(2); }
    float xmax() const { return max(0); }
    float ymax() const { return max(1); }
    float zmax() const { return max(2); }

    bool empty() const;

private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    QString file_name;

    std::vector<LCS*> lcs;

    std::vector<Mesh*> nested;
    bool hide;
    float v_sum;
    float v_ind;

    friend class GLMesh;
    friend class ArenaLoader;
    friend class Canvas;
    friend class Arena;
};

#endif // MESH_H
