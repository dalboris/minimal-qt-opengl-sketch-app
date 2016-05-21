#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    viewMatrix_.setToIdentity();
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    cleanupGL();
    doneCurrent();
}

OpenGLFunctions * OpenGLWidget::openGLFunctions() const
{
    return context()->versionFunctions<OpenGLFunctions>();
}

void OpenGLWidget::initializeGL()
{
    OpenGLFunctions * f = openGLFunctions();

    f->glClearColor(1, 1, 1, 1);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    const float left   = 0.0f;
    const float right  = w;
    const float bottom = h;
    const float top    = 0.0f;
    const float near   = -1.0f;
    const float far    = 1.0f;

    projectionMatrix_.setToIdentity();
    projectionMatrix_.ortho(left, right, bottom, top, near, far);
}

void OpenGLWidget::paintGL()
{
    OpenGLFunctions * f = openGLFunctions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void OpenGLWidget::cleanupGL()
{
    OpenGLFunctions * f = openGLFunctions();

}
