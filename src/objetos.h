#ifndef OBJETO_H
#define OBJETO_H

#include <GL/gl.h>
#include <GL/glut.h>
#include "cor.h"
#include "vertice.h"

class Objeto
{
protected:
    Coordenada pos;
    Vertice *vtx;
    int numVtx;

public:
    Objeto(){ 
    };
    void free_obj();

    void DesenhaComCor(Cor *cor);
    void DesenhaComTextura(GLuint texture);
};

void DenhaCuboGLComCor(Coordenada posRelativa, Coordenada escala, Cor *cor);

#endif /* OBJETO_H */