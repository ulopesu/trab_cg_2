#include "objetos.h"

GLfloat mat_emission[] = {0.0, 0.0, 0.0, 0.0};
GLfloat mat_ambiente[] = {0.0, 0.0, 0.0, 0.0};
static GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat mat_shininess[] = {128};

void Objeto::free_obj()
{
    free(vtx);
}

void Objeto::DesenhaComCor(Cor cor, int glTipo)
{
    glPushAttrib(GL_ENABLE_BIT);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, cor.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(glTipo);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X, vtx[i].normal.Y, vtx[i].normal.Z);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
    glPopAttrib();
}

void Objeto::DesenhaComTextura(GLuint texture, int glTipo)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiente);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(glTipo);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X, vtx[i].normal.Y, vtx[i].normal.Z);
        glTexCoord2f(vtx[i].U, vtx[i].V);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
}

void DesenhaCuboGL(D3 posCubo, D3 posRelativa, D3 escala, Cor corCubo)
{

    glPushAttrib(GL_ENABLE_BIT);
    glPushMatrix();
    glTranslatef(posCubo.X, posCubo.Y, posCubo.Z);

    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, corCubo.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glScalef(escala.X, escala.Y, escala.Z);
    glTranslatef(posRelativa.X, posRelativa.Y, posRelativa.Z);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopAttrib();
}