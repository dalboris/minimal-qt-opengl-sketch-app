#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <vector>

using OpenGLFunctions = QOpenGLFunctions_3_2_Core;

class OpenGLWidget : public QOpenGLWidget
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLWidget)

public:
    OpenGLWidget(QWidget * parent = nullptr);
    ~OpenGLWidget();

private:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    OpenGLFunctions * openGLFunctions() const;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void cleanupGL();

private:
    void computeGLSamples_();

private:
    QMatrix4x4 projectionMatrix_;
    QMatrix4x4 viewMatrix_;

    std::vector<QPoint> inputSamples_;

    struct GLVertex { float x, y; };
    std::vector<GLVertex> glSamples_;

    QOpenGLBuffer vbo_;
    QOpenGLVertexArrayObject vao_;
};

#endif // OPENGLWIDGET_H
