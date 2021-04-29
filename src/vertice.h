#include <GL/gl.h>
#include "matrix.h"

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