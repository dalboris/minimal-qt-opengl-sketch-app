#include "OpenGLWidget.h"

#include <QMouseEvent>

#include <cmath>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // Set view matrix
    viewMatrix_.setToIdentity();
}

OpenGLWidget::~OpenGLWidget()
{
    // Make OpenGL context current and call cleanupGL()
    makeCurrent();
    cleanupGL();
    doneCurrent();
}

void OpenGLWidget::mousePressEvent(QMouseEvent * /*event*/)
{
    // Clear input samples and redraw
    inputSamples_.clear();
    repaint();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    // Add input sample and redraw
    inputSamples_.push_back(event->pos());
    repaint();
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

    // Initialize shader program
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.v.glsl");
    shaderProgram_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.f.glsl");
    shaderProgram_.link();

    // Get shader locations
    shaderProgram_.bind();
    vertexLoc_     = shaderProgram_.attributeLocation("vertex");
    projMatrixLoc_ = shaderProgram_.uniformLocation("projMatrix");
    viewMatrixLoc_ = shaderProgram_.uniformLocation("viewMatrix");
    shaderProgram_.release();

    // Create VBO
    vbo_.create();

    // Create VAO
    vao_.create();
    GLsizei  stride  = sizeof(GLVertex);
    GLvoid * pointer = reinterpret_cast<void*>(offsetof(GLVertex, x));
    vao_.bind();
    vbo_.bind();
    f->glEnableVertexAttribArray(vertexLoc_);
    f->glVertexAttribPointer(
                vertexLoc_, // index of the generic vertex attribute
                2,          // number of components   (x and y components)
                GL_FLOAT,   // type of each component
                GL_FALSE,   // should it be normalized
                stride,     // byte offset between consecutive vertex attributes
                pointer);   // byte offset between the first attribute and the pointer given to allocate()
    vbo_.release();
    vao_.release();

    // Set clear color
    f->glClearColor(1, 1, 1, 1);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    // Set projection matrix
    const float left   = 0.0f;
    const float right  = w;
    const float bottom = h;
    const float top    = 0.0f;
    const float near   = -1.0f;
    const float far    = 1.0f;
    projMatrix_.setToIdentity();
    projMatrix_.ortho(left, right, bottom, top, near, far);
}

void OpenGLWidget::paintGL()
{
    OpenGLFunctions * f = openGLFunctions();

    // Update VBO
    // Note: for simplicity, we perform this at each paintGL. In an actual app,
    // it would be much better to do this only once after each mouse event.
    computeGLVertices_();
    vbo_.bind();
    vbo_.allocate(glVertices_.data(), glVertices_.size() * sizeof(GLVertex));
    vbo_.release();

    // Clear color and depth buffer
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind shader program
    shaderProgram_.bind();

    // Set uniform values
    shaderProgram_.setUniformValue(projMatrixLoc_, projMatrix_);
    shaderProgram_.setUniformValue(viewMatrixLoc_, viewMatrix_);

    // Draw triangles
    vao_.bind();
    f->glDrawArrays(GL_TRIANGLE_STRIP,   // mode
                    0,                   // first index
                    glVertices_.size()); // number of indices
    vao_.release();

    // Release shader program
    shaderProgram_.release();
}

void OpenGLWidget::cleanupGL()
{
    // Destroy VAO
    vao_.destroy();

    // Destroy VBO
    vbo_.destroy();
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

void OpenGLWidget::computeGLVertices_()
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
