#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include<QtWidgets/QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLFunctions_3_3_Core>

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWidget *parent = 0);
    virtual void initialize();
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    QOpenGLShaderProgram *m_program;

public slots:
      void cleanup();
private:
    bool m_update_pending;

};
#endif // OPENGLWINDOW_H
