#include "pagshaderprogram.h"
#include <QtDebug>
#include <QFileInfo>

GLuint shaderID;

PagShaderProgram::PagShaderProgram(QString name)
{
    initializeOpenGLFunctions();

    program = glCreateProgram();
    if (program == 0) {
        qDebug() << "No se puede crear el shader program";
    }

    GLuint vertexShaderObject = compileShader(":/"+name+".vert", GL_VERTEX_SHADER);
    GLuint fragmentShaderObject = compileShader(":/"+name+".frag", GL_FRAGMENT_SHADER);
    GLuint geometryShaderObject = compileShader(":/"+name+".geom", GL_GEOMETRY_SHADER);

    glAttachShader(program, vertexShaderObject);
    glAttachShader(program, fragmentShaderObject);
    glAttachShader(program, geometryShaderObject);

    shaderProgram = createShaderProgram(name, program);

}

void PagShaderProgram::setColor(GLfloat c1, GLfloat c2, GLfloat c3){

    vcolor[0] = c1;
    vcolor[1] = c2;
    vcolor[2] = c3;
    setUniform("vColor", vcolor[0], vcolor[1], vcolor[2]);

}

void PagShaderProgram::use(){

    glUseProgram(program);

}

//points position
void PagShaderProgram::setUniform(std::string name, QMatrix4x4 value)
{
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location >= 0) {
        glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
    } else {
        qDebug() << "No es posible encontrar localización para: " << name.c_str();
    }
}

//points size
void PagShaderProgram::setUniform(std::string name, GLfloat value)
{
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location >= 0) {
        glUniform1f(location, value);
    } else {
        qDebug() << "No es posible encontrar localización para: " << name.c_str();
    }
}

//points color
void PagShaderProgram::setUniform(std::string name, float v0, float v1, float v2)
{

    GLint location = glGetUniformLocation(program, name.c_str());
    if (location >= 0) {
        glUniform3f(location, v0, v1, v2);
    } else {
        qDebug() << "No es posible encontrar localización para: " << name.c_str();
    }
}

GLuint PagShaderProgram::compileShader(QString name, GLenum type)
{
    QFileInfo shaderName(name);
    if (!shaderName.exists())
    {
        //qDebug() << "No existe el shader source: " << name;
        return 0;
    }

    std::string shaderString;
    QFile shaderSource(name);
    if (!shaderSource.open(QIODevice::ReadOnly))
    {
        qDebug() << "No se puede abrir el archivo " << name;
        return 0;
    }
    shaderString = (std::string)shaderSource.readAll();
    shaderSource.close();

    GLuint shaderHandler = glCreateShader(type);
    if (shaderHandler == 0) {
        qDebug() << "No se puede crear el shader object";
        return 0;
    }

    const char *c_str = shaderString.c_str();
    glShaderSource(shaderHandler, 1, &c_str, NULL);

    glCompileShader(shaderHandler);
    GLint compileResult;

    glGetShaderiv(shaderHandler,GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        qDebug() << "Error al compilar el shader: " << name;
        GLint logLen = 0;
        glGetShaderiv(shaderHandler, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char * log = new char[logLen];
            GLint written = 0;
            glGetShaderInfoLog(shaderHandler,logLen,&written, log);
            qDebug() << log << endl;
            delete [] log;
        }
    }

    return shaderHandler;
}

GLuint PagShaderProgram::createShaderProgram(QString name, GLuint program)
{

    glLinkProgram(program);
    GLint linkSuccess = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        qDebug() << "Error al enlazar el shader program: " << name;
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char * log = new char[logLen];
            GLint written = 0;
            glGetProgramInfoLog(program,logLen,&written, log);
            qDebug() << log << endl;
            delete [] log;
        }
    }

    return program;
}
