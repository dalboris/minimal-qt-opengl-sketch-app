#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>

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
    OpenGLFunctions * openGLFunctions() const;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void cleanupGL();

private:
    QMatrix4x4 projectionMatrix_;
    QMatrix4x4 viewMatrix_;
};

#endif // OPENGLWIDGET_H
