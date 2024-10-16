#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void pintaUll();
    void pintaCap();
    void pintaMandibula(bool up, double angleApertura);
    void pintaParpella();

    void creaBuffers ();        
    void creaBuffersMandibula();    
    GLuint creaBuffersQuadrat(glm::vec3 color);
    
    void carregaShaders();
    
    void modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala);
    void modelTransformMandibula(bool up, double angleApertura);
    
    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint TGLoc;

    // VAOs
    GLuint VAO_Quadrat_verd, VAO_Quadrat_blanc, VAO_Mandibula;

    // viewport
    GLint ample, alt;    
    
    //angle mandibula superior
    double angle;
    float escala_ull;
    float posx;
    // colors
    glm::vec3 verd = glm::vec3(0.3,0.6,0.3);
    glm::vec3 blanc = glm::vec3(1.0,1.0,1.0);
    glm::vec3 blanc_dent = glm::vec3(0.78, 0.78, 0.5);
};
