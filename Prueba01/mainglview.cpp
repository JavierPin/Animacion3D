#include "mainglview.h"
#include <QtDebug>
#include <QFileInfo>
#include <iostream>
#include <QTime>
#include <QCoreApplication>
#include "ipolacion.h"


using namespace std;

MainGLView::MainGLView(QWidget *parent) : QOpenGLWidget(parent)
{
    this->setFocusPolicy ( Qt::StrongFocus );
}

void MainGLView::initializeGL()
{

    initializeOpenGLFunctions();

    camera = new PagCamera();
    camera->resetCamera();

    camera->setWidth(4);
    camera->setHeight(3);

    drawMode=123;

    //obj = new PagRevolutionObject("cono",44,4);
    obj = new PagRevolutionObject("cono",4,6);
    shader1 = new PagShaderProgram("points-1");
    shader2 = new PagShaderProgram("lines-1");
    shader3 = new PagShaderProgram("phong");

    vScale=QVector3D(1,1,1);


}

void MainGLView::paintGL()
{

    m = camera->getView();
    p = camera->getProjection();
    obj->clear();

    obj->setOffset(false);
    //m.scale(1.5,0.6,1.5);
    m.scale(vScale);
    obj->setDrawTriangles(shader3,m,p,mTransformacion);
}

void MainGLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    aspect = float(w) / h;
    camera->setWidth(w);
    camera->setHeight(h);
    camera->setAspect();

}

void MainGLView::aplicaLineal(QVector3D q){
    mTransformacion.setToIdentity();
    mTransformacion.translate(q);
    update();

}

void MainGLView::aplicaEsferica(QMatrix4x4 v){
    mTransformacion=v;
    update();
}

void MainGLView::aplicaEscalado(QVector3D v){
    m.scale(v);
}

void MainGLView::keyPressEvent(QKeyEvent *event){

    QString key = event->text();

    if (key=="1"){

        drawMode=1;

    }
    if (key=="2"){

        drawMode=2;

    }
    if (key=="3"){

        drawMode=3;

    }
    if (key=="4"){

        drawMode=12;

    }
    if (key=="5"){

        drawMode=13;

    }
    if (key=="6"){

        drawMode=23;

    }
    if (key=="7"){

        drawMode=123;

    }


    if (key=="r" || key=="R"){

        camera->resetCamera();
    }

    if(key=="m" || key=="M"){

        camera->resetCamera();

    }

    if(key=="p" || key=="P"){

        camera->changePerspective();
    }

    if(key=="j" || key=="J"){

        camera->moveCameraX(-0.2f);
    }

    if(key=="l" || key=="L"){

        camera->moveCameraX(0.2f);
    }

    if(key=="k" || key=="K"){

        camera->moveCameraY(-0.2f);

    }

    if(key=="i" || key=="I"){

        camera->moveCameraY(0.2f);

    }

    if(key=="u" || key=="U"){

        camera->moveCameraZ(-0.2f);

    }

    if(key=="o" || key=="O"){

        camera->moveCameraZ(0.2f);

    }

    if(event->key()==16777235){
                camera->orbit(1,0,1);
        }

        if(event->key()==16777237){
                camera->orbit(-1,0,1);
        }

        if(event->key()==16777234){
                camera->orbit(-1,1,0);
        }

        if(event->key()==16777236){
                camera->orbit(1,1,0);

        }


    update();
}

//void MainGLView::mouseMoveEvent(QMouseEvent *event)
//{
//    int newX = event->pos().rx();
//    int newY = event->pos().ry();
//    int button = event->modifiers();

//    switch(button){

//        case 0:

//            if(oldX<newX){
//                camera->rotateX(1);
//            }
//            else if(oldX>newX){

//                camera->rotateX(-1);
//            }

//            if(oldY<newY){

//                camera->rotateY(1);
//            }
//            else if(oldY>newY){

//                camera->rotateY(-1);
//            }

//        break;

//        case 67108864:

//            if(oldX<newX){

//                camera->rotateCameraX(-0.5f);
//            }
//            else if(oldX>newX){

//                camera->rotateCameraX(0.5f);
//            }

//            if(oldY<newY){

//                camera->rotateCameraY(-0.5f);
//            }
//            else if(oldY>newY){

//                camera->rotateCameraY(0.5f);
//            }

//        break;

//    }

//    oldX=newX;
//    oldY=newY;

//    update();
//}

void MainGLView::mousePressEvent(QMouseEvent *event)
{

    if((event->button())==2){

        int newZ = event->pos().rx();

        if(newZ>camera->getWidth()/2){

            camera->rotateCameraZ(1);
        }
        else if(newZ<camera->getWidth()/2){

            camera->rotateCameraZ(-1);
        }

        oldZ=newZ;
        update();

    }
}

void MainGLView::wheelEvent(QWheelEvent *event){

    float wheel = ((event->angleDelta().y())%119)*0.2f;
    camera->zoom(wheel);
    update();

}

void MainGLView::bezier(){
    vector<QVector3D> p;
    p.push_back(QVector3D(0,0,0));
    p.push_back(QVector3D(2,5,0));
    p.push_back(QVector3D(4,-5,0));
    p.push_back(QVector3D(6,0,0));
    vector<QVector3D> *bz;
    bz=ip.ComputeBezier(p,100);

    for(int i = 0; i<100;i++){
        qDebug() << bz->at(i);
        aplicaLineal(bz->at(i));
        step(0.1);
    }


}



void MainGLView::step(float t){
    QTime tick = QTime::currentTime().addMSecs(t*100);
    while (QTime::currentTime()<tick){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}


