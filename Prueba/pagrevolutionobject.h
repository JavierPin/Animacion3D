#ifndef PAGREVOLUTIONOBJECT_H
#define PAGREVOLUTIONOBJECT_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenglfunctions_4_0_core.h>
#include <pagshaderprogram.h>
#include <pagcamera.h>


class PagRevolutionObject : protected QOpenGLFunctions_4_0_Core
{
public:

    PagRevolutionObject(std::string object, int nr, int nd);
    void revPaint(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p);
    void setDrawLines(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p);
    void setDrawPoints(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p);
    void setDrawTriangles(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p, QMatrix4x4 m2);
    void clear();
    void setOffset(bool o);

private:

    void poligonalControl();
    void revolution();
    void createIndexPoints();
    void createIndexLines();
    void createIndexTriangles();
    void prepareBuffer();
    void setVase();
    void setBattery();
    void setBasic();
    void setCono();

    int divisiones;
    int subdivisiones;
    int cortes;
    float alpha;

    bool offset;

    QMatrix4x4 modelo;

    typedef struct{

        GLfloat x,y,z;

    }vCoord;

    typedef struct{

        vCoord puntos, normales;

    }point;

    vCoord *vertices;
    vCoord *verticesR;
    vCoord *normales;
    vCoord *normalesR;
    point *puntosR;

    int *indicesP;
    int *indicesL;
    int *indicesT;

    GLuint vao;
    GLuint vbo;
    GLuint ibo;

};

#endif // PAGREVOLUTIONOBJECT_H
