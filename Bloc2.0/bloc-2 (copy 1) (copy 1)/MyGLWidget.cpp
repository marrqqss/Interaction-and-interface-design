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

void MyGLWidget::calculs(const glm::vec3 &pmin, const glm::vec3 &pmax)
{
    float dist = glm::distance(pmin, pmax);
    radi = dist / 2.0;
    centre = glm::vec3((pmax.x + pmin.x) / 2.0, (pmax.y + pmin.y) / 2.0, (pmax.z + pmin.z) / 2.0);
}


void MyGLWidget::initializeGL()
{
    BL2GLWidget::initializeGL();
    glEnable (GL_DEPTH_TEST);
    connect(&timer, SIGNAL(timeout()), this, SLOT(animar()));
    timer.start(16);
    iniCamera();
}

void MyGLWidget::iniCamera()
{
    calculs(glm::vec3(-2.5, 0, -2.5), glm::vec3(2.5, 1, 2.5));
    obs = centre + glm::vec3(0, 0, 2.0*radi);
    vrp = centre;
    up = glm::vec3(0, 1, 0);
    fov = 2*asin(float(radi/(2*radi)));
    ra = 1.0f;  
    znear = 2*radi - radi;
    zfar = 2*radi + radi;
    angle = 0.0f;
    angleAnim = 0.0f;
    perspectiva = true;
    left = -radi;
    right = radi;
    bot = -radi;
    top = radi;
    psi = 0;
    phi = 0;
    theta = 0;
    moving = false;
    projectTransform();
    viewTransform();
}


void MyGLWidget::paintGL()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelTransformModel();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Model);

    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (0);

    modelTransformTerra();

    glBindVertexArray (VAO_terra);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray (0);

    modelTransformModel2();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Model);

    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (0);

    modelTransformModel3();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Model);

    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (0);

    modelTransformModel4();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Model);

    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
    float ra_viewport = float (w) / float (h);
    ra = ra_viewport;
    if (perspectiva)
    {
        if (ra_viewport < 1.0) fov = 2.0 * atan(tan(fov/2.0)/ra_viewport);
        else fov = 2*asin(float(radi/(2*radi)));
    }
    else
    {
        if (ra == 1)
        {
            left = -radi;
            right = radi;
            bot = -radi;
            top = radi;
        }
        else if (ra < 1)
        {
            left = -radi;
            right = radi;
            bot = -radi/ra;
            top = radi/ra;
        }
        else if (ra > 1)
        {
            left = -radi*ra;
            right = radi*ra;
            bot = -radi;
            top = radi;
        }
    }
    projectTransform();
}

void MyGLWidget::creaBuffers()
{
    //Model
    m.load("../../models/Patricio.obj");
    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Model);
    glBindVertexArray(VAO_Model);

    GLuint VBO_Model[2];
    glGenBuffers(2, VBO_Model);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces ().size () * 3 * 3, m.VBO_vertices (), GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Model[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);

    //terra

    glm::vec3 posicio[4] = {
        glm::vec3(-2.5, 0.0, -2.5),
        glm::vec3(-2.5, 0.0, 2.5),
        glm::vec3(2.5,  0.0, 2.5),
        glm::vec3(2.5,  0.0, -2.5)
    }; 
    glm::vec3 color[4] = {
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(0,0,1),
        glm::vec3(0,0,1)
    };

    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_terra);
    glBindVertexArray(VAO_terra);

    GLuint VBO_Terra[2];
    glGenBuffers(2, VBO_Terra);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::calcul_capsa()
{
    float max_x, max_y, max_z, min_x, min_y, min_z;
    max_x = min_x = m.vertices()[0];
    max_y = min_y = m.vertices()[1];
    max_z = min_z = m.vertices()[2];

    for (int i = 0; i < m.vertices().size(); i += 3)
    {
        max_x = std::fmax(max_x, m.vertices()[i]);
        min_x = std::fmin(min_x, m.vertices()[i]);
        max_y = std::fmax(max_y, m.vertices()[i + 1]);
        min_y = std::fmin(min_y, m.vertices()[i + 1]);
        max_z = std::fmax(max_z, m.vertices()[i + 2]);
        min_z = std::fmin(min_z, m.vertices()[i + 2]);
    }

    centre_model = glm::vec3((max_x + min_x) / 2, min_y, (max_z + min_z) / 2);
    escala_model = 1/ (max_y - min_y);
}

