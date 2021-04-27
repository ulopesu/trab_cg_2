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
    Esfera(D3 xyz, GLfloat r, GLfloat s);
    void DesenhaComCor(Cor cor){
        Objeto::DesenhaComCor(cor, GL_TRIANGLE_STRIP);
    };
    void DesenhaComTextura(GLuint texture){
        Objeto::DesenhaComTextura(texture, GL_TRIANGLE_STRIP);
    };
};

#endif /* ESFERA_H */