#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    
    virtual void modelTransformCurrentBrick();

    virtual void resizeGL (int w, int h);

    float distance, fovopt;

    glm::vec3 currentPose;
    float angleBlock;
    glm::mat4 currentTG;
    int brickCount;

    GLuint modelIndexSaved[NUM_BRICKS];

    bool perspectiva;

    float left, right, bot, top;

  public slots:
    
  signals:


  private:
    int printOglError(const char file[], int line, const char func[]);
    
};
