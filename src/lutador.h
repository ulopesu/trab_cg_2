#ifndef LUTADOR_H
#define LUTADOR_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

#include "cor.h"
#include "matrix.h"

#include "esfera.h"

#include "circulo.h"
#include "retangulo.h"

using namespace std;

enum LadoSoco
{
    DIREITA,
    ESQUERDA,
    TODOS,
};

#define N_MTX 4

#define toRad (M_PI / 180)
#define fromRad (180 / M_PI)

#define LIM_INF_THETA_1 -50
#define LIM_INF_THETA_2 140

#define LIM_SUP_THETA_1 85
#define LIM_SUP_THETA_2 15

#define VEL_MOVE 5
#define VEL_GIRO 2
#define VEL_DAR_SOCO 4 / TAM_ARENA_Y
#define VEL_VOLTAR_SOCO 10

#define VEL_BOOT 0.2

#define ALT_CAB_LUT 6

class Lutador
{
    string gNome;

    int TAM_ARENA_X;
    int TAM_ARENA_Y;

    Coordenada gPos;

    Cor *cor;

    GLfloat gTheta;

    GLfloat gTheta1_R; // ANGULO DO BRACO DIREITO
    GLfloat gTheta2_R; // ANGULO DO ANTEBRACO DIREITO

    GLfloat gTheta1_L; // ANGULO DO BRACO ESQUERDO
    GLfloat gTheta2_L; // ANGULO DO ANTEBRACO ESQUERDO

    GLfloat rCabeca;
    GLfloat rNariz;
    GLfloat tamBracos;
    GLfloat rLuvas;
    GLfloat rColisao;

    bool gSocoStatus;
    LadoSoco gLadoSoco;
    GLfloat gdSoco;

    Lutador *gOponente;
    int gPoss;

    GLfloat gTheta1_R_Ant;
    GLfloat gdiffTheta1_R;

    GLfloat gTheta1_L_Ant;
    GLfloat gdiffTheta1_L;

    bool gEhBoot;

private:
    void getPosLuvaR(GLfloat &xL, GLfloat &yL);
    void getPosLuvaL(GLfloat &xL, GLfloat &yL);
    bool colisaoLut(GLfloat dXY);
    bool colisaoTelaX(GLfloat dXY);
    bool colisaoTelaY(GLfloat dXY);
    bool colisaoX(GLfloat dXY);
    bool colisaoY(GLfloat dXY);
    void darSocoRL(GLfloat dSoco);
    void voltarSoco(GLfloat dSoco);
    void DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2,
                      GLfloat tamBracos, GLfloat rLuvas);

    void DesenhaNariz(GLfloat x, GLfloat y);

    void DesenhaCabeca(GLfloat x, GLfloat y);

    void DesenhaRaioColisao(GLfloat x, GLfloat y);

    void DesenhaLutador(GLfloat x, GLfloat y, Cor *cor,
                        GLfloat theta, GLfloat theta1_R, GLfloat theta2_R,
                        GLfloat theta1_L, GLfloat theta2_L, GLfloat rCab,
                        GLfloat tBracos, GLfloat rLvs, GLfloat rClsao);

public:
    Lutador(
        string nome,
        Coordenada ponto, Cor *_cor,
        GLfloat _theta, GLfloat _tam,
        GLfloat TA_X, GLfloat TA_Y);

    void setOponente(Lutador *Op)
    {
        gOponente = Op;
    }

    void addPontos(int pontos)
    {
        gPoss += pontos;
    }

    int getPontos()
    {
        return gPoss;
    }

    void Desenha()
    {
        DesenhaLutador(gPos.X, gPos.Y, cor, gTheta,
                       gTheta1_R, gTheta2_R,
                       gTheta1_L, gTheta2_L,
                       rCabeca, tamBracos,
                       rLuvas, rColisao);
    };
    void darSoco();
    void Move(GLfloat dY, GLfloat dTheta);
    void controleSoco(GLfloat dSoco, LadoSoco ladoSoco);

    void getXYT(GLfloat &x, GLfloat &y, GLfloat &dir)
    {
        x = gPos.X;
        y = gPos.Y;
        dir = gTheta;
    }

    void getXYZ(Coordenada &ponto)
    {
        ponto.X = gPos.X;
        ponto.Y = gPos.Y;
        ponto.Z = gPos.Z;
    }

    bool getSocoStatus()
    {
        return gSocoStatus;
    }

    bool acerto(bool &acerto_ant);

    void getPosNariz(GLfloat &xL, GLfloat &yL);

    void moveBoot();

    string getNome()
    {
        return gNome;
    }

    Cor *getCor()
    {
        return cor;
    }

    bool ehBoot()
    {
        return gEhBoot;
    }

    void setEhBoot(bool ehBoot)
    {
        gEhBoot = ehBoot;
    }

    void dirOponente()
    {
        GLfloat xOp, yOp, dirOp;
        gOponente->getXYT(xOp, yOp, dirOp);
        gTheta = atan2(gPos.Y - yOp, gPos.X - xOp) * fromRad + 90;
    }
};

#endif /* LUTADOR_H */