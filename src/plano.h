#ifndef PLANO_H
#define PLANO_H

#include <math.h>
#include <GL/gl.h>
#include "objetos.h"

enum posPlano
{
    X,
    Y,
    Z,
};

class Plano: public Objeto
{   
private:
    D3 dimensao;
    posPlano posNormal;
public:
    Plano(D3 dimensao, posPlano posNormal);
    void DesenhaComCor(Cor cor){
        Objeto::DesenhaComCor(cor, GL_QUADS);
    };
    void DesenhaComTextura(GLuint texture){
        Objeto::DesenhaComTextura(texture, GL_QUADS);
    };
};

#endif /* PLANO_H */