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

MyGLWidget::MyGLWidget(QWidget *parent) : ExamGLWidget(parent) {

		
}


MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::iniEscena ()
{
  ExamGLWidget::iniEscena();
  currentPose = glm::vec3(0,0,0);
  angleBlock = 0;
  brickCount = 0;

}

void MyGLWidget::iniCamera ()
{
  perspectiva = true;
  glm::vec3 pmin = glm::vec3(-10,-1,-10);
  glm::vec3 pmax = glm::vec3(10, 9.75,10); //alçada 10 blocs
  radiEsc = glm::distance(pmin, pmax)/2.0f;
  distance = 2.0f*radiEsc;
  angleY = 15.0f*((2.0f*float(M_PI))/360.0f);
  angleX = 15.0f*((2.0f*float(M_PI))/360.0f);
  ra = float(width())/height();
  fovopt = 2.0f*asin(radiEsc/distance);
  fov = fovopt;
  zn = distance-radiEsc;
  zf = distance+radiEsc;
  left = -10;
  right = 10;
  bot = -10;
  top = 10;

  projectTransform ();
  viewTransform ();
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
  if (perspectiva)
  {
    if (ra < 1) fov = 2.0f*atan(tan(fovopt/2.0f)/ra);
    else fov = fovopt;
  }
  else
  {
    if (ra > 1)
    {
      left = -10*ra;
      right = 10*ra;
    }
    else if (ra < 1)
    {
      bot = -10/ra;
      top = 10/ra;
    }
  }
  projectTransform(); 
}

void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra = GROUND_BRICKS
  glm::vec3 red = glm::vec3 (0.7, 0, 0);
  glUniform3fv(colorLoc,1, &red[0]);
  glBindVertexArray (VAO_models[GROUND_BRICKS]);
  modelTransformGround();
  glDrawArrays(GL_TRIANGLES, 0, models[GROUND_BRICKS].faces().size()*3);
   
  // Pintem el bloc de Lego actual (currentBrick) del color actual
  glUniform3fv(colorLoc,1,&currentColor[0]);
  glBindVertexArray (VAO_models[currentBrickModelIndex]);
  modelTransformCurrentBrick();
  glDrawArrays(GL_TRIANGLES, 0, models[currentBrickModelIndex].faces().size()*3); 

  // Pintem el bloc de Lego actual un altre cop pero nomes les linies en negre
  glm::vec3 black = glm::vec3(0,0,0);
  glUniform3fv(colorLoc,1,&black[0]);
  glBindVertexArray (VAO_models[currentBrickModelIndex]);
  modelTransformCurrentBrick();
  glDrawArrays(GL_LINES, 0, models[currentBrickModelIndex].faces().size()*3); 
  
  for (int i = 0; i < NUM_BRICKS; ++i)
  {
    if (brickTGs[i] != glm::mat4(1))
    {
      glUniform3fv(colorLoc,1,&brickColors[i][0]);
      glBindVertexArray (VAO_models[modelIndexSaved[i]]);
      modelTransformBrick(i);
      glDrawArrays(GL_TRIANGLES, 0, models[modelIndexSaved[i]].faces().size()*3); 

      // Pintem el bloc de Lego actual un altre cop pero nomes les linies en negre
      glm::vec3 black = glm::vec3(0,0,0);
      glUniform3fv(colorLoc,1,&black[0]);
      glBindVertexArray (VAO_models[modelIndexSaved[i]]);
      modelTransformBrick(i);
      glDrawArrays(GL_LINES, 0, models[modelIndexSaved[i]].faces().size()*3); 
    }
  }

  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCurrentBrick()
{
  currentTG = glm::mat4(1.f);
  if (currentBrickModelIndex == 1)
  {
    if (angleBlock == 0 or angleBlock == 2.0f*M_PI)
    {
      if (currentPose.x >= 10) currentPose.x -= 1;
      currentTG = glm::translate(currentTG, currentPose + glm::vec3(0.5,0,0)); 

    }
    else 
    {
      if (currentPose.z >= 10) currentPose.z -= 1;
      currentTG = glm::translate(currentTG, currentPose + glm::vec3(0,0,0.5)); 
    }
    
  } 
  else currentTG = glm::translate(currentTG, currentPose); 
  currentTG = glm::rotate(currentTG, angleBlock, glm::vec3(0,1,0));
  currentTG = glm::rotate(currentTG, float(M_PI/2.0f), glm::vec3(1,0,0));
  currentTG = glm::scale(currentTG, glm::vec3 (escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex]));
  currentTG = glm::translate(currentTG, -centreCapsaModels[currentBrickModelIndex]);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &currentTG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1, 0, 0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (perspectiva) Proj = glm::perspective(fov, ra, zn, zf);
  else Proj = glm::ortho(left, right, bot, top, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: { //-X
    if (currentPose.x > -10)currentPose = currentPose - glm::vec3(1,0,0);
    break;
	}
  case Qt::Key_D: { //+X
    if (currentPose.x < 10) currentPose = currentPose + glm::vec3(1,0,0);
    break;
    }
  case Qt::Key_W: { //+Z
    if (currentPose.z < 10) currentPose = currentPose + glm::vec3(0,0,1);
    break;
	}
  case Qt::Key_S: { //-Z
    if (currentPose.z > -10) currentPose = currentPose - glm::vec3(0,0,1);
    break;  
	}
  case Qt::Key_Up: { //+Y
    if (currentPose.y < 10) currentPose = currentPose + glm::vec3(0,1.25,0);
    break;
	}
  case Qt::Key_Down: { //-Y
    if (currentPose.y > 0) currentPose = currentPose - glm::vec3(0,1.25,0);
    break;  
	}		
  case Qt::Key_Q: {
    angleBlock += M_PI/2.0f;
    break;
	}
  case Qt::Key_C: {
    perspectiva = !perspectiva;
    projectTransform();
    break;
	}
  case Qt::Key_Space: {
    if (brickCount < NUM_BRICKS)
    {
      brickTGs[brickCount] = currentTG;
      brickColors[brickCount] = currentColor;
      modelIndexSaved[brickCount] = currentBrickModelIndex;
      ++brickCount;
      currentPose = glm::vec3(0,0,0);
      angleBlock = 0;
    }
    break;
    }
  case Qt::Key_R: {
    currentPose = glm::vec3(0,0,0);
    angleBlock = 0;
    brickCount = 0;
    for (int i = 0; i < NUM_BRICKS; ++i)
    {
      brickTGs[i] = glm::mat4(1);
      brickColors[i] = glm::vec3(1,1,1);
    }
    perspectiva = true;
    currentBrickObjectIndex = 0;
    currentBrickModelIndex = 1;

    // inicialitzem el color actual
    currentColor = white;	
    projectTransform();

    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació (només en Y)
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}


