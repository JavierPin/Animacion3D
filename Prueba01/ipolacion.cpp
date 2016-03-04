#include "ipolacion.h"
#include "math.h"


iPolacion::iPolacion()
{
}

QVector3D iPolacion::ipLineal(vector<QVector3D> p, vector<float> t,float _t){
    if (_t<=t[0]) return p[0];//Posicion inicial
    if (_t>=(t[t.size()-1])) return p[t.size()-1];//Posicion final
    float x,y,z;
    for (int i=1; i<(t.size());i++){ // recorro el vector de tiempos...
        if(_t<=t[i]){ //y encuentro que punto corresponde con el tiempo. Esto va a doler con la esferica...
            x = p[i-1].x()+(_t-t[i-1])*((p[i].x()-p[i-1].x())/(t[i]-t[i-1]));//Interpolo la posicion de x
            y = p[i-1].y()+(_t-t[i-1])*((p[i].y()-p[i-1].y())/(t[i]-t[i-1]));//Interpolo la posicion de y
            z = p[i-1].z()+(_t-t[i-1])*((p[i].z()-p[i-1].z())/(t[i]-t[i-1]));//Interpolo la posicion de z
            QVector3D *punto = new QVector3D(x,y,z); // me fabrico un punto 3D
            return *punto;
        }
    }
}

QVector4D iPolacion::ipEsferica(QVector4D q1, QVector4D q2, float lam){
    // Caso facil primero.
        if (lam <= 0.0f)
            return q1;
        else if (lam >= 1.0f)
            return q2;

        // Determinamos el angulo
        QVector4D q2b;
        float dot;
        dot = q1.x() * q2.x() + q1.y() * q2.y() + q1.z() * q2.z() + q1.w() * q2.w();
        if (dot >= 0.0f) {
            q2b = q2;
        } else {
            q2b = -q2;
            dot = -dot;
        }

        // Obtenemos los factores de interpolacion
        float factor1 = 1.0f - lam;
        float factor2 = lam;
        if ((1.0f - dot) > 0.0000001) {
            float angle = float(acos(dot));
            float sinOfAngle = float(sin(angle));
            if (sinOfAngle > 0.0000001) {
                factor1 = float(sin((1.0f - lam) * angle)) / sinOfAngle;
                factor2 = float(sin(lam * angle)) / sinOfAngle;

            }
        }

        // Construimos el nuevo quaternion
        return q1 * factor1 + q2b * factor2;
}
QVector4D iPolacion::ipEsferica(vector<QVector4D> q, vector<float> l, float w){
    //prfff que pereza... por cierto: l de lambdas
    //Busco en entre que dos puntos tengo que interpolar, pero primero a ver si no estoy en el punto inicial o final
    if (w<=l[0]) return q[0];//Posicion inicial
    if (w>=(l[l.size()-1])) return q[l.size()-1];//Posicion final

    for (int i=1; i<(l.size());i++){ // recorro el vector de tiempos...
        if(w<=l[i]){ //y encuentro que punto corresponde con el tiempo. Esto va a doler con la esferica...
            return ipEsferica(q[i-1],q[i],w);
        }
    }
}

QMatrix4x4 iPolacion::getMatrizRotacion(QVector4D q){


    QMatrix4x4 m = QMatrix4x4((1-(2*((q.y()*q.y())+(q.z()*q.z())))),
                              ((2*q.x()*q.y())-(2*q.w()*q.z())),
                              ((2*q.w()*q.y())+(2*q.x()*q.z())),
                              0
                              ,
                              ((2*q.x()*q.y())+(2*q.w()*q.z())),
                              (1-(2*((q.x()*q.x())+(q.z()*q.z())))),
                              ((-2*q.w()*q.x())+(2*q.y()*q.z())),
                              0
                              ,
                              ((-2*q.w()*q.y())+(2*q.x()*q.z())),
                              ((2*q.w()*q.x())+(2*q.y()*q.z())),
                              (1-(2*((q.x()*q.x())+(q.y()*q.y())))),
                              0
                              ,
                              0,0,0,1);
    return m;
}
