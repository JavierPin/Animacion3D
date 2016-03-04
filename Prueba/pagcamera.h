#ifndef PAGCAMERA_H
#define PAGCAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenglfunctions_4_0_core.h>

class PagCamera : protected QOpenGLFunctions_4_0_Core
{
public:

    PagCamera();

    void resetCamera();
    void changePerspective();
    void setAspect();
    void setWidth(int width);
    int getWidth();
    void setHeight(int h);
    int getHeight();

    void rotateCameraX(float newX);
    void rotateCameraY(float newY);
    void rotateCameraZ(int newZ);

    void moveCameraX(float newX);
    void moveCameraY(float newY);
    void moveCameraZ(float newZ);

    void orbit(int angle, int x, int y);
    void rotateY(int newY);

    void zoom(float newW);

    QMatrix4x4 getProjection();
    QMatrix4x4 getView();

    struct cameraParameters{

        QVector3D vn;
        QVector3D vu;
        QVector3D vv;

        QVector3D position;
        QVector3D lookAt;
        QVector3D up;

        float angle;
        float aspect;

        float xmin;
        float xmax;
        float ymin;
        float ymax;
        float znear;
        float zfar;

        QString perspective;

    }cParameters;

private:

    float aspect;
    int w;
    int h;
    QString perspective;
    QMatrix4x4 p;
    QMatrix4x4 v;
    QMatrix4x4 view;




};

#endif // PAGCAMERA_H
