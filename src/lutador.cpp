#include "lutador.h"

Lutador::Lutador(string nome, Coordenada ponto, Cor *_cor,
                 GLfloat _theta, GLfloat _tam,
                 GLfloat TA_X, GLfloat TA_Y)
{
    gNome = nome;
    gEhBoot = false;
    // PRESET LOCALIZAÇÃO E DIREÇÃO
    TAM_ARENA_X = TA_X;
    TAM_ARENA_Y = TA_Y;

    gTheta = _theta;
    gTheta1_R = LIM_INF_THETA_1;
    gTheta2_R = LIM_INF_THETA_2;

    gTheta1_L = LIM_INF_THETA_1;
    gTheta2_L = LIM_INF_THETA_2;

    GLfloat dir[1][4];

    // PRESET DE TAMANHOS E ESCALAS E CORES
    rCabeca = _tam * 0.5;
    gPonto = ponto;
    cor = _cor;
    rNariz = rCabeca / 5;
    tamBracos = rCabeca * 2;
    rLuvas = rCabeca / 2;
    rColisao = rCabeca * 3;

    // PRESET DE SOCO
    gSocoStatus = false;
    gLadoSoco = DIREITA;
    gTheta1_R_Ant = LIM_INF_THETA_1;
    gTheta1_L_Ant = LIM_INF_THETA_1;
    gdSoco = 0;
    gPontos = 0;
};

void Lutador::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2,
                           GLfloat tamBracos, GLfloat rLuvas)
{
    Cor *cor = new Cor(0.05, 0.05, 0.05);
    glPushMatrix();

    glTranslatef(x, y, 0);
    glRotatef(theta1, 0, 0, 1);

    Retangulo *ret = new Retangulo(tamBracos / 6, tamBracos, 0, 0);
    ret->desenha(cor);

    glTranslatef(0, tamBracos, 0);

    glRotatef(theta2, 0, 0, 1);
    ret->desenha(cor);

    glTranslatef(0, tamBracos, 0);

    Circulo *circ = new Circulo(rLuvas, 20, 0, 0);
    circ->desenhaComBorda(new Cor(1, 0.2, 0.2));

    glPopMatrix();

    free(ret);
    free(circ);
}

void Lutador::DesenhaNariz(GLfloat x, GLfloat y, Cor *_cor, GLfloat rNariz)
{
    Circulo *circ = new Circulo(rNariz, 100, x, y);
    circ->desenhaComBorda(_cor);
    free(circ);
}

void Lutador::DesenhaCabeca(GLfloat x, GLfloat y, Cor *_cor, GLfloat rCabeca)
{   

    Circulo *circ = new Circulo(rCabeca, 100, x, y);
    circ->desenhaComBorda(_cor);
    free(circ);
}

void Lutador::DesenhaRaioColisao(GLfloat x, GLfloat y, Cor *_cor, GLfloat rClsao)
{
    Circulo *circ = new Circulo(rClsao, 100, x, y);
    circ->desenhaPontos(3, _cor);
    free(circ);
}

void Lutador::DesenhaLutador(GLfloat x, GLfloat y, Cor *cor, GLfloat theta,
                             GLfloat theta1_R, GLfloat theta2_R,
                             GLfloat theta1_L, GLfloat theta2_L,
                             GLfloat rCab, GLfloat tBracos,
                             GLfloat rLvs, GLfloat rClsao)
{

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(gTheta, 0, 0, 1);
    //DesenhaRaioColisao(0, 0, new Cor(1, 1, 1), rClsao);
    DesenhaNariz(0, rCabeca + (rNariz / 2), cor, rNariz);
    DesenhaBraco(rCabeca, 0, (-90 + theta1_R), theta2_R, tamBracos, rLuvas); // DIREITA
    DesenhaBraco(-rCabeca, 0, 90 - theta1_L, -theta2_L, tamBracos, rLuvas);  // ESQUERDA
    DesenhaCabeca(0, 0, cor, rCabeca);

    glPopMatrix();
    gdiffTheta1_R = gTheta1_R_Ant - gTheta1_R;
    gdiffTheta1_L = gTheta1_L_Ant - gTheta1_L;
    gTheta1_R_Ant = gTheta1_R;
    gTheta1_L_Ant = gTheta1_L;
}

