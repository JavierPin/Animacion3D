#include "pagrevolutionobject.h"
#include "pagshaderprogram.h"
#include <QFileInfo>
#include <QVector3D>
#include <QMatrix3x3>
#include <pagcamera.h>
#include "math.h"

PagRevolutionObject::PagRevolutionObject(std::string object, int nr, int nd)
{

    initializeOpenGLFunctions();

    cortes=nr;
    alpha=(360.0/cortes)*(atan(1.0f)*4)/180.0f;
    subdivisiones=nd;
    modelo.setToIdentity(); // inicializo el modelo

    if(subdivisiones>6||  subdivisiones<0 || cortes<2){

        qDebug()<<"Error al crear el objeto: Numero de cortes o\n"
                  "numero de divisiones demasiado elevado: Subdivisiones ="<<subdivisiones<<", Cortes ="<<cortes;

    }
    else{

        if(object=="vase"){

            setVase();
        }
        else if(object=="battery"){

            setBattery();

        }else if (object=="cono"){
            setCono();
        }
        else{

            setBasic();
        }

        poligonalControl();

    }

}

void PagRevolutionObject::poligonalControl(){


        for (int i=0; i<subdivisiones; i++){

            vCoord *newDivision = new vCoord [(divisiones*2)-1];

            for (int i=0; i<divisiones;i++){

                newDivision[i*2]=vertices[i];

            }

            for (int i=0; i<divisiones-2;i++){

                newDivision[(i*2)+1].x=(newDivision[(i*2)].x+newDivision[(i+1)*2].x)/2;
                newDivision[(i*2)+1].y=(newDivision[(i*2)].y+newDivision[(i+1)*2].y)/2;
                newDivision[(i*2)+1].z=0.0;

                newDivision[(i+1)*2].x=((3*newDivision[(i+1)*2].x)/(4.0))+((newDivision[i*2].x)/(8.0))+((newDivision[(i+2)*2].x)/(8.0));
                newDivision[(i+1)*2].y=((3*newDivision[(i+1)*2].y)/(4.0))+((newDivision[i*2].y)/(8.0))+((newDivision[(i+2)*2].y)/(8.0));
                newDivision[(i+1)*2].z=0.0;

            }

            newDivision[(divisiones*2)-3].x=(newDivision[(divisiones*2)-4].x+newDivision[(divisiones*2)-2].x)/2;
            newDivision[(divisiones*2)-3].y=(newDivision[(divisiones*2)-4].y+newDivision[(divisiones*2)-2].y)/2;
            newDivision[(divisiones*2)-3].z=0.0;

            divisiones=(divisiones*2)-1;
            vertices=new vCoord[divisiones];
            vertices=newDivision;

        }

        vCoord *normalesS=new vCoord[divisiones-1];

        for (int i=0; i<divisiones-1;i++){

            float module=sqrt(pow(vertices[i+1].y-vertices[i].y,2.0f)+pow(vertices[i+1].x-vertices[i].x,2.0f));

            normalesS[i].x=(vertices[i+1].y-vertices[i].y)/module;
            normalesS[i].y=-((vertices[i+1].x-vertices[i].x)/module);
            normalesS[i].z=0;

        }

        normales= new vCoord[divisiones];
        normales[0]=normalesS[0];

        for (int i=1; i<=divisiones-2; i++){

            normales[i].x=(normalesS[i-1].x+normalesS[i].x)/2.0f;
            normales[i].y=(normalesS[i-1].y+normalesS[i].y)/2.0f;
            normales[i].z=0;

        }
        normales[divisiones-1]=normalesS[divisiones-2];

        revolution();

}

void PagRevolutionObject::revolution(){

    verticesR = new vCoord[divisiones*cortes];
    normalesR = new vCoord[divisiones*cortes];
    puntosR = new point[divisiones*cortes*2];

    for (int i=0; i<cortes; i++){

        for (int j=0; j<divisiones; j++){

            int index=divisiones*i+j;

            verticesR[index].x=vertices[j].x*(cos(alpha*i));
            verticesR[index].y=vertices[j].y;
            verticesR[index].z=vertices[j].x*(sin(alpha*i));

            puntosR[index].puntos.x=vertices[j].x*(cos(alpha*i));
            puntosR[index].puntos.y=vertices[j].y;
            puntosR[index].puntos.z=vertices[j].x*(sin(alpha*i));
            //qDebug()<<puntosR[index].puntos.x<<", "<<puntosR[index].puntos.y<<", "<<puntosR[index].puntos.z;

            puntosR[index].normales.x=normales[j].x*(cos(alpha*i));
            puntosR[index].normales.y=normales[j].y;
            puntosR[index].normales.z=normales[j].x*(sin(alpha*i));
            //qDebug()<<puntosR[index].normales.x<<", "<<puntosR[index].normales.y<<", "<<puntosR[index].normales.z;

        }

    }

    createIndexPoints();
    createIndexLines();
    createIndexTriangles();

    prepareBuffer();

}

