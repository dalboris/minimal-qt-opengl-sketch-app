#include "OpenGLWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Initialize application
    QApplication a(argc, argv);

    // Set OpenGL 3.2, core profile
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(16);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);

    // Create and show OpenGL widget
    OpenGLWidget w;
    w.show();

    // Start event loop
    return a.exec();
}
