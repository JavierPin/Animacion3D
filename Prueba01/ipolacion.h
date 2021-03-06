#ifndef IPOLACION_H
#define IPOLACION_H
#include <vector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
using namespace std;

class iPolacion
{

public:
    iPolacion();
    QVector3D ipLineal(vector<QVector3D> p, vector<float> t, float _t);
    QVector4D ipEsferica(QVector4D q1, QVector4D q2, float lamba);
    QVector4D ipEsferica(vector<QVector4D> q, vector<float> l, float w);
    QMatrix4x4 getMatrizRotacion(QVector4D q);

};

#endif // IPOLACION_H
