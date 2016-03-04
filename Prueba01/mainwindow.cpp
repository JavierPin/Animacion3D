#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipolacion.h"
#include <vector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <iostream>
#include <QString>
#include <QTime>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::interpolacion_lineal(){
    cout<<"He pulsado un boton! El de interpolacion lineal"<<endl;
    vector<QVector3D> vPuntos(3);
    vector<float> vT(3);
    QStringList punto;
    vector<double> vCoor;

    //Obtener la coordenadas del punto 1
    vCoor.clear();
    QString cadena = ui->tb_P1->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vCoor.push_back(p.toFloat());
    }
    vPuntos[0]=QVector3D(vCoor[0],vCoor[1],vCoor[2]);

        //Obtener la coordenadas del punto 2
    vCoor.clear();
    cadena = ui->tb_P2->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vCoor.push_back(p.toFloat());
    }
    vPuntos[1]=QVector3D(vCoor[0],vCoor[1],vCoor[2]);

        //Obtener la coordenadas del punto 3
    vCoor.clear();
    cadena = ui->tb_P3->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vCoor.push_back(p.toFloat());
    }
    vPuntos[2]=QVector3D(vCoor[0],vCoor[1],vCoor[2]);

    //Obtener los puntos t
    vT.clear();
    cadena = ui->tb_t->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vT.push_back(p.toFloat());
    }

    iPolacion *ip = new iPolacion();

    double t = ui->sb_t->value();
    cout << t << endl;

    QVector3D test = ip->ipLineal(vPuntos,vT,(float)t);

    cout << test.x() << "," << test.y() << "," << test.z() << endl;

    ui->openGLWidget->aplicaLineal(test);

}
void MainWindow::interpolacion_esferica(){
    cout<<"He pulsado un boton! El de interpolacion lineal"<<endl;
    vector<QVector4D> vPuntos(3);
    vector<float> vT(3);
    QStringList punto;
    vector<double> vCoor;

    //Obtener la coordenadas del punto 1
    vCoor.clear();
    QString cadena = ui->tb_P1_2->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vCoor.push_back(p.toFloat());
    }
    vPuntos[0]=QVector4D(vCoor[0],vCoor[1],vCoor[2],vCoor[3]);

        //Obtener la coordenadas del punto 2
    vCoor.clear();
    cadena = ui->tb_P2_2->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vCoor.push_back(p.toFloat());
    }
    vPuntos[1]=QVector4D(vCoor[0],vCoor[1],vCoor[2],vCoor[3]);


    //Obtener los puntos t
    vT.clear();
    cadena = ui->tb_t_2->text();
    punto = cadena.split(",");
    foreach(QString p,punto){
        vT.push_back(p.toFloat());
    }

    iPolacion *ip = new iPolacion();

    double t = ui->sb_t->value();
    cout << t << endl;

    QVector4D test = ip->ipEsferica(vPuntos,vT,(float)t);

    cout << test.x() << "," << test.y() << "," << test.z() <<","<<test.w() << endl;

    QMatrix4x4 mt = ip->getMatrizRotacion(test);

    ui->openGLWidget->aplicaEsferica(mt);

}

void MainWindow::on_bt_lineal_clicked()
{
    MainWindow::interpolacion_lineal();
    //playLineal();

}


void MainWindow::on_bt_esferica_clicked()
{

    interpolacion_esferica();
}

void MainWindow::playLineal(){
    do{
        for(float i=(float)ui->sb_t->value();i<=1.1f;i=i+0.05f){
            ui->sb_t->setValue((double)i);

            //animacion movimiento
            if((float)ui->sb_t->value()==0.0f){
                for(int j=0;j<=5;j++){
                    ui->openGLWidget->vScale=(QVector3D(1.3,0.7,1.3));
                    interpolacion_lineal();
                    //interpolacion_esferica(); //en la proxima version
                    step(0.5);
                }
            }else{
                 ui->openGLWidget->vScale=(QVector3D(1,1,1));
                 interpolacion_lineal();
                 //interpolacion_esferica(); // en la proxima version
            }
            step(0.5);

        }
        ui->sb_t->setValue(0.0);
    }while(!stop);
}

void MainWindow::playEsferica(){
    do{
        for(float i=(float)ui->sb_t->value();i<=1.1f;i=i+0.1f){
            ui->sb_t->setValue((double)i);
               interpolacion_esferica();
               step(1);
            }
        ui->sb_t->setValue(0.0);
    }while(!stop);
}

void MainWindow::step(float t){
    QTime tick = QTime::currentTime().addMSecs(t*100);
    while (QTime::currentTime()<tick){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

void MainWindow::on_bt_play_clicked()
{
    stop=false;
    MainWindow::interpolacion_lineal();
    playLineal();
}

void MainWindow::on_bt_stop_clicked()
{
    stop=true;
}

void MainWindow::on_bt_play_2_clicked()
{
    stop=false;
    MainWindow::interpolacion_esferica();
    playEsferica();
}
void MainWindow::on_bt_stop_2_clicked()
{
    stop=true;
}

void MainWindow::on_sb_t_valueChanged(double arg1)
{
    ui->hs_t->setValue(arg1*100);
}



void MainWindow::on_hs_t_sliderMoved(int position)
{
    float valor;
    valor=(float) position;
    valor = valor /100;
    ui->sb_t->setValue((double)valor);
}
