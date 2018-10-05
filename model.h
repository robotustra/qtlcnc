#ifndef MODEL_H
#define MODEL_H

#ifdef QT_VERSION_48
#undef QT_VERSION_48
#endif

#if QT_VERSION >= 5
#define QT_VERSION_48 1
#else
#define QT_VERSION_48 0
#endif


#if (QT_VERSION_48)
#include <QtOpenGL>
#else
#include <qopengl.h>
#endif
#include <QVector>
#include <QVector3D>

class Model
{
public:
    Model();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }

private:
    void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v, const QVector3D &n);

    QVector<GLfloat> m_data;
    int m_count;
};

#endif // MODEL_H
