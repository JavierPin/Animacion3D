#ifndef PAGSHADERPROGRAM_H
#define PAGSHADERPROGRAM_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenglfunctions_4_0_core.h>

class PagShaderProgram : protected QOpenGLFunctions_4_0_Core
{
public:
    PagShaderProgram(QString name);
    void use();
    void setColor(GLfloat c1, GLfloat c2, GLfloat c3);
    void setUniform(std::string name, GLfloat value);
    void setUniform(std::string name, QMatrix4x4 value);
    void setUniform(std::string name, float v0, float v1, float v2);
    void compileName(QString name);
    GLuint createShaderProgram(QString name, GLuint program);
    GLuint getShaderID();


private:

    GLuint shaderProgram;
    GLuint compileShader(QString name, GLenum type);
    GLfloat vcolor[3];

    GLuint program;
    void setShaderID(GLuint shaderProgram);



signals:

public slots:
};

#endif // PAGSHADERPROGRAM_H
