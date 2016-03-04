#include "pagcamera.h"
#include <QtDebug>
#include <QFileInfo>
#include <math.h>

PagCamera::PagCamera()
{
    initializeOpenGLFunctions();

    setWidth(4);
    setHeight(3);
    setAspect();

    resetCamera();

}

QMatrix4x4 PagCamera::getProjection(){

    return p;

}

QMatrix4x4 PagCamera::getView(){

    return v;
}

void PagCamera::setAspect(){

    aspect=getWidth()/getHeight();

}

void PagCamera::setWidth(int width){

    w=width;

}

int PagCamera::getWidth(){

    return w;
}

void PagCamera::setHeight(int height){

    h=height;

}

int PagCamera::getHeight(){

    return h;

}

void PagCamera::changePerspective(){

    if(cParameters.perspective=="ortho"){

        p.setToIdentity();
        p.perspective(cParameters.angle,cParameters.aspect,cParameters.znear, cParameters.zfar);
        cParameters.perspective="parallel";
    }
    else{

        p.setToIdentity();
        p.ortho(cParameters.xmin,cParameters.xmax,cParameters.ymin,cParameters.ymax,cParameters.znear,cParameters.zfar);
        cParameters.perspective="ortho";

    }

}

void PagCamera::resetCamera()
{

    p.setToIdentity();
    v.setToIdentity();

    cParameters.position=QVector3D(0.0f, 3.0f, 90.0f);
    cParameters.lookAt=QVector3D(0.0f, 3.0f, 0.0f);
    cParameters.up=QVector3D(0.0f, 1.0f, 0.0f);

    cParameters.vn=QVector3D(cParameters.position-cParameters.lookAt).normalized();
    cParameters.vu= QVector3D::crossProduct(cParameters.vn, cParameters.up);
    cParameters.vv=QVector3D::crossProduct(cParameters.vu,cParameters.vn);

    //matriz de vision
    v.lookAt(cParameters.position,cParameters.lookAt,cParameters.vv);

    //matriz de proyección
    cParameters.xmin=-8;
    cParameters.xmax=8;
    cParameters.ymin=-8;
    cParameters.ymax=8;
    cParameters.znear=0.1f;
    cParameters.zfar=100;

    cParameters.angle=(2*atan(((abs(cParameters.xmin)+abs(cParameters.xmax))/2)/(sqrt(pow(cParameters.position.x(),2)+pow(cParameters.position.y(),2)+pow(cParameters.position.z(),2)))))*(180/3.14159265358979323846);
    cParameters.aspect=aspect;

    p.perspective(cParameters.angle,cParameters.aspect,cParameters.znear, cParameters.zfar);
    cParameters.perspective="parallel";
}

//panning, izquierda y derecha sobre el propio eje Y de la cámara.
void PagCamera::rotateCameraX(float newX){

    p.rotate(newX, 0, 1, 0);
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(newX,0,1,0);
    cParameters.vn=cParameters.vn*rotation;
    cParameters.vu=cParameters.vu*rotation;
    cParameters.vv=cParameters.vv*rotation;

}

//tilt, arriba y abajo sobre el propio eje X de la cámara.
void PagCamera::rotateCameraY(float newY){

    p.rotate(newY, 1, 0, 0);
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(newY,1,0,0);
    cParameters.vn=cParameters.vn*rotation;
    cParameters.vu=cParameters.vu*rotation;
    cParameters.vv=cParameters.vv*rotation;

}

//Up, cambio del vector up, izquierda y derecha sobre el propio eje Z de la cámara.
void PagCamera::rotateCameraZ(int newZ){

    int angleR=10*newZ;

    p.rotate(-angleR, 0, 0, 1);
    QMatrix4x4 rotation;
    rotation.setToIdentity();
    rotation.rotate(-angleR,0,0,1);
    cParameters.vn=cParameters.vn*rotation;
    cParameters.vu=cParameters.vu*rotation;
    cParameters.vv=cParameters.vv*rotation;

}

//track, movimiento hacia la izquierda y la derecha de la cámara en el eje X.
void PagCamera::moveCameraX(float newX){

    p.translate(newX,0,0);

    QMatrix4x4 translation;
    translation.setToIdentity();
    translation.translate(newX,0,0);
    cParameters.vn=cParameters.vn*translation;
    cParameters.vu=cParameters.vu*translation;
    cParameters.vv=cParameters.vv*translation;

}

//pedestal, movimiento hacia arriba y hacia abajo de la cámara en el eje Y.
void PagCamera::moveCameraY(float newY){

    p.translate(0,newY,0);

    QMatrix4x4 translation;
    translation.setToIdentity();
    translation.translate(0,newY,0);
    cParameters.vn=cParameters.vn*translation;
    cParameters.vu=cParameters.vu*translation;
    cParameters.vv=cParameters.vv*translation;

}

//dolly in-out, movimiento hacia delante y hacia atrás de la cámara en el eje Z.
void PagCamera::moveCameraZ(float newZ){

    p.translate(0,0,newZ);

    QMatrix4x4 translation;
    translation.setToIdentity();
    translation.translate(0,0,newZ);
    cParameters.vn=cParameters.vn*translation;
    cParameters.vu=cParameters.vu*translation;
    cParameters.vv=cParameters.vv*translation;

}

//orbit horizontal, rotación hacia la izquierda y la derecha de la cámara sobre un punto en el eje X.
void PagCamera::orbit(int angle, int x, int y){

    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity();
    rotationMatrix.translate(cParameters.lookAt);
    if(x==1){
        rotationMatrix.rotate(angle*3, cParameters.vv);
    }
    else if(y==1){
        rotationMatrix.rotate(angle*3, cParameters.vu);
    }
    rotationMatrix.translate(-cParameters.lookAt);

    cParameters.position=rotationMatrix*cParameters.position;

    rotationMatrix.setToIdentity();

    if(x==1){
        rotationMatrix.rotate(angle*3, cParameters.vv);
    }
    else if(y==1){
        rotationMatrix.rotate(angle*3, cParameters.vu);
    }


    cParameters.vu=rotationMatrix*cParameters.vu;
    cParameters.vv=rotationMatrix*cParameters.vv;
    cParameters.vn=rotationMatrix*cParameters.vn;

    v.setToIdentity();
    v.lookAt(cParameters.position,cParameters.lookAt,cParameters.vv);



}

//zoom, movimiento de zoom hacia adentro.
void PagCamera::zoom(float newW){

    if((cParameters.xmin<-1.3) || newW<0){

        cParameters.xmin+=newW;
        cParameters.xmax-=newW;
        cParameters.ymin+=newW;
        cParameters.ymax-=newW;
        cParameters.angle=(2*atan(((abs(cParameters.xmin)+abs(cParameters.xmax))/2)/(sqrt(pow(cParameters.position.x(),2)+pow(cParameters.position.y(),2)+pow(cParameters.position.z(),2)))))*(180/3.14159265358979323846);
        p.setToIdentity();

        if(cParameters.perspective=="ortho"){

            p.ortho(cParameters.xmin,cParameters.xmax,cParameters.ymin,cParameters.ymax,cParameters.znear,cParameters.zfar);

        }

        else{

            p.perspective(cParameters.angle, cParameters.aspect, cParameters.znear, cParameters.zfar);

        }

    }

}
