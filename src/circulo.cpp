#include "circulo.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

Circulo::Circulo(GLfloat _raio, int _nPts, GLfloat _gX, GLfloat _gY)
{
    raio = _raio;
    nPts = _nPts;
    gX = _gX;
    gY = _gY;
}

void Circulo::desenhaPontos(GLfloat pSize, Cor _cor)
{
    GLfloat angle;
    int i;
    glColor3f(_cor.getR(), _cor.getG(), _cor.getB());
    glPointSize(pSize);
    glBegin(GL_POINTS);
    for (i = 0; i < nPts; i++)
    {
        angle = 2 * M_PI * i / nPts;
        if (i % 6 == 1)
        {
            glVertex2i(gX + raio * cos(angle), gY + raio * sin(angle));
        }
    }
    glEnd();
}

void Circulo::desenhaCompleto(Cor _cor)
{
    GLfloat angle;
    int i;
    glColor3f(_cor.getR(), _cor.getG(), _cor.getB());
    glBegin(GL_POLYGON);
    for (i = 0; i < nPts; i++)
    {
        angle = 2 * M_PI * i / nPts;
        glVertex2f(gX + raio * cos(angle), gY + raio * sin(angle));
    }
    glEnd();
}

void Circulo::desenhaBorda(GLfloat pSize, Cor _cor)
{
    GLfloat angle;
    int i;
    glColor3f(_cor.getR(), _cor.getG(), _cor.getB());
    glPointSize(pSize);
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < nPts; i++)
    {
        angle = 2 * M_PI * i / nPts;
        glVertex2i(gX + raio * cos(angle), gY + raio * sin(angle));
    }
    glEnd();
}

void Circulo::desenhaComBorda(Cor _cor)
{   
    GLfloat dRGB = dRGB;
    RGB rgb = {_cor.getR() - dRGB, _cor.getG() - dRGB, _cor.getG() - dRGB};
    desenhaCompleto(Cor(rgb));
    this->raio -= 2;

    GLfloat angle;
    int i;

    glColor3f(_cor.getR(), _cor.getG(), _cor.getB());
    glBegin(GL_POLYGON);
    for (i = 0; i < nPts; i++)
    {
        angle = 2 * M_PI * i / nPts;
        glVertex2f(gX + raio * cos(angle), gY + raio * sin(angle));
    }
    glEnd();
}