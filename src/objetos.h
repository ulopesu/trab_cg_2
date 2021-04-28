#ifndef OBJETO_H
#define OBJETO_H

#include <GL/gl.h>
#include <GL/glut.h>
#include "cor.h"
#include "vertice.h"

class Objeto
{
protected:
    D3 pos;
    Vertice *vtx;
    int numVtx;

public:
    Objeto(){ 
    };
    void free_obj();

    void DesenhaComCor(Cor cor, int glTipo);
    void DesenhaComTextura(GLuint texture, int glTipo);
};

void DesenhaCuboGL(D3 posCubo, D3 posRelativa, D3 escala, Cor corCubo);

#endif /* OBJETO_H */