void PagRevolutionObject::createIndexPoints(){

    indicesP = new int[divisiones*cortes];

    for (int i=0; i<divisiones*cortes;i++){

        indicesP[i]=i;

    }

}

void PagRevolutionObject::createIndexLines(){

    int d=0;

    indicesL = new int[(divisiones*cortes)+cortes-1];

    for (int i=0; i<(divisiones*cortes)+cortes-1; i++){

        if((i)%((divisiones)+((divisiones)*d)+d)==0 && i!=0){

            indicesL[i]=0xFFFF;
            d++;

        }
        else{

            indicesL[i]=i-d;

        }

    }

}

void PagRevolutionObject::createIndexTriangles(){

    GLuint contador = 0;

    indicesT = new int[((divisiones*2)+1)*cortes];

    for (int i=0; i<cortes;i++){

        for (int j=0; j<divisiones; j++){

            indicesT[contador]=(i*divisiones)+j;
            contador++;
            indicesT[contador]=(((i+1)%cortes)*divisiones)+j;
            contador++;

        }

    indicesT[contador] = 0xFFFF;
    contador++;

    }
}

void PagRevolutionObject::setDrawPoints(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p){

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, divisiones*cortes*sizeof(int), indicesP, GL_STATIC_DRAW);
    glBindVertexArray(vao);

    shader->use();
    shader->setUniform("mvpMatrix", p*m*modelo);
    shader->setUniform("pointSize", 12.0);
    shader->setUniform("vColor", 0.0f, 0.0f, 1.0f);

    if(offset){

        glEnable(GLenum(GL_POLYGON_OFFSET_FILL));
        glPolygonOffset(2.5, 0);
    }
    else{

        glDisable(GLenum(GL_POLYGON_OFFSET_FILL));

    }

    glDrawElements(GL_POINTS, divisiones*cortes, GL_UNSIGNED_INT, NULL);

}

void PagRevolutionObject::setDrawLines(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p){

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((divisiones*cortes)+cortes-1)*sizeof(int), indicesL, GL_STATIC_DRAW);
    glBindVertexArray(vao);

    shader->use();
    shader->setUniform("mvpMatrix", p*m);
    shader->setUniform("vColor", 0.0f, 1.0f, 0.0f);

    if(offset){

        glEnable(GLenum(GL_POLYGON_OFFSET_FILL));
        glPolygonOffset(2, 0);
    }
    else{

        glDisable(GLenum(GL_POLYGON_OFFSET_FILL));

    }

    glDrawElements(GL_LINE_STRIP, (divisiones*cortes)+cortes-1, GL_UNSIGNED_INT, NULL);

}

void PagRevolutionObject::setDrawTriangles(PagShaderProgram *shader, QMatrix4x4 m, QMatrix4x4 p,QMatrix4x4 m2){

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((divisiones*2)+1)*cortes*sizeof(int), indicesT, GL_STATIC_DRAW);
    glBindVertexArray(vao);
    shader->use();
    //m.scale(0.3,1,1);
    shader->setUniform("mvpMatrix", p*m*m2*modelo); // p matriz proyeccion(camara), m matriz modelado*vision
    shader->setUniform("modelView", m*m2*modelo);

    shader->setUniform("Ia", 0.1f, 0.045f, 0.045f);
    shader->setUniform("Ka", 1.0f, 1.0f, 1.0f);

    shader->setUniform("Id", 1.0f, 1.0f, 1.0f);
    shader->setUniform("Kd", 1.0f, 0.5f, 0.0f);//er color

    shader->setUniform("Is", 0.8f, 0.8f, 0.8f);
    shader->setUniform("Ks", 1.0f, 1.0f, 1.0f);
    shader->setUniform("LightPosition", 2.0f, 1.0f, -1.0f);

    shader->setUniform("Shininess", 20.0f);


    glDrawElements(GL_TRIANGLE_STRIP, ((divisiones*2)+1)*cortes, GL_UNSIGNED_INT, NULL);
}

void PagRevolutionObject::prepareBuffer(){

    glPrimitiveRestartIndex(0xFFFF);
    glEnable(GL_PRIMITIVE_RESTART);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, sizeof(vCoord)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(point),((GLubyte *)NULL+(0)));
    glVertexAttribPointer(1, sizeof(vCoord)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(point),((GLubyte *)NULL + (sizeof(vCoord))));

    glBufferData(GL_ARRAY_BUFFER, divisiones*cortes*6*sizeof(GLfloat), puntosR, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

}

void PagRevolutionObject::clear(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void PagRevolutionObject::setOffset(bool o){

    offset = o;

}

void PagRevolutionObject::setVase(){

    divisiones=9;

    vertices = new vCoord [divisiones];

    vertices[8].x=0.99f;
    vertices[8].y=5.2f;
    vertices[8].z=0.0f;

    vertices[7].x=1.0f;
    vertices[7].y=5.2f;
    vertices[7].z=0.0f;

    vertices[6].x=1.0f;
    vertices[6].y=4.0f;
    vertices[6].z=0.0f;

    vertices[5].x=1.02f;
    vertices[5].y=3.9f;
    vertices[5].z=0.0f;

    vertices[4].x=1.7f;
    vertices[4].y=3.0f;
    vertices[4].z=0.0f;

    vertices[3].x=2.2f;
    vertices[3].y=0.0f;
    vertices[3].z=0.0f;

    vertices[2].x=2.0f;
    vertices[2].y=-2.5f;
    vertices[2].z=0.0f;

    vertices[1].x=1.0f;
    vertices[1].y=-5.0f;
    vertices[1].z=0.0f;

    vertices[0].x=0.99f;
    vertices[0].y=-5.0f;
    vertices[0].z=0.0f;

}

void PagRevolutionObject::setBattery(){

    divisiones=19;
    vertices = new vCoord [divisiones];

    vertices[18].x=0.1f;
    vertices[18].y=7.0f;
    vertices[18].z=0.0f;

    vertices[17].x=0.3f;
    vertices[17].y=7.0f;
    vertices[17].z=0.0f;

    vertices[16].x=0.4f;
    vertices[16].y=7.0f;
    vertices[16].z=0.0f;

    vertices[15].x=0.5f;
    vertices[15].y=6.8f;
    vertices[15].z=0.0f;

    vertices[14].x=0.5f;
    vertices[14].y=6.6f;
    vertices[14].z=0.0f;

    vertices[13].x=0.6f;
    vertices[13].y=6.6f;
    vertices[13].z=0.0f;

    vertices[12].x=0.7f;
    vertices[12].y=6.6f;
    vertices[12].z=0.0f;

    vertices[11].x=0.8f;
    vertices[11].y=6.6f;
    vertices[11].z=0.0f;

    vertices[10].x=0.8f;
    vertices[10].y=6.8f;
    vertices[10].z=0.0f;

    vertices[9].x=1.0f;
    vertices[9].y=6.8f;
    vertices[9].z=0.0f;

    vertices[8].x=1.1f;
    vertices[8].y=6.8f;
    vertices[8].z=0.0f;

    vertices[7].x=1.4f;
    vertices[7].y=6.5f;
    vertices[7].z=0.0f;

    vertices[6].x=1.4f;
    vertices[6].y=6.4f;
    vertices[6].z=0.0f;

    vertices[5].x=1.4f;
    vertices[5].y=0.0f;
    vertices[5].z=0.0f;

    vertices[4].x=1.4f;
    vertices[4].y=-6.0f;
    vertices[4].z=0.0f;

    vertices[3].x=1.4f;
    vertices[3].y=-6.1f;
    vertices[3].z=0.0f;

    vertices[2].x=1.1f;
    vertices[2].y=-6.4f;
    vertices[2].z=0.0f;

    vertices[1].x=1.0f;
    vertices[1].y=-6.4f;
    vertices[1].z=0.0f;

    vertices[0].x=0.1f;
    vertices[0].y=-6.4f;
    vertices[0].z=0.0f;

}

void PagRevolutionObject::setBasic(){

    divisiones=3;
    vertices = new vCoord [divisiones];

    vertices[2].x=1.0f;
    vertices[2].y=4.0f;
    vertices[2].z=0.0f;

    vertices[1].x=5.0f;
    vertices[1].y=0.0f;
    vertices[1].z=0.0f;

    vertices[0].x=1.0f;
    vertices[0].y=-4.0f;
    vertices[0].z=0.0f;

}
void PagRevolutionObject::setCono(){

    divisiones=3;
    vertices = new vCoord [divisiones];

    vertices[0].x=0.0f;
    vertices[0].y=0.0f;
    vertices[0].z=0.0f;

    vertices[1].x=1.5f;
    vertices[1].y=0.0f;
    vertices[1].z=0.0f;

    vertices[2].x=0.0f;
    vertices[2].y=3.0f;
    vertices[2].z=0.0f;

}

