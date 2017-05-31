#include "swarm2dwindow.h"
#include <QTimer>
#include <QOpenGLBuffer>
#include <QtConcurrent/QtConcurrent>
#include <iostream>

Swarm2dWindow::Swarm2dWindow(QWidget *parent):OpenGLWindow(parent)
{

}


//static void NormalizeAngle(int &angle)
//{
//    while (angle < 0)
//        angle += 360*16;
//    while (angle > 360*16)
//        angle -= 360*16;
//}

Swarm2dWindow::Swarm2dWindow(Swarm2d *model, QWidget *parent):
    OpenGLWindow(parent),running(false),tpp(50),m_xRot(0),m_yRot(0),m_zRot(0),
    currHeight(500),currWidth(500),m_xCenter(0),m_yCenter(0),zoom(0.5)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    timer->setInterval(16);
    timer->start(0);

    computeTimer = new QTimer(this);
    connect(computeTimer, SIGNAL(timeout()), this, SLOT(compute()));
    computeTimer->setInterval(tpp);
    computeTimer->start(0);

    this->model=model;
}

void Swarm2dWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
//    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"common.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"swarm2d.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"swarm2d.frag");
//    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"common.frag");

    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
//    m_posAttr = m_program->attributeLocation("vertex");
    m_colAttr = m_program->attributeLocation("colAttr");
//    m_colAttr = m_program->attributeLocation("color");
    m_matrixUniform = m_program->uniformLocation("proj_Matrix");
    m_mv_MatrixUniform = m_program->uniformLocation("mv_Matrix");
    m_radiusUniform = m_program->uniformLocation("radius");
    m_winUniform = m_program->uniformLocation("win");
    viewport = m_program->uniformLocation("viewport");

    common_programm = new QOpenGLShaderProgram(this);
    common_programm->addShaderFromSourceFile(QOpenGLShader::Vertex,"common.vert");
    common_programm->addShaderFromSourceFile(QOpenGLShader::Fragment,"common.frag");
    common_programm->link();
    c_posAttr = common_programm->attributeLocation("vertex");
    c_colAttr = common_programm->attributeLocation("color");
    c_matrixUniform = common_programm->uniformLocation("matrix");
    glClearColor(0.4,0.4,0.4,1.0);
    vao.create();
}

void Swarm2dWindow::resizeGL(int w, int h)
{
    matrix.setToIdentity();
    matrix.ortho(-w/2,w/2,-h/2,h/2,1000,-1000);
}

void Swarm2dWindow::drawBorder()
{
    vector<float> vert;
    vert.push_back(-currWidth/2);vert.push_back(-currHeight/2);vert.push_back(0);
    vert.push_back(-currWidth/2);vert.push_back(currHeight/2);vert.push_back(0);
    vert.push_back(currWidth/2);vert.push_back(currHeight/2);vert.push_back(0);
    vert.push_back(currWidth/2);vert.push_back(-currHeight/2);vert.push_back(0);
    auto vertBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertBuffer->create();
    vertBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertBuffer->bind();
    vertBuffer->allocate(&vert[0],vert.size()*sizeof(GLfloat));
    common_programm->enableAttributeArray(c_posAttr);
    common_programm->setAttributeBuffer(c_posAttr,GL_FLOAT,0,3);

    vector<float> color(vert.size()*2,1.0);
    auto vertBufferColor = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertBufferColor->create();
    vertBufferColor->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertBufferColor->bind();
    vertBufferColor->allocate(&color[0],color.size()*sizeof(GLfloat));
    common_programm->enableAttributeArray(c_colAttr);
    common_programm->setAttributeBuffer(c_colAttr,GL_FLOAT,0,4);
    glDrawArrays(GL_LINE_LOOP, 0,vert.size()/3);
    common_programm->disableAttributeArray(c_posAttr);
    common_programm->disableAttributeArray(c_colAttr);

    vertBufferColor->release();
    vertBufferColor->destroy();
    vertBuffer->release();
    vertBuffer->destroy();
}

