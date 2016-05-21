#include "OpenGLWidget.h"

#include <QMouseEvent>

#include <cmath>

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

void OpenGLWidget::mousePressEvent(QMouseEvent * /*event*/)
{
    inputSamples_.clear();
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    inputSamples_.push_back(event->pos());
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    // Nothing to do
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
    computeGLSamples_();

    OpenGLFunctions * f = openGLFunctions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void OpenGLWidget::cleanupGL()
{
    OpenGLFunctions * f = openGLFunctions();

}

QPointF OpenGLWidget::computeNormal_(const QPoint & p, const QPoint & q)
{
    // Get difference
    QPointF d = q-p;

    // Normalize difference to get tangent
    const double length = std::sqrt(d.x()*d.x() + d.y() * d.y());
    if (length > 1e-6)
        d /= length;
    else
        d = QPointF(1.0, 0.0);

    // Return vector orthogonal to tangent
    return QPointF(-d.y(), d.x());
}

void OpenGLWidget::computeGLSamples_()
{
    glVertices_.clear();

    const float width = 10.0;
    const size_t n = inputSamples_.size();

    if (n < 2)
    {
        // nothing to do
    }
    else
    {
        for (unsigned int i=0; i<n; ++i)
        {
            // Compute normal for sample
            QPointF normal;
            if (i==0)
                normal = computeNormal_(inputSamples_[0], inputSamples_[1]);
            else if (i == n-1)
                normal = computeNormal_(inputSamples_[n-2], inputSamples_[n-1]);
            else
                normal = computeNormal_(inputSamples_[i-1], inputSamples_[i+1]);

            // Compute left and right GL vertices from centerline + normal + width
            QPointF leftPos  = inputSamples_[i] + width * normal;
            QPointF rightPos = inputSamples_[i] - width * normal;
            GLVertex leftVertex(leftPos.x(), leftPos.y());
            GLVertex rightVertex(rightPos.x(), rightPos.y());

            // Add vertices to list of vertices
            glVertices_.push_back(leftVertex);
            glVertices_.push_back(rightVertex);
        }
    }
}
