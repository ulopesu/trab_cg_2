#include "objetos.h"

GLfloat emi = 0;
GLfloat amb = 1;
GLfloat spc = 1;
GLfloat dif = 0.5;
GLfloat mat_shininess = 100;

GLfloat mat_emission[] = {emi, emi, emi, 1.0};
GLfloat mat_ambiente[] = {amb, amb, amb, 1.0};
GLfloat mat_diffuse[] = {dif, dif, dif, 1.0};
GLfloat mat_specular[] = {spc, spc, spc, 1.0};

void Objeto::free_obj()
{
    free(vtx);
}

void Objeto::DesenhaComCor(Cor cor, int glTipo)
{
    glPushAttrib(GL_ENABLE_BIT);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, cor.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cor.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(1, 1, 1);

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
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushAttrib(GL_ENABLE_BIT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(glTipo);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X, vtx[i].normal.Y, vtx[i].normal.Z);
        glTexCoord2f(vtx[i].U, vtx[i].V);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
    glPopAttrib();
}

void DesenhaCuboGL(D3 posCubo, D3 posRelativa, D3 escala, Cor corCubo)
{
    glPushAttrib(GL_ENABLE_BIT);
    glPushMatrix();
    glTranslatef(posCubo.X, posCubo.Y, posCubo.Z);

    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, corCubo.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, corCubo.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glScalef(escala.X, escala.Y, escala.Z);
    glTranslatef(posRelativa.X, posRelativa.Y, posRelativa.Z);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopAttrib();
}