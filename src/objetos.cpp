#include "objetos.h"

GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1.0};
static GLfloat mat_specular[] = {0.3, 0.3, 0.3, 1};
static GLfloat mat_shininess[] = {100.0};

void Objeto::free_obj(){
    free(vtx);
}

void Objeto::DesenhaComCor(Cor cor, int glTipo) { 
    glColor3f(1, 1, 1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, cor.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cor.Cor2Vetor());
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(glTipo);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X , vtx[i].normal.Y, vtx[i].normal.Z);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
}


void Objeto::DesenhaComTextura(GLuint texture, int glTipo)
{
    GLfloat materialEmission[] = {0.0, 0.0, 0.0, 1};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = {100.0};
    glColor3f(1, 1, 1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(glTipo);
    for (int i = 0; i < numVtx; i++)
    {
        glNormal3f(vtx[i].normal.X , vtx[i].normal.Y, vtx[i].normal.Z);
        glTexCoord2f(vtx[i].U, vtx[i].V);
        glVertex3f(vtx[i].ponto.X + pos.X, vtx[i].ponto.Y + pos.Y, vtx[i].ponto.Z + pos.Z);
    }
    glEnd();
}

void DenhaCuboGLComCor(D3 posRelativa, D3 escala, Cor cor){ 
    
    GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1.0};
    static GLfloat mat_specular[] = {0.3, 0.3, 0.3, 1};
    static GLfloat mat_shininess[] = {100.0};

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
        glMaterialfv(GL_FRONT, GL_AMBIENT, cor.Cor2Vetor());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cor.Cor2Vetor());
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glColor3fv(cor.Cor2Vetor());
        glScalef(escala.X, escala.Y, escala.Z);
        glTranslatef(posRelativa.X, posRelativa.Y, posRelativa.Z);
        glutSolidCube(1.0);
    glPopMatrix();
}