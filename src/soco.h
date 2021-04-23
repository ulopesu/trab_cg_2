#ifndef SOCO_H
#define SOCO_H
#include <GL/gl.h>
#include <GL/glu.h>

class Soco
{
    GLfloat gXInit;
    GLfloat gYInit;
    GLfloat gX;
    GLfloat gY;
    GLfloat rSoco;

    bool valido;

private:
    void DesenhaSoco(GLfloat x, GLfloat y, GLfloat _rSoco);

public:
    Soco(GLfloat x, GLfloat y, GLfloat _rSoco)
    {
        gXInit = x;
        gYInit = y;
        gX = x;
        gY = y;
        rSoco = _rSoco;
        valido = true;
    };

    void Desenha()
    {
        DesenhaSoco(gX, gY, rSoco);
    };

    void Move(GLfloat x, GLfloat y)
    {
        gX = x;
        gY = y;
    };

    void GetPos(GLfloat &xOut, GLfloat &yOut)
    {
        xOut = gX;
        yOut = gY;
    };

    void setValido(bool _valido)
    {
        valido=_valido;
    };

    bool EhValido() 
    {
        return valido;
    };
};

#endif /* SOCO_H */