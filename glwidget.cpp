/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QMouseEvent>


#if (QT_VERSION_48)
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLContext>
#else
#include <QOpenGLShaderProgram>
#endif

#include <QCoreApplication>
#include <math.h>

bool GLWidget::m_transparent = false;

GLWidget::GLWidget(QWidget *parent, int w, int h)
#if (QT_VERSION_48)
    : QGLWidget(parent),
#else
    : QOpenGLWidget(parent),
#endif
      m_xRot(0),
      m_yRot(0),
      m_zRot(0),
      m_program(0)
{
    //m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    m_core = false; //run core shaders which should work ok
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    /*if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }*/
    w_viewport = w;
    h_viewport = h;
}

void GLWidget::setViewPort(int w,int h){
    w_viewport = w;
    h_viewport = h;
    glViewport(0,0,w,h);
    glOrtho(-1.0,+1.0,-1.0,+1.0,-90.0,+90.0);
}

GLWidget::~GLWidget()
{
    cleanup();
}

#if QT_VERSION_48
QSize GLWidget::minimumSizeHint()// const
#else
QSize GLWidget::minimumSizeHint() const
#endif
{
    return QSize(50, 50);
}

#if QT_VERSION_48
QSize GLWidget::sizeHint()// const
#else
QSize GLWidget::sizeHint() const
#endif
{
    return QSize(200, 200);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::cleanup()
{
    if (m_program == NULL)
        return;
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
    if (m_program1 == NULL)
        return;
    makeCurrent();
    m_logoVbo1.destroy();
    delete m_program1;
    m_program1 = 0;
    doneCurrent();
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 0.8);\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *my_vertexShaderSource =
    "attribute vec4 vertex1;\n"
    "attribute vec3 normal1;\n"
    "varying vec3 vert1;\n"
    "varying vec3 vertNormal1;\n"
    "uniform mat4 projMatrix1;\n"
    "uniform mat4 mvMatrix1;\n"
    "uniform mat3 normalMatrix1;\n"
    "void main() {\n"
    "   vert1 = vertex1.xyz;\n"
    "   vertNormal1 = normalMatrix1 * normal1;\n"
    "   gl_Position = projMatrix1 * mvMatrix1 * vertex1;\n"
    "}\n";


static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 0.8);\n"
    "}\n";

static const char *my_fragmentShaderSource =
    "varying highp vec3 vert1;\n"
    "varying highp vec3 vertNormal1;\n"
    "uniform highp vec3 lightPos1;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos1 - vert1);\n"
    "   highp float NL = max(dot(normalize(vertNormal1), L), 0.0);\n"
    "   highp vec3 color = vec3(0.4, 0.5, 1.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.1, 0.9);\n"
    "   gl_FragColor = vec4(col, 0.5);\n"
    "}\n";


void GLWidget::initializeGL()
{
#if (QT_VERSION_48)
    initializeGLFunctions();
#else
    initializeOpenGLFunctions();
#endif

    glClearColor(0.25, 0.5, 0, 1.0); // not transparent
    glViewport(0,0,w_viewport,h_viewport);
#if (QT_VERSION_48)
    m_program = new QGLShaderProgram;
    m_program->addShaderFromSourceCode(QGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
#else
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
#endif
    m_program->link();

#if (QT_VERSION_48)
    m_program1 = new QGLShaderProgram;
    m_program1->addShaderFromSourceCode(QGLShader::Vertex, my_vertexShaderSource);
    m_program1->addShaderFromSourceCode(QGLShader::Fragment, my_fragmentShaderSource);
#else
    m_program1 = new QOpenGLShaderProgram;
    m_program1->addShaderFromSourceCode(QOpenGLShader::Vertex, my_vertexShaderSource);
    m_program1->addShaderFromSourceCode(QOpenGLShader::Fragment, my_fragmentShaderSource);
#endif
    m_program1->link();

    // setup vertex data

    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));
    m_logoVbo.release();

    m_logoVbo1.create();
    m_logoVbo1.bind();
    m_logoVbo1.allocate(m_logo1.constData(), m_logo1.count() * sizeof(GLfloat));
    m_logoVbo1.release();

}

