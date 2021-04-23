#include "objetos.h"

static GLfloat materialColorA[] = {0.1, 0.1, 0.1, 1};
static GLfloat materialColorD[] = {0.2, 0.2, 0.2, 1};
static GLfloat mat_specular[] = {0.3, 0.3, 0.3, 1};
static GLfloat mat_shininess[] = {100.0};

void Objeto::DesenhaComCor(Cor *cor) { 
    glColor3f(1, 1, 1);

    glMaterialfv(GL_FRONT, GL_EMISSION, cor->Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X , vtx[i].normal.Y, vtx[i].normal.Z);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
}


void Objeto::DesenhaComTextura(GLuint texture)
{
    GLfloat materialEmission[] = {0.10, 0.10, 0.10, 1};
    GLfloat materialColorA[] = {0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = {1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = {100.0};
    glColor3f(1, 1, 1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X , vtx[i].normal.Y, vtx[i].normal.Z);
        glTexCoord2f(vtx[i].U, vtx[i].V);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
}