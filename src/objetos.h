#ifndef OBJETO_H
#define OBJETO_H

#include <GL/gl.h>
#include "cor.h"
#include "vertice.h"

class Objeto
{
protected:
    Vertice *vtx;
    int numVtx;

public:
    Objeto(){ 
    };

    void DesenhaComCor(Cor *cor);
    void DesenhaComTextura(GLuint texture);
};

#endif /* OBJETO_H */