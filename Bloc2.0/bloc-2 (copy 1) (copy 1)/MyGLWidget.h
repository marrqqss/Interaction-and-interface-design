// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include <QTimer>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    
    virtual void animar();

  protected:

    GLuint projLoc, viewLoc, VAO_Model, VAO_terra;

    float fov, ra, znear, zfar;
    float radi;
    glm::vec3 centre;
    float angle;
    glm::vec3 obs, vrp, up;
    float escala_model;
    glm::vec3 centre_model;
    bool perspectiva;
    float left, right, top, bot;
    float psi, theta, phi;
    int xClick, yClick;
    bool moving;

    float angleAnim;

    QTimer timer;

    Model m;

    virtual void initializeGL();
    virtual void carregaShaders();
    virtual void projectTransform();
    virtual void modelTransformModel();
    virtual void modelTransformModel2();
    virtual void modelTransformModel3();
    virtual void modelTransformModel4();
    virtual void modelTransformTerra();
    virtual void viewTransform();
    virtual void paintGL();
    virtual void iniCamera();
    virtual void creaBuffers();
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void calculs(const glm::vec3 &pmin, const glm::vec3 &pmax);
    virtual void resizeGL(int w, int h);
    virtual void calcul_capsa();

  private:
    int printOglError(const char file[], int line, const char func[]);
};
