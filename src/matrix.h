#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "d3.h"

#define toRad (M_PI / 180)

void imprimeMatrix(GLfloat** mtx, int N);
GLfloat** zeroMatrix(int N);
GLfloat** identityMatrix(int N);
GLfloat **transposedMatrix(GLfloat **mtx, int N);
GLfloat** multiMatrix(GLfloat **m1, GLfloat **m2, int N);
GLfloat** rotateZmatrix(GLfloat **m1, GLfloat c,  int N);
GLfloat** rotateYmatrix(GLfloat **m1, GLfloat b,  int N); 
GLfloat** rotateXmatrix(GLfloat **m1, GLfloat a,  int N);
GLfloat** rotateMatrix(GLfloat **m1, GLfloat a, GLfloat b, GLfloat c, int N);
GLfloat** translateMatrix(GLfloat **m1, GLfloat dx, GLfloat dy, GLfloat dz, int N);
GLfloat** scaleMatrix(GLfloat **m1, GLfloat sx, GLfloat sy, GLfloat sz, int N);
GLfloat dist(D3 p1, D3 p2);

