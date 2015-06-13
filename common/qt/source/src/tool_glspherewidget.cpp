/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
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

#include <QtWidgets>
#include <QtOpenGL>
#include <QMessageBox>

#include "tool_glspherewidget.h"

namespace Magus
{
    //****************************************************************************/
    QtGLSphereWidget::QtGLSphereWidget(QWidget *parent, QGLWidget *shareWidget)
        : QGLWidget(parent, shareWidget)
    {
        clearColor = Qt::black;
        xRot = 0;
        yRot = 0;
        zRot = 0;
        program = 0;
    }

    //****************************************************************************/
    QtGLSphereWidget::~QtGLSphereWidget()
    {
    }

    //****************************************************************************/
    QSize QtGLSphereWidget::minimumSizeHint() const
    {
        return QSize(16, 16);
    }

    //****************************************************************************/
    QSize QtGLSphereWidget::sizeHint() const
    {
        return QSize(200, 200);
    }

    //****************************************************************************/
    void QtGLSphereWidget::rotateBy(int xAngle, int yAngle, int zAngle)
    {
        xRot += xAngle;
        yRot += yAngle;
        zRot += zAngle;
        updateGL();
    }

    //****************************************************************************/
    void QtGLSphereWidget::setClearColor(const QColor &color)
    {
        clearColor = color;
        updateGL();
    }

    //****************************************************************************/
    void QtGLSphereWidget::setPixmap(const QPixmap& pixmap, const QString& fullName, const QString& baseName)
    {
        mPixmap = pixmap;
        mFullName = fullName;
        mBaseName = baseName;
    }

    //****************************************************************************/
    const QString& QtGLSphereWidget::getFullName(void) const
    {
        return mFullName;
    }

    //****************************************************************************/
    const QString& QtGLSphereWidget::getBaseName(void) const
    {
        return mBaseName;
    }

    //****************************************************************************/
    void QtGLSphereWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        buildSphere();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    #ifdef GL_TEXTURE_2D
        glEnable(GL_TEXTURE_2D);
    #endif

    #define PROGRAM_VERTEX_ATTRIBUTE 0
    #define PROGRAM_TEXCOORD_ATTRIBUTE 1

        QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
        const char *vsrc =
            "attribute highp vec4 vertex;\n"
            "attribute mediump vec4 texCoord;\n"
            "varying mediump vec4 texc;\n"
            "uniform mediump mat4 matrix;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    texc = texCoord;\n"
            "}\n";
        vshader->compileSourceCode(vsrc);

        QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
        const char *fsrc =
            "uniform sampler2D texture;\n"
            "varying mediump vec4 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc.st);\n"
            "}\n";
        fshader->compileSourceCode(fsrc);

        program = new QGLShaderProgram(this);
        program->addShader(vshader);
        program->addShader(fshader);
        program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
        program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
        program->link();

        program->bind();
        program->setUniformValue("texture", 0);

        // Set the clear color and rotate the sphere
        QColor clearColor;
        clearColor = Qt::black;
        setClearColor(clearColor);
        rotateBy(0 * 16, -90 * 16, 180 * 16);
    }

    //****************************************************************************/
    void QtGLSphereWidget::paintGL()
    {
        qglClearColor(clearColor);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        QMatrix4x4 m;
        m.ortho(-0.5f, +0.5f, +0.5f, -0.5f, 4.0f, 15.0f);
        m.translate(0.0f, 0.0f, -10.0f);
        m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
        m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
        m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

        program->setUniformValue("matrix", m);
        program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
        program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
        program->setAttributeArray
            (PROGRAM_VERTEX_ATTRIBUTE, vertices.constData());
        program->setAttributeArray
            (PROGRAM_TEXCOORD_ATTRIBUTE, texCoords.constData());

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, TOOL_SPHERE_VERTS);
    }

    //****************************************************************************/
    void QtGLSphereWidget::resizeGL(int width, int height)
    {
        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);
    }

    //****************************************************************************/
    void QtGLSphereWidget::mousePressEvent(QMouseEvent *event)
    {
        lastPos = event->pos();
        emit selected(mFullName, mBaseName);
    }

    //****************************************************************************/
    void QtGLSphereWidget::mouseMoveEvent(QMouseEvent *event)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton)
        {
            rotateBy(8 * dy, 8 * dx, 0);
        }
        else if (event->buttons() & Qt::RightButton)
        {
            rotateBy(8 * dy, 0, 8 * dx);
        }
        lastPos = event->pos();
    }

    //****************************************************************************/
    void QtGLSphereWidget::mouseReleaseEvent(QMouseEvent * /* event */)
    {
        emit clicked();
    }

    //****************************************************************************/
    void QtGLSphereWidget::buildSphere(void)
    {
        texture = bindTexture(mPixmap, GL_TEXTURE_2D);

        float cx = 0.0f;
        float cy = 0.0f;
        float cz = 0.0f;

        float theta1 = 0.0;
        float theta2 = 0.0;
        float theta3 = 0.0;

        float ex = 0.0f;
        float ey = 0.0f;
        float ez = 0.0f;

        float px = 0.0f;
        float py = 0.0f;
        float pz = 0.0f;

        for (int i = 0; i < TOOL_SPHERE_PRECISIONDIV2; ++i)
        {
            theta1 = i * TOOL_SPHERE_TWOPI / TOOL_SPHERE_PRECISION - TOOL_SPHERE_PIDIV2;
            theta2 = (i + 1) * TOOL_SPHERE_TWOPI / TOOL_SPHERE_PRECISION - TOOL_SPHERE_PIDIV2;
            for (int j = 0; j <= TOOL_SPHERE_PRECISION; ++j)
            {
                theta3 = j * TOOL_SPHERE_TWOPI / TOOL_SPHERE_PRECISION;
                ex = cosf(theta2) * cosf(theta3);
                ey = sinf(theta2);
                ez = cosf(theta2) * sinf(theta3);
                px = cx + TOOL_SPHERE_RADIUS * ex;
                py = cy + TOOL_SPHERE_RADIUS * ey;
                pz = cz + TOOL_SPHERE_RADIUS * ez;

                vertices.append(QVector3D(px, py, pz));
                texCoords.append (QVector2D( -(j/(float)TOOL_SPHERE_PRECISION), 2*(i+1)/(float)TOOL_SPHERE_PRECISION));

                ex = cosf(theta1) * cosf(theta3);
                ey = sinf(theta1);
                ez = cosf(theta1) * sinf(theta3);
                px = cx + TOOL_SPHERE_RADIUS * ex;
                py = cy + TOOL_SPHERE_RADIUS * ey;
                pz = cz + TOOL_SPHERE_RADIUS * ez;

                vertices.append(QVector3D(px, py, pz));
                texCoords.append (QVector2D(-(j/(float)TOOL_SPHERE_PRECISION), 2*i/(float)TOOL_SPHERE_PRECISION));
            }
        }
    }
}