void GLWidget::setupVertexAttribs(const GLuint v, const GLuint n)
{
    m_logoVbo.bind();
#if (QT_VERSION_48)
    //QGLFunctions *f = QGLContext::functions();
    QGLFunctions::glEnableVertexAttribArray(v);
    QGLFunctions::glEnableVertexAttribArray(n);
    QGLFunctions::glVertexAttribPointer(v, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    QGLFunctions::glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
#else
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(v);
    f->glEnableVertexAttribArray(n);
    f->glVertexAttribPointer(v, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
#endif
    m_logoVbo.release();
}

void GLWidget::setupVertexAttribs1(const GLuint v, const GLuint n)
{
    m_logoVbo1.bind();
#if (QT_VERSION_48)
    //QGLFunctions *f = QGLContext::functions();
    QGLFunctions::glEnableVertexAttribArray(v);
    QGLFunctions::glEnableVertexAttribArray(n);
    QGLFunctions::glVertexAttribPointer(v, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    QGLFunctions::glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
#else
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(v);
    f->glEnableVertexAttribArray(n);
    f->glVertexAttribPointer(v, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(n, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
#endif
    m_logoVbo1.release();
}

void GLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); //back side of the object
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparancy works for all programs
    glEnable( GL_BLEND ); //glClearColor(0.0, 0.0, 0, 1.0); // clear color is done once

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_program->setUniformValue(m_lightPosLoc, QVector3D(10, 10, -10));

    m_camera.setToIdentity();
    m_camera.translate(0, 0, -3);
    m_camera.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_camera.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_camera.rotate(m_zRot / 16.0f, 0, 0, 1);

    m_world.setToIdentity();
    m_world.translate(0.5,0.5,-1.0);

    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    // Find and enable the attribute location for vertex position
    const GLuint vp = m_program->attributeLocation("vertex");
    const GLuint nm = m_program->attributeLocation("normal");
    //glEnableVertexAttribArray(vp);
    //glEnableVertexAttribArray(nm);
    setupVertexAttribs(vp, nm);

    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

    // Clean up state machine
    glDisableVertexAttribArray(vp);
    glDisableVertexAttribArray(nm);

    m_logoVbo.release();
    m_program->release();

    // another program

    m_program1->bind();
    m_projMatrixLoc1 = m_program1->uniformLocation("projMatrix1");
    m_mvMatrixLoc1 = m_program1->uniformLocation("mvMatrix1");
    m_normalMatrixLoc1 = m_program1->uniformLocation("normalMatrix1");
    m_lightPosLoc1 = m_program1->uniformLocation("lightPos1");

    m_program1->setUniformValue(m_lightPosLoc1, QVector3D(10, -10, -10));

    m_camera.setToIdentity();
    m_camera.translate(0, 0, -3);
    m_camera.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_camera.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_camera.rotate(m_zRot / 16.0f, 0, 0, 1);

    m_world1.setToIdentity();
    m_world1.translate(-0.2,-0.2,-0.3);

    m_program1->setUniformValue(m_projMatrixLoc1, m_proj);
    m_program1->setUniformValue(m_mvMatrixLoc1, m_camera * m_world1);
    QMatrix3x3 normalMatrix1 = m_world1.normalMatrix();
    m_program1->setUniformValue(m_normalMatrixLoc1, normalMatrix1);

    // Find and enable the attribute location for vertex position
    const GLuint vp1 = m_program1->attributeLocation("vertex1");
    const GLuint nm1 = m_program1->attributeLocation("normal1");
    //glEnableVertexAttribArray(vp);
    //glEnableVertexAttribArray(nm);
    setupVertexAttribs1(vp1, nm1);

    glDrawArrays(GL_TRIANGLES, 0, m_logo1.vertexCount());

    // Clean up state machine
    glDisableVertexAttribArray(vp1);
    glDisableVertexAttribArray(nm1);

    m_logoVbo1.release();
    m_program1->release();


}

void GLWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}
