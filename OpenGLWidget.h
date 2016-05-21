#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <vector>

typedef QOpenGLFunctions_3_2_Core OpenGLFunctions;

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
    QPointF computeNormal_(const QPoint & p, const QPoint & q);
    void computeGLVertices_();

private:
    // Projection and view matrix
    QMatrix4x4 projMatrix_;
    QMatrix4x4 viewMatrix_;

    // Input mouse data
    std::vector<QPoint> inputSamples_;

    // RAM resources synced with GL resources
    struct GLVertex {
        float x, y;
        GLVertex(float x, float y) : x(x), y(y) {}
    };
    std::vector<GLVertex> glVertices_;

    // GL resources
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLBuffer vbo_;
    QOpenGLVertexArrayObject vao_;

    // Shader locations
    int vertexLoc_;
    int projMatrixLoc_;
    int viewMatrixLoc_;
};

#endif // OPENGLWIDGET_H
