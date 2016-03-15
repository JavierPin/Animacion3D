#ifndef MAINGLVIEW_H
#define MAINGLVIEW_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenglfunctions_4_0_core.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "pagshaderprogram.h"
#include "pagcamera.h"
#include "pagrevolutionobject.h"
#include "ipolacion.h"

class MainGLView : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
public:
    MainGLView(QWidget* parent);

    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    //void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void aplicaLineal (QVector3D q);
    void aplicaEsferica(QMatrix4x4 v);
    void aplicaEscalado(QVector3D v);
    void bezier();
    void step(float t);

    QVector3D vScale;
    QMatrix4x4 m;

signals:

public slots:

private:
    float aspect;

    int oldX;
    int oldY;
    int oldZ;

    int drawMode;


    QMatrix4x4 p;
    QMatrix4x4 mTransformacion;

    PagCamera* camera;
    PagShaderProgram* shader1;
    PagShaderProgram* shader2;
    PagShaderProgram* shader3;
    PagRevolutionObject* obj;
    PagRevolutionObject* obj2;

    void setShader(QString sname);

    iPolacion ip;

};

#endif // MAINGLVIEW_H
