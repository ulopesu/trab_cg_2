#ifndef COR_H
#define COR_H
#include <GL/gl.h>
#include <string>

typedef struct
{
    GLfloat R;
    GLfloat G;
    GLfloat B;
} RGB;

using namespace std;

class Cor
{   
private:
    GLfloat *vCor;

public:
    Cor(RGB rgb);
    Cor(string nameCor);
    GLfloat getR();
    GLfloat getG();
    GLfloat getB();
    void setRGB(RGB rgb);
    GLfloat* Cor2Vetor();
};

#endif /* COR_H */