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

void MyGLWidget::initializeGL ()
{
  patricioCub = 1;
  pintaPatricio = false;
  left = -10.0f;
  right = 10.f;
  bot = -10.0f;
  top = 10.0f;
  angleCub1 = 0;
  angleCub2 = float(2*M_PI/3);
  angleCub3 = float(2*float(2*M_PI/3));
  ExamGLWidget::initializeGL();
}


void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (!patricioCub)
  {
    // Pintem el cub 1
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2.0, angleCub1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Pintem el cub 2
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2.5, angleCub2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Pintem el cub 3
    glBindVertexArray(VAO_Cub);
    modelTransformCub (3, angleCub3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  else
  {
    // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }

  glBindVertexArray(0);

}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  glm::mat4 TG = glm::mat4(1.0);
  TG = rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = scale(TG, glm::vec3(escala/0.5,escala/0.5,escala/0.5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  calculaCapsaModel(patr, escala, centreBasePat);

  TG = glm::mat4(1.f);
  if (patricioCub == 1)
  {
    TG = rotate(TG, angleCub1, glm::vec3(0, 1, 0));
    TG = translate(TG, glm::vec3(5,0,0));
  }
  else if (patricioCub == 2)
  {
    TG = rotate(TG, angleCub2, glm::vec3(0, 1, 0));
    TG = translate(TG, glm::vec3(5,0,0));
  }
  else if (patricioCub == 3)
  {
    TG = rotate(TG, angleCub3, glm::vec3(0, 1, 0));
    TG = translate(TG, glm::vec3(5,0,0));
  }

  TG = glm::rotate(TG, -float(M_PI/2.0f), glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {

    glm::mat4 Proj = glm::ortho(left, right, bot, top, 0.0f, 30.0f);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
 
  }
}

void MyGLWidget::resizeGL (int w, int h)
{
  #ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif

  ra = float(ample)/float(alt);
  if (camPlanta)
  {
    if (ra > 1)
    {
      left = -10*ra;
      right = 10*ra;
    }
    else
    {
      bot = -10/ra;
      top = 10/ra;
    }
  }
  projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
    patricioCub = !patricioCub;
    break;
	}
  case Qt::Key_1: {
    patricioCub = 1;
    break;
	}
  case Qt::Key_2: {
    patricioCub = 2;
    break;
	}
  case Qt::Key_3: {
    patricioCub = 3;
    break;
	}
  case Qt::Key_F: {
    if (colFoc == glm::vec3(1,1,1)) colFoc = glm::vec3(1,1,0); //groc;
    else if (colFoc == glm::vec3(1,1,0)) colFoc = glm::vec3(1,1,1);
    enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
        angleCub1 += float(2*M_PI/3);
        angleCub2 += float(2*M_PI/3);
        angleCub3 += float(2*M_PI/3);
    break;
	}
  case Qt::Key_Left: {
        angleCub1 -= float(2*M_PI/3);
        angleCub2 -= float(2*M_PI/3);
        angleCub3 -= float(2*M_PI/3);
    break;
	}
  case Qt::Key_R: {
    patricioCub = 1;
    pintaPatricio = false;
    angleCub1 = 0;
    angleCub2 = float(2*M_PI/3);
    angleCub3 = float(2*float(2*M_PI/3));
    camPlanta = false;
    colFoc = glm::vec3(1,1,1);
    enviaColFocus();
    viewTransform();
    projectTransform();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

