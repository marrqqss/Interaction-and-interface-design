// MyGLWidget.h
#include "BL2GLWidget.h"
//definimos el .h de model, previamente añadido al source en el .pro
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

    //sobreescribimos la funcion de la clase padre para reescribirla completamente
    virtual void initializeGL();

    //sobreescribimos la declaracion de carregaShaders de su clase padre BL2GLWidget para añadir codigo
    //sin modificar el fichero padre (esta prohibido)
    virtual void carregaShaders();

    //sobreescribimos paintGL para pintar el modelo en vez de la "casita"
    virtual void paintGL ( );

    //sobreescribimos el creaBuffer para que no genere los vertices de la casita y para crear vao de homer
    virtual void creaBuffers ();

    //sobreescribimos la declaracion del padre
    virtual void modelTransform ();

    //declaramos nueva funcion para hacer el transform de perspective/ortogonal
    void projectTransform();

    //declaramos nueva funcion para hacer el transform de la matriz de camara
    void viewTransform();

    float max(float x, float y);

    float min(float x, float y);

    //creamos una nueva funcion para calcular el punto minimo y el maximo de Homer
    void calculateMaxPoints();

    glm::vec3 pmax;
    glm::vec3 pmin;

    //declaramos el modelo
    Model m;

    //declaramos VAO para modelo
    GLuint VAO_Homer;

    GLuint prjLoc;
    GLuint vmLoc;

  private:
    int printOglError(const char file[], int line, const char func[]);
};
