// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

    protected:

      GLuint projLoc, viewLoc, VAO_Homer, VAO_terra;

      float fov, ra, znear, zfar;
      float angle;
      glm::vec3 obs, vrp, up;

      Model m;

      virtual void initializeGL();
      virtual void carregaShaders();
      virtual void projectTransform();
      virtual void modelTransformHomer();
      virtual void modelTransformTerra();
      virtual void viewTransform();
      virtual void paintGL();
      virtual void iniCamera();
      virtual void creaBuffers();
      virtual void keyPressEvent (QKeyEvent *event);

  private:
    int printOglError(const char file[], int line, const char func[]);
};
