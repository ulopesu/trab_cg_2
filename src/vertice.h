#include <GL/gl.h>
#include "d3.h"

typedef struct
{
    // RGBs
    D3 ponto;

    // Normal
    D3 normal;

    // Textura
    GLfloat U;
    GLfloat V;
} Vertice;