#include "openglwindow.h"
#include <QPainter>
#include <QCoreApplication>

OpenGLWindow::OpenGLWindow(QWidget *parent)
    : QOpenGLWidget(parent),m_program(0)
    , m_update_pending(false)

{
    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setVersion(3,3);
    format.setSwapInterval(0);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    create();  
}

void OpenGLWindow::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLWindow::cleanup);
    initializeOpenGLFunctions();
    initialize();
}

void OpenGLWindow::resizeGL(int , int )
{
}

void OpenGLWindow::paintGL()
{

}

void OpenGLWindow::cleanup()
{
    makeCurrent();
    delete m_program;
    doneCurrent();
}

void OpenGLWindow::initialize()
{
}

QSize OpenGLWindow::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OpenGLWindow::sizeHint() const
{
    return QSize(500, 250);
}


