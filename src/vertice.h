#include <GL/gl.h>
#include "coordenada.h"

typedef struct
{
    // Coordenadas
    Coordenada ponto;

    // Normal
    Coordenada normal;

    // Textura
    GLfloat U;
    GLfloat V;
} Vertice;