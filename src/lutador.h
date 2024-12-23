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

#define LIM_INF_PHI_1 -45
#define LIM_SUP_PHI_1 45
#define LIM_INF_PHI_2 0
#define LIM_SUP_PHI_2 60

#define VEL_PERNA 5     //MAIOR MAIS LENTA
#define VEL_MOVE 5
#define VEL_GIRO 2
#define VEL_DAR_SOCO 4 / TAM_ARENA_Y
#define VEL_VOLTAR_SOCO 10

#define VEL_BOOT 0.2

#define ALT_CAB_LUT 4

class Lutador
{
    string gNome;

    int TAM_ARENA_X;
    int TAM_ARENA_Y;

    D3 gPos;

    Cor *gCorCabeca;

    Cor *gCorCorpo;

    GLfloat gTheta;

    GLfloat gTheta1_R; // ANGULO DO BRACO DIREITO
    GLfloat gTheta2_R; // ANGULO DO ANTEBRACO DIREITO

    GLfloat gTheta1_L; // ANGULO DO BRACO ESQUERDO
    GLfloat gTheta2_L; // ANGULO DO ANTEBRACO ESQUERDO

    bool gLadoAnda;
    GLfloat gPhi1;      // ANGULO DAS COXAS
    GLfloat gPhi2_R;    // ANGULO DO JOELHO DIREITO
    GLfloat gPhi2_L;    // ANGULO DO JOELHO ESQUERDO

    GLfloat rCabeca;
    GLfloat rNariz;
    GLfloat tamBracos;
    GLfloat tamPernas;
    GLfloat rPe;
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
    void DesenhaPescoco(D3 pos);    
    void DesenhaPerna(D3 pos, bool ehPernaDireita);
    GLfloat calculaAngJoelho();
    bool colisaoLut(GLfloat dXY);
    bool colisaoTelaX(GLfloat dXY);
    bool colisaoTelaY(GLfloat dXY);
    bool colisaoX(GLfloat dXY);
    bool colisaoY(GLfloat dXY);
    void darSocoRL(GLfloat dSoco);
    void voltarSoco(GLfloat dSoco);

    void DesenhaCorpo(D3 pos);

    void DesenhaBraco(D3 pos, GLfloat theta1, GLfloat theta2,
                      GLfloat tamBracos, GLfloat rLuvas);

    void DesenhaNariz(D3 pos);

    void DesenhaCabeca(D3 pos);

    void DesenhaRaioColisao(D3 pos);

    void DesenhaLutador(D3 pos, Cor *cor,
                        GLfloat theta, GLfloat theta1_R, GLfloat theta2_R,
                        GLfloat theta1_L, GLfloat theta2_L, GLfloat rCab,
                        GLfloat tBracos, GLfloat rLvs, GLfloat rClsao, bool isMM);

public:
    Lutador(
        string nome,
        D3 ponto, Cor *_cor,
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

    void Desenha(bool isMM)
    {
        DesenhaLutador(gPos, gCorCabeca, gTheta,
                       gTheta1_R, gTheta2_R,
                       gTheta1_L, gTheta2_L,
                       rCabeca, tamBracos,
                       rLuvas, rColisao, isMM);
    };
    void darSoco();
    void Move(GLfloat dY, GLfloat dTheta);
    void controleSoco(GLfloat dSoco, LadoSoco ladoSoco);
    void getPosLuvaR(D3 &posLuvaR);
    void getPosLuvaL(D3 &posLuvaL);

    void getXYZT(D3 &posLut, GLfloat &dir)
    {
        posLut = gPos;
        dir = gTheta;
    }

    bool getSocoStatus()
    {
        return gSocoStatus;
    }

    bool acerto(bool &acerto_ant);

    void getPosNariz(D3 &posNariz);

    void moveBoot();

    string getNome()
    {
        return gNome;
    }

    Cor *getCor()
    {
        return gCorCabeca;
    }

    bool ehBoot()
    {
        return gEhBoot;
    }

    void setEhBoot(bool ehBoot)
    {
        gEhBoot = ehBoot;
    }

    void getTamBracos(GLfloat &tamB){
        tamB = tamBracos;
    }

    void dirOponente()
    {   
        D3 posOp;
        GLfloat dirOp;
        gOponente->getXYZT(posOp, dirOp);
        gTheta = atan2(gPos.Y - posOp.Y, gPos.X - posOp.X) * fromRad + 90;
    }
};

#endif /* LUTADOR_H */