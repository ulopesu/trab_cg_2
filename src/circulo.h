#ifndef CIRCULO_H
#define CIRCULO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "cor.h"

class Circulo
{   
    GLfloat raio;
    int nPts;
    GLfloat gX, gY;
    
private:


public:
    Circulo(GLfloat raio, int nPts, GLfloat gX, GLfloat gY);
    void desenhaPontos(GLfloat pSize, Cor _cor);
    void desenhaCompleto(Cor _cor);
    void desenhaComBorda(Cor _cor);
    void desenhaBorda(GLfloat pSize, Cor _cor);
    //bool isIn(GLfloat _x, GLfloat _y);

};

#endif /* CIRCULO_H */
