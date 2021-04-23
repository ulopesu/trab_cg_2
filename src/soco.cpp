#include "soco.h"
#include "circulo.h"


void Soco:: DesenhaSoco(GLfloat x, GLfloat y, GLfloat _rSoco)
{
    Circulo *circ = new Circulo(_rSoco, 100, x, y);
    circ->desenhaCompleto(new Cor(1,0.2,0.2));
    free(circ);
}