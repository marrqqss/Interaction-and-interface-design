// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

float MyGLWidget::max(float x, float y)
{
    if (x > y) return x;
    return y;
}

float MyGLWidget::min(float x, float y)
{
    if (x > y) return y;
    return x;
}

void MyGLWidget::calculateMaxPoints()
{
    for (uint i = 0; i < m.vertices().size(); i+=3)
    {
        if (i == 0)
        {
            pmax[0] = m.vertices()[0];
            pmax[1] = m.vertices()[1];
            pmax[2] = m.vertices()[2];
            pmin[0] = m.vertices()[0];
            pmin[1] = m.vertices()[1];
            pmin[2] = m.vertices()[2];
        }
        else
        {
            pmax[0] = max(pmax[0], m.vertices()[i]);
            pmax[1] = max(pmax[1], m.vertices()[i+1]);
            pmax[2] = max(pmax[2], m.vertices()[i+2]);
            pmin[0] = min(pmin[0], m.vertices()[i]);
            pmin[1] = min(pmin[1], m.vertices()[i+1]);
            pmin[2] = min(pmin[0], m.vertices()[i]);
        }
    }
}

void MyGLWidget::initializeGL ( )
{
    BL2GLWidget::initializeGL();
    //cargamos modelo de homer
    m.load("./models/HomerProves.obj");
    glEnable(GL_DEPTH_TEST);
    //hay que llamar a las nuevas funciones para que se ejecuten
    projectTransform();
    viewTransform();
}

void MyGLWidget::creaBuffers ()
{
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Homer);
    glBindVertexArray(VAO_Homer);

    GLuint VBO_Homer[2];
    glGenBuffers(2, VBO_Homer);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
    //llamamos al codigo del padre y luego añadimos más codigo
    //BL2GLWidget::carregaShaders();
    //ejemploLoc = glGetUniform...

    BL2GLWidget::carregaShaders();
    prjLoc = glGetUniformLocation(program->programId(), "PRJ");
    vmLoc = glGetUniformLocation(program->programId(), "VM");
    //accedemos a los vbos del modelo, primero al de posicion y segundo al de color
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices (), GL_STATIC_DRAW);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff (), GL_STATIC_DRAW);
}

void MyGLWidget::modelTransform()
{
    // Matriu de transformació de model
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::paintGL()
{
    // Esborrem el frame-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model

    modelTransform();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray(VAO_Homer);

    // pintem
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size() * 3); //!!!!!!!!! IMPORTANTE CAMBIAR ULTIMO PARAMETRO

    glBindVertexArray (0);
}


void MyGLWidget::projectTransform()
{
    glm::mat4 Proj = glm::perspective(float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f);
    glUniformMatrix4fv(prjLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform()
{
    glm::mat4 View = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv(vmLoc, 1, GL_FALSE, &View[0][0]);
}


