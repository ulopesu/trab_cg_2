#ifndef RETANGULO_H
#define RETANGULO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "cor.h"

class Retangulo
{
    GLfloat gX, gY;
    GLfloat vertices[4][3];

private:


public:
    Retangulo(GLfloat width, GLfloat height, GLfloat gX, GLfloat gY);
    void desenha(Cor* _cor);
    bool isIn(GLfloat _x, GLfloat _y);

};

#endif /* RETANGULO_H */
