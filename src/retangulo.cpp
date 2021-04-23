#include "retangulo.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

Retangulo::Retangulo(GLfloat width, GLfloat height, GLfloat _gX, GLfloat _gY){
    height = height;
    width = width/2;

    vertices[0][0] = -width;
    vertices[0][1] = 0.0;
    vertices[0][2] = 0.0;

    vertices[1][0] = width;
    vertices[1][1] = 0.0;
    vertices[1][2] = 0.0;

    vertices[2][0] = width;
    vertices[2][1] = height;
    vertices[2][2] = 0.0;

    vertices[3][0] = -width;
    vertices[3][1] = height;
    vertices[3][2] = 0.0;

    gX = _gX;
    gY = _gY;
}

void Retangulo::desenha(Cor* _cor)
{
    glColor3f(_cor->getR(), _cor->getG(), _cor->getB());
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(gX + vertices[i][0], gY + vertices[i][1], vertices[i][2]);
    }
    glEnd();
};

bool Retangulo::isIn(GLfloat _x, GLfloat _y)
{
    GLfloat x_MIN = gX + vertices[0][0];
    GLfloat y_MIN = gY + vertices[0][1];
    GLfloat x_MAX = gX + vertices[2][0];
    GLfloat y_MAX = gY + vertices[2][1];

    return (_x >= x_MIN && _x <= x_MAX && _y >= y_MIN && _y <= y_MAX) ? true : false;
}