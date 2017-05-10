#ifndef SWARM2DWINDOW_H
#define SWARM2DWINDOW_H
#include "openglwindow.h"
#include "swarm2d.h"
#include <QOpenGLVertexArrayObject>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
class Swarm2dWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    Swarm2dWindow(QWidget * parent = 0 );
    Swarm2dWindow(Swarm2d *model,QWidget * parent = 0 );
    void initialize() Q_DECL_OVERRIDE;
    void resizeGL(int w,int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    void setModel(Swarm2d *mod);
    bool running;
    void drawBorder();
    void drawSeed();
    void drawCore();
    void drawParts(uint key);
public slots:
//    void setAttractionScale(int value);
//    void setGeneration(int value);
    void setTpp(int value);
    void compute();
    void setHeight(QString);
    void setWidth(QString);
//    void setSeedLenght(double value);
//    void setRillRadius(double value);
//    void setStickness(int value);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
private:
    Swarm2d *model;
    QTimer *computeTimer;
    QMatrix4x4 matrix;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint c_posAttr;
    GLuint c_colAttr;
    GLuint m_radiusUniform;
    GLuint m_winUniform;
    GLuint m_matrixUniform;
    GLuint m_mv_MatrixUniform;
    GLuint c_matrixUniform;
    GLuint viewport;
    QPoint m_lastPos;
    QOpenGLVertexArrayObject vao;
    QOpenGLShaderProgram *common_programm;

    int tpp; // time per compute
    int m_xRot;
    int m_yRot;
    int m_zRot;
    int currHeight;
    int currWidth;
    int m_xCenter;
    int m_yCenter;
    float zoom;
};

#endif // SWARM2DWINDOW_H