void MyGLWidget::modelTransformModel() //(0,0,0)
{
    calcul_capsa();
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala_model*escala));
    transform = glm::rotate(transform, float(M_PI/2 + angle), glm::vec3(0, 1, 0));
    transform = glm::translate(transform, glm::vec3(-centre_model[0], -centre_model[1], -centre_model[2]));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformModel2() //(2,0,2)
{
    calcul_capsa();
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(2, 0, 2));
    transform = glm::scale(transform, glm::vec3(escala_model*escala));
    transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
    transform = glm::translate(transform, glm::vec3(-centre_model[0], -centre_model[1], -centre_model[2]));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformModel3() //(-2,0,-2)
{
    calcul_capsa();
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(-2, 0, -2));
    transform = glm::scale(transform, glm::vec3(escala_model*escala));
    transform = glm::rotate(transform, float(M_PI + angle), glm::vec3(0, 1, 0));
    transform = glm::translate(transform, glm::vec3(-centre_model[0], -centre_model[1], -centre_model[2]));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformModel4() //patricio que gira
{
    calcul_capsa();
    glm::mat4 transform (1.0f);
    transform = glm::rotate(transform, angleAnim, glm::vec3(0, 1, 0));
    transform = glm::translate(transform, glm::vec3(1.5, 0, 0));
    transform = glm::scale(transform, glm::vec3(escala_model*escala));
    transform = glm::translate(transform, glm::vec3(-centre_model[0], -centre_model[1], -centre_model[2]));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra()
{
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform()
{
    glm::mat4 Proj;
    if (perspectiva) Proj = glm::perspective(fov, ra, znear, zfar);
    else Proj = glm::ortho(left, right, bot, top, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform()
{
    //glm::mat4 View = glm::lookAt(obs, vrp, up);
    glm::mat4 View(1.0f); //IMPORTANTE PQ MOULTIPLICA
    View = translate(View, glm::vec3(0, 0, -2*radi));
    View = rotate(View, -phi, glm::vec3(0, 0, 1));
    View = rotate(View, theta,glm::vec3(1, 0, 0));
    View = rotate(View, -psi, glm::vec3(0, 1, 0));
    View = translate(View, -centre);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event)
{
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_R:  
      angle += M_PI/4;
      break;
    case Qt::Key_S:
        escala += 0.05;
        break;
    case Qt::Key_D:
        escala -= 0.05;
        break;
    case Qt::Key_O:
        perspectiva = !perspectiva;
        projectTransform();
        break;
    case Qt::Key_Z:
        fov -= 0.1;
        left += 0.1;
        right -= 0.1;
        top -= 0.1;
        bot += 0.1;
        projectTransform();
        break;
    case Qt::Key_X:
        fov += 0.1;
        left -= 0.1;
        right += 0.1;
        top += 0.1;
        bot -= 0.1;
        projectTransform();
        break;
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *event)
{
    xClick = event->x();
    yClick = event->y();
    if ( event->buttons() == Qt::LeftButton && !( event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier)))
    {
        moving = true;
    }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    moving = false;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    makeCurrent();
    if (moving)
    {
        psi -= (event->x() - xClick) * M_PI / 180.0;
        theta += (event->y() - yClick) * M_PI / 180.0;
        viewTransform();
    }
    xClick = event->x();
    yClick = event->y();
    update();
}

void MyGLWidget::animar()
{
    makeCurrent();
    angleAnim += 0.1;
    update();
}