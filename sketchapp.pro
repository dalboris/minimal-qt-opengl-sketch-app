QT += core gui widgets opengl
TEMPLATE = app

SOURCES += \
    main.cpp \
    OpenGLWidget.cpp

HEADERS  += \
    OpenGLWidget.h

DISTFILES += \
    shader.f.glsl \
    shader.v.glsl
