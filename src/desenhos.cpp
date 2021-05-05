#include <unistd.h>
#include <iostream>
#include <string>

#include "desenhos.h"

static char str[1000];

void RasterChars(D3 pos, const char *text, Cor cor)
{
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor3f(cor.getR(), cor.getG(), cor.getB());
    glRasterPos3f(pos.X, pos.Y, pos.Z);
    const char *tmpStr;
    tmpStr = text;
    while (*tmpStr)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
        tmpStr++;
    }
    glPopAttrib();
}

void imprimeTexto(D3 pos, const char *text, Cor cor)
{
    glMatrixMode(GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity();
            glOrtho(0, 1, 0, 1, -1, 1);
            RasterChars(pos, text, cor);
        glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void imprimePlacar(Lutador *lutador1, Lutador *lutador2)
{
    int ptsLut1 = lutador1->getPontos();
    int ptsLut2 = lutador2->getPontos();
    sprintf(str, "| Player 1:  %d    |    Player 2:  %d |", ptsLut1, ptsLut2);

    Cor cor = Cor((RGB){1, 1, 1});
    D3 pos = {0.3, 0.97, 0};

    imprimeTexto(pos, str, cor);
}

void imprimeReload()
{
    sprintf(str, "Press (R) to RELOAD!");

    Cor cor = Cor((RGB){1, 1, 1});
    D3 pos = {0.1, 0.05, 0};

    imprimeTexto(pos, str, cor);
}

void imprimeVitoria(Lutador *lut, bool &FIM)
{
    sprintf(str, "| %s !!! WIN !!! |", lut->getNome().c_str());

    Cor cor = Cor((RGB){1, 1, 1});
    D3 pos = {0.38, 0.93, 0};

    imprimeTexto(pos, str, cor);

    FIM = true;
}

GLuint LoadTextureRAW(const char *filename)
{

    GLuint texture;

    Image *image = loadBMP(filename);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE );
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,               //Always GL_TEXTURE_2D
                 0,                           //0 for now
                 GL_RGB,                      //Format OpenGL uses for image
                 image->width, image->height, //Width and height
                 0,                           //The border of the image
                 GL_RGB,                      //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE,            //GL_UNSIGNED_BYTE, because pixels are stored
                                              //as unsigned numbers
                 image->pixels);              //The actual pixel data
    delete image;

    return texture;
}

void desenhaArenaMM(GLfloat arenaWidth, GLfloat arenaHeight){

    D3 escala, posCubo, posRelativa;
    posCubo = {-arenaWidth / 2, 0, 0};
    posRelativa = {0, 0, 0};
    escala = {arenaWidth/50, arenaWidth, arenaWidth/50};

    Cor corGrade = Cor((RGB){1, 0, 1});
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);

    posCubo = {0, arenaHeight / 2, 0};
    escala = {arenaWidth, arenaWidth/50, arenaWidth/50};
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);
}

void desenhaArena(GLfloat arenaWidth, GLfloat arenaHeight, GLfloat lut1rCabeca, GLuint *texturas, bool ehModoNoturno)
{
    D3 posInical = {0, 0, 0};
    // DESENHA CHAO
    Plano chao = Plano(posInical, {arenaWidth / 2, arenaHeight / 2, 0}, Z);
    chao.DesenhaComTextura(texturas[Chao]);

    // DESENHA TETO
    Plano teto = Plano(posInical, {arenaWidth / 2, arenaHeight / 2, lut1rCabeca * ALT_GRADE}, Z);
    teto.DesenhaComTextura(texturas[Ceu]);

    // DESENHA PAREDES
    if(ehModoNoturno){
        posInical = {0, 0, lut1rCabeca * ALT_GRADE / 2};
    }
    Plano p1 = Plano(posInical, {arenaWidth / 2, lut1rCabeca * ALT_GRADE / 2, arenaHeight / 2}, Y);
    p1.DesenhaComTextura(texturas[Paredes]);
    Plano p2 = Plano(posInical, {arenaWidth / 2, lut1rCabeca * ALT_GRADE / 2, -arenaHeight / 2}, Y);
    p2.DesenhaComTextura(texturas[Paredes]);
    Plano p3 = Plano(posInical, {arenaHeight / 2, lut1rCabeca * ALT_GRADE / 2, arenaWidth / 2}, X);
    p3.DesenhaComTextura(texturas[Paredes]);
    Plano p4 = Plano(posInical, {arenaHeight / 2, lut1rCabeca * ALT_GRADE / 2, -arenaWidth / 2}, X);
    p4.DesenhaComTextura(texturas[Paredes]);

    // DESENHA OS PILARES
    D3 escala, posCubo, posRelativa;
    posCubo = {-arenaWidth / 2, -arenaHeight / 2, 0};
    posRelativa = {0, 0, 0.5};
    escala = {lut1rCabeca / 2, lut1rCabeca / 2, lut1rCabeca * ALT_GRADE};

    Cor corGrade = Cor((RGB){1, 0, 1});
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);
    posCubo = {-arenaWidth / 2, arenaHeight / 2, 0};
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);
    posCubo = {arenaWidth / 2, arenaHeight / 2, 0};
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);
    posCubo = {arenaWidth / 2, -arenaHeight / 2, 0};
    DesenhaCuboGL(posCubo, posRelativa, escala, corGrade);

    // DESENHA FUNDOS
    if(!ehModoNoturno) {
        posInical = {0, 0, -lut1rCabeca * ALT_GRADE / 2};
        GLfloat dist = 1500;
        Plano f1 = Plano(posInical, {dist, dist, dist}, Y);
        f1.DesenhaComTextura(texturas[Fundos]);
        Plano f2 = Plano(posInical, {dist, dist, -dist}, Y);
        f2.DesenhaComTextura(texturas[Fundos]);
        Plano f3 = Plano(posInical, {dist, dist, dist}, X);
        f3.DesenhaComTextura(texturas[Fundos]);
        Plano f4 = Plano(posInical, {dist, dist, -dist}, X);
        f4.DesenhaComTextura(texturas[Fundos]);
    }
}