bool Lutador::colisaoTelaX(GLfloat dXY)
{
    bool colisaoTela;
    GLfloat tam = -dXY * sin(gTheta * toRad);
    if (gPonto.X > 0)
    {
        tam += gPonto.X + rCabeca;
    }
    else
    {
        tam += gPonto.X - rCabeca;
    }
    return abs(tam) > (TAM_ARENA_X / 2) ? true : false;
}

bool Lutador::colisaoTelaY(GLfloat dXY)
{
    GLfloat tam = dXY * cos(gTheta * toRad);
    if (gPonto.Y > 0)
    {
        tam += gPonto.Y + rCabeca;
    }
    else
    {
        tam += gPonto.Y - rCabeca;
    }
    return abs(tam) >= (TAM_ARENA_Y / 2) ? true : false;
}

bool Lutador::colisaoLut(GLfloat dXY)
{
    GLfloat dX = -dXY * sin(gTheta * toRad);
    GLfloat dY = dXY * cos(gTheta * toRad);
    GLfloat xOp, yOp, dirOp;
    gOponente->getXYT(xOp, yOp, dirOp);

    dX = gPonto.X + dX - xOp;
    dY = gPonto.Y + dY - yOp;

    GLfloat dist = sqrt(pow(dX, 2) + pow(dY, 2));

    return dist <= (rColisao + rCabeca) ? true : false;
}

bool Lutador::colisaoX(GLfloat dXY)
{
    return colisaoTelaX(dXY);
}

bool Lutador::colisaoY(GLfloat dXY)
{
    return colisaoTelaY(dXY);
}

void Lutador::Move(GLfloat dXY, GLfloat dTheta)
{
    //GLfloat toRad = M_PI / 180;
    dXY *= VEL_MOVE;
    dTheta *= VEL_GIRO;
    gTheta += dTheta;

    if (dXY != 0)
    {
        if (!colisaoX(dXY) && !colisaoLut(dXY))
        {
            gPonto.X -= dXY * sin(gTheta * toRad);
        }

        if (!colisaoY(dXY) && !colisaoLut(dXY))
        {
            gPonto.Y += dXY * cos(gTheta * toRad);
        }
    }
    else
    {
        if (gTheta > 360 || gTheta < -360)
        {
            gTheta = fmod(gTheta, 360);
        }
    }

    //printf("gPonto.X: %.2f gPonto.Y: %.2f\n", cos(dTheta), sin(dTheta));
}

void Lutador::controleSoco(GLfloat dSoco, LadoSoco ladoSoco)
{
    gdSoco = dSoco;

    gLadoSoco = ladoSoco;
}

void Lutador::darSocoRL(GLfloat dSoco)
{

    GLfloat p1 = ((LIM_SUP_THETA_1 - LIM_INF_THETA_1) * dSoco) + LIM_INF_THETA_1;

    GLfloat p2 = ((LIM_INF_THETA_2 - LIM_SUP_THETA_2) * dSoco);
    p2 *= (-1);
    p2 += LIM_INF_THETA_2;

    if (p1 > LIM_INF_THETA_1 && p1 < LIM_SUP_THETA_1)
    {
        switch (gLadoSoco)
        {
        case ESQUERDA:
            gTheta1_L = p1;
            break;

        case DIREITA:
            gTheta1_R = p1;
            break;
        }
    }

    if (p2 > LIM_SUP_THETA_2 && p2 < LIM_INF_THETA_2)
    {
        switch (gLadoSoco)
        {
        case ESQUERDA:
            gTheta2_L = p2;
            break;

        case DIREITA:
            gTheta2_R = p2;
            break;
        }
    }
}

void Lutador::voltarSoco(GLfloat dSoco)
{
    if ((gTheta1_R - dSoco) > LIM_INF_THETA_1 && (gTheta1_R - dSoco) < LIM_SUP_THETA_1)
    {
        gTheta1_R -= dSoco;
    }

    if ((gTheta1_L - dSoco) > LIM_INF_THETA_1 && (gTheta1_L - dSoco) < LIM_SUP_THETA_1)
    {
        gTheta1_L -= dSoco;
    }

    if ((gTheta2_R + dSoco) > LIM_SUP_THETA_2 && (gTheta2_R + dSoco) < LIM_INF_THETA_2)
    {
        gTheta2_R += dSoco;
    }

    if ((gTheta2_L + dSoco) > LIM_SUP_THETA_2 && (gTheta2_L + dSoco) < LIM_INF_THETA_2)
    {
        gTheta2_L += dSoco;
    }
}

