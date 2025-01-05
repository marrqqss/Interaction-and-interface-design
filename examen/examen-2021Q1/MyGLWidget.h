#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

    virtual void initializeGL ();
    virtual void resizeGL (int w, int h);


    int patricioCub; //1 = cub1; 2 = cub2; 3 = cub3; es canvia mitjançant tecles 1, 2 y 3
    bool pintaPatricio; //true = pinta nomes patricio; false = pinta nomes els cubs; es canvia amb tecla v

    float left, right, bot, top;

    float angleCub1, angleCub2, angleCub3;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
