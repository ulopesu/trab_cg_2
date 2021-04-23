#ifndef ESFERA_H
#define ESFERA_H

#include <math.h>
#include <GL/gl.h>
#include "objetos.h"


class Esfera: public Objeto
{   
private:
    GLfloat raio;
    GLfloat space;

public:
    Esfera(GLfloat r, GLfloat s);
};

#endif /* ESFERA_H */