void Lutador::darSoco()
{
    GLfloat theta1, theta2;
    switch (gLadoSoco)
    {
    case TODOS:
        gSocoStatus = false;
        voltarSoco(gdSoco * VEL_VOLTAR_SOCO);
        break;

    default:
        gSocoStatus = true;
        darSocoRL(gdSoco * VEL_DAR_SOCO);
        break;
    }
}

void Lutador::getPosLuvaR(GLfloat &xL, GLfloat &yL)
{
    GLfloat **mtx_lut = identityMatrix(N_MTX);
    GLfloat **aux;

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, gTheta1_R, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos-2, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, gTheta2_R+gTheta1_R+2, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    mtx_lut = translateMatrix(mtx_lut, rCabeca, 0, 0, N_MTX);
    mtx_lut = rotateMatrix(mtx_lut, 0, 0, gTheta, N_MTX);
    mtx_lut = translateMatrix(mtx_lut, gPonto.X, gPonto.Y, 0, N_MTX);

    xL = mtx_lut[0][0];
    yL = mtx_lut[1][0];

    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

void Lutador::getPosLuvaL(GLfloat &xL, GLfloat &yL)
{
    GLfloat **mtx_lut = identityMatrix(N_MTX);
    GLfloat **aux;

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, 180-gTheta1_L, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos-2, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, 180-gTheta1_L-gTheta2_L+1, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    mtx_lut = translateMatrix(mtx_lut, -rCabeca, 0, 0, N_MTX);
    mtx_lut = rotateMatrix(mtx_lut, 0, 0, gTheta, N_MTX);
    mtx_lut = translateMatrix(mtx_lut, gPonto.X, gPonto.Y, 0, N_MTX);

    xL = mtx_lut[0][0];
    yL = mtx_lut[1][0];

    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

void Lutador::getPosNariz(GLfloat &xL, GLfloat &yL)
{
    GLfloat **mtx = translateMatrix(identityMatrix(N_MTX), 0, rCabeca, 0, N_MTX);
    mtx = rotateMatrix(mtx, 0, 0, -gTheta, N_MTX);
    xL = mtx[0][0] + gPonto.X;
    yL = mtx[0][1] + gPonto.Y;

    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

bool Lutador::acerto(bool &acerto_ant)
{
    GLfloat xOp, yOp, dirOp, xLuva, yLuva;
    gOponente->getXYT(xOp, yOp, dirOp);

    // LUVA DIREITA
    getPosLuvaR(xLuva, yLuva);
    GLfloat dtR = dist(xLuva, yLuva, xOp, yOp);

    // LUVA ESQUERDA
    getPosLuvaL(xLuva, yLuva);
    GLfloat dtL = dist(xLuva, yLuva, xOp, yOp);

    //printf("\n gdiffTheta1_R: %f \n gdiffTheta1_L: %f\n", gdiffTheta1_R, gdiffTheta1_L);

    bool bracoRCab = (dtR < rLuvas + rCabeca);
    bool bracoLCab = (dtL < rLuvas + rCabeca);
    bool acertoCab = ((bracoRCab && gdiffTheta1_R < 0) || (bracoLCab && gdiffTheta1_L < 0));

    // NARIZ OPONENTE
    gOponente->getPosNariz(xOp, yOp);

    // LUVA DIREITA NARIZ
    getPosLuvaR(xLuva, yLuva);
    dtR = dist(xLuva, yLuva, xOp, yOp);

    // LUVA ESQUERDA NARIZ
    getPosLuvaL(xLuva, yLuva);
    dtL = dist(xLuva, yLuva, xOp, yOp);

    //printf("\ndT: %f\nRAIOS: %f\n", dt, (rLuvas + rCabeca));

    bool bracoRNariz = (dtR < rLuvas + rNariz);
    bool bracoLNariz = (dtL < rLuvas + rNariz);
    bool acertoNariz = ((bracoRNariz && gdiffTheta1_R < 0) || (bracoLNariz && gdiffTheta1_L < 0));
    bool acerto = (acertoCab || acertoNariz) && !acerto_ant;

    acerto_ant = bracoRCab || bracoLCab || bracoRNariz || bracoLNariz;

    return acerto;
}

void Lutador::moveBoot()
{
    dirOponente();

    Move(VEL_BOOT, 0);
}