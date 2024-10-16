#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  angle = 0;
  escala_ull = 0;
  posx = 0;

  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  pintaCap();
  pintaUll();
  pintaParpella();
  pintaMandibula(false, 0.0);
  pintaMandibula(true, angle);
      
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::pintaUll(){
  // Pintem l'ull
  glBindVertexArray(VAO_Quadrat_blanc);
  modelTransformQuadrat(glm::vec3(-0.15, 0.25, 0.0), glm::vec3(0.2,0.2,0.2));
  glDrawArrays(GL_TRIANGLES, 0, 6);    	
}	

void MyGLWidget::pintaParpella()
{
  glBindVertexArray(VAO_Quadrat_verd);
  modelTransformQuadrat(glm::vec3(-0.15, 0.25, 0.0), glm::vec3(0.2, escala_ull, 0.2));
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MyGLWidget::pintaCap(){
  // Pintem el cap
  glBindVertexArray(VAO_Quadrat_verd);
  modelTransformQuadrat(glm::vec3(0,0.4,0), glm::vec3(0.8,0.8,0.8));
  glDrawArrays(GL_TRIANGLES, 0, 6);    	
}	

void MyGLWidget::pintaMandibula(bool up, double angleApertura){
  glBindVertexArray(VAO_Mandibula);
  modelTransformMandibula(up, angleApertura);
  glDrawArrays(GL_TRIANGLES, 0, 18);
}

void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(posx+posicio[0], posicio[1], posicio[2]));  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMandibula(bool up, double angleApertura)
{
  if (not up) //mandibula inferior
  {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(posx + 0,-0.2,0));
    TG = glm::scale(TG, glm::vec3(0.6,0.6,0.6));
    glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
  }
  else //mandibula superior
  {
    glm::mat4 TG(1.0f);
    
    TG = glm::translate(TG, glm::vec3(posx -0.075,0,0));
    TG = glm::rotate(TG, glm::radians(float(angleApertura)), glm::vec3(0,0,1));
    TG = glm::translate(TG, glm::vec3(0,0.16,0));
    TG = glm::scale(TG, glm::vec3(0.6,0.6,0.6));
    TG = glm::rotate(TG, glm::radians(180.0f), glm::vec3(1,0,0));
    glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
  }
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: 
      if (angle < 45)
      {
        angle += 5;
        paintGL();
      }
        break;
    case Qt::Key_Down: 
      if (angle > 0)
      {
        angle -= 5;
        paintGL();
      }
    	break;
    case Qt::Key_Right:
      posx += 0.2;
      paintGL();
    	break;
    case Qt::Key_Left: 
    	posx -= 0.2;
      paintGL();
      break;
    case Qt::Key_W:
      if (escala_ull < 0.2)
      {
        escala_ull += 0.02;
        paintGL();
      }
    	break;
    case Qt::Key_S:
      if (escala_ull > 0)
      {
        escala_ull -= 0.02;
        paintGL();
      }
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffers (){
    creaBuffersMandibula();
    VAO_Quadrat_verd = creaBuffersQuadrat(verd);
    VAO_Quadrat_blanc = creaBuffersQuadrat(blanc);
}

GLuint MyGLWidget::creaBuffersQuadrat (glm::vec3 color)
{
  GLuint VAOId;

  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  Vertices[0] = glm::vec3(-1.0, 0.0, 0);
  Vertices[1] = glm::vec3( 0.0, -1.0, 0);
  Vertices[2] = glm::vec3( 0.0, 0.0, 0);
  Vertices[3] = glm::vec3( 0.0, -1.0, 0);
  Vertices[4] = glm::vec3( -1.0, 0.0, 0);
  Vertices[5] = glm::vec3( -1.0, -1.0, 0);
  
  glm::vec3 Colors[6];
  for(int i=0;i<6;i++) {
      Colors[i] = color;//
  }

  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOId);
  glBindVertexArray(VAOId);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  // retornem l'identificador de VAO creat
  return VAOId;
}

void MyGLWidget::creaBuffersMandibula()
{
  int numDents = 4;
  float w = 1;
  float h = 0.2;
  float toothWidth = w / numDents;
  int numVertexs = (numDents+2) * 3;
  glm::vec3 Colors[numVertexs];
  glm::vec3 Vertices[numVertexs];
  glm::vec3 black(0,0,0);
  int i=0;
  Vertices[i++] = glm::vec3(0.0, 0.0, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w, h, 0.0);
  Vertices[i++] = glm::vec3(0.0, h, 0.0);
  Vertices[i++] = glm::vec3(w, 0.0, 0.0);
  float x=0;
  for(int d=0;  d<numDents; d++, x+=toothWidth) {
      Vertices[i++] = glm::vec3(x, h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth/2, 2*h, 0.0);
      Vertices[i++] = glm::vec3(x+toothWidth, h, 0.0);
  }
  i = 0;
  for(i;i<6;i++) {
      Colors[i]=verd;
  }
  for (int d=0; d<numDents; d++)
  {
    Colors[i++] = blanc;
    Colors[i++] = blanc_dent;
    Colors[i++] = blanc;
  }

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO_Mandibula);
  glBindVertexArray(VAO_Mandibula);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO[2];
  glGenBuffers(2, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Creació del buffer de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificadors per als atributs “vertex” i "color" del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 
}
