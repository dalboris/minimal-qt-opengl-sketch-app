#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
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

}

void OpenGLWidget::resizeGL(int w, int h)
{
    OpenGLFunctions * f = openGLFunctions();

}

void OpenGLWidget::paintGL()
{
    OpenGLFunctions * f = openGLFunctions();

}

void OpenGLWidget::cleanupGL()
{
    OpenGLFunctions * f = openGLFunctions();

}
