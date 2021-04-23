#ifndef COR_H
#define COR_H
#include <GL/gl.h>
#include <string>
using namespace std;

class Cor
{   
private:
    GLfloat R;
    GLfloat G;
    GLfloat B;
    GLfloat *vCor;

public:
    Cor(GLfloat r, GLfloat g, GLfloat b);
    Cor(string nameCor);
    GLfloat getR();
    GLfloat getG();
    GLfloat getB();
    void setRGB(GLfloat R, GLfloat G, GLfloat B);
    GLfloat* Cor2Vetor();
};

#endif /* COR_H */