void Swarm2dWindow::drawSeed()
{

}

void Swarm2dWindow::drawCore()
{

}

void Swarm2dWindow::drawParts(uint key)
{
    vector<float> vertices = model->getModel(key);
    Swarm* s = model->getSwarm(key);
    auto vertBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertBuffer->create();
    vertBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertBuffer->bind();
    vertBuffer->allocate(&vertices[0],vertices.size()*sizeof(GLfloat));
    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr,GL_FLOAT,0,2);
    float pointSize = s->size;
    m_program->setUniformValue(m_radiusUniform,pointSize);

    vector<float> color;
    for (uint i=0;i<vertices.size()/2;++i){
        color.push_back(s->color.redF());
        color.push_back(s->color.greenF());
        color.push_back(s->color.blueF());
        color.push_back(1.0f);
    }
    auto vertBufferColor = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertBufferColor->create();
    vertBufferColor->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertBufferColor->bind();
    vertBufferColor->allocate(&color[0],color.size()*sizeof(GLfloat));
    m_program->enableAttributeArray(m_colAttr);
    m_program->setAttributeBuffer(m_colAttr,GL_FLOAT,0,4);

    glEnable(GL_DEPTH_TEST);
//    glLineWidth(5);
    glDrawArrays(GL_LINES, 0,vertices.size()/2);
//    glLineWidth(1);
    glDisable(GL_DEPTH_TEST);
    m_program->disableAttributeArray(m_posAttr);
    vertBufferColor->release();
    vertBufferColor->destroy();
    vertBuffer->release();
    vertBuffer->destroy();
}

void Swarm2dWindow::paintGL()
{
    glViewport(0, 0, width(), height());
    glLineWidth(3);
    QOpenGLVertexArrayObject::Binder binderVao(&vao);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 m_world;
    m_world.setToIdentity();
    m_world.translate(m_xCenter,m_yCenter);
    m_world.scale(zoom);
//    m_world.rotate(180-m_xRot/16.0f, 1, 0, 0);
//    m_world.rotate(m_yRot/16.0f, 0, 1, 0);
//    m_world.rotate(m_zRot/16.0f, 0, 0, 1);
    m_program->bind();
    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_mv_MatrixUniform, m_world);
    m_program->setUniformValue(m_winUniform,width(),height());

    for (uint i=0; i<model->swarmCount();i++){
        drawParts(i);
    }
    m_program->release();

    common_programm->bind();
    common_programm->setUniformValue(c_matrixUniform,matrix*m_world);
    drawBorder();
    common_programm->release();
}

QSize Swarm2dWindow::minimumSizeHint() const
{
    return QSize(500, 500);
}

QSize Swarm2dWindow::sizeHint() const
{
    return QSize(500, 500);
}


void Swarm2dWindow::setTpp(int value)
{
    tpp = 100 - value;
    computeTimer->setInterval(tpp);
}

void Swarm2dWindow::compute()
{
    if(running)
        model->iterate();
}

void Swarm2dWindow::setHeight(QString value)
{
    currHeight = value.toInt();
    model->setSize(currHeight,currWidth);
}

void Swarm2dWindow::setWidth(QString value)
{
    currWidth = value.toInt();
    model->setSize(currHeight,currWidth);
}

void Swarm2dWindow::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void Swarm2dWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_xCenter += dx;
        m_yCenter -= dy;
    } else if (event->buttons() & Qt::RightButton) {
//        m_xRot += 8*dy;
//        NormalizeAngle(m_xRot);
//        m_zRot += 8*dx;
//        NormalizeAngle(m_zRot);
    }
    m_lastPos = event->pos();
}

void Swarm2dWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        zoom /= numPixels.y()<0 ? 1.2 : 1/1.2;
    } else if (!numDegrees.isNull()) {
        zoom /= numDegrees.y()<0 ? 1.2 : 1/1.2;
    }

    event->accept();
}
