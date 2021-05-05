#include "lutador.h"

Lutador::Lutador(string nome, D3 ponto, Cor *_cor,
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

    gLadoAnda = true;
    gPhi1 = LIM_INF_PHI_1;
    gPhi2_R = LIM_INF_PHI_2;
    gPhi2_L = LIM_INF_PHI_2;

    GLfloat dir[1][4];

    // PRESET DE TAMANHOS E ESCALAS E CORES
    rCabeca = _tam * 0.5;
    gPos = ponto;
    gCorCabeca = _cor;
    gCorCorpo = new Cor((RGB){1, 1, 1});
    rNariz = rCabeca / 5;
    tamBracos = rCabeca * 2;
    rLuvas = rCabeca / 2;
    rColisao = rCabeca * 3;
    tamPernas = rCabeca*1.5;
    rPe = rCabeca / 2;

    // PRESET DE SOCO
    gSocoStatus = false;
    gLadoSoco = DIREITA;
    gTheta1_R_Ant = LIM_INF_THETA_1;
    gTheta1_L_Ant = LIM_INF_THETA_1;
    gdSoco = 0;
    gPoss = 0;
};

void Lutador::DesenhaCorpo(D3 pos)
{
    D3 escala = {2 * rCabeca, rCabeca, rCabeca * 3};
    D3 posRelativa = {0, 0, -0.5};

    DesenhaCuboGL(pos, posRelativa, escala, *gCorCorpo);
}

void Lutador::DesenhaBraco(D3 pos,
                           GLfloat theta1, GLfloat theta2,
                           GLfloat tamBracos, GLfloat rLuvas)
{

    D3 escala = {tamBracos / 6, tamBracos, tamBracos / 6};
    D3 posRelativa = {0, 0.5, 0};

    glPushMatrix();
    glTranslatef(pos.X, pos.Y, pos.Z);

    glRotatef(25, 1, 0, 0);

    glRotatef(theta1, 0, 0, 1);

    DesenhaCuboGL({0, 0, 0}, posRelativa, escala, *gCorCorpo);

    glTranslatef(0, tamBracos, 0);
    glRotatef(theta2, 0, 0, 1);

    DesenhaCuboGL({0, 0, 0}, posRelativa, escala, *gCorCorpo);

    glTranslatef(0, tamBracos, 0);

    Esfera luva = Esfera({0, 0, 0}, rLuvas, 10);
    luva.DesenhaComCor(*gCorCabeca);

    glPopMatrix();

    luva.free_obj();
}

void Lutador::DesenhaNariz(D3 pos)
{
    Esfera nariz = Esfera(pos, rNariz, 10);
    nariz.DesenhaComCor(*gCorCabeca);

    nariz.free_obj();
}

void Lutador::DesenhaCabeca(D3 pos)
{

    //Circulo *circ = new Circulo(rCabeca, 100, x, y);
    //circ->desenhaComBorda(cor);
    //free(circ);

    Esfera cabeca = Esfera(pos, rCabeca, 5);
    cabeca.DesenhaComCor(*gCorCabeca);

    cabeca.free_obj();
}

void Lutador::DesenhaRaioColisao(D3 pos)
{
    Circulo circ = Circulo(rColisao, 100, pos.X, pos.Y);
    Cor newCor = Cor((RGB){1, 1, 1});
    circ.desenhaPontos(3, newCor);
}


void Lutador::DesenhaPerna(D3 pos, bool ehPernaDireita){
    D3 escala = {tamPernas / 4, tamPernas / 4, tamPernas};
    D3 posRelativa = {0, 0, -0.5};

    glPushMatrix();
    glTranslatef(pos.X, pos.Y, pos.Z);

    GLfloat phi2;
    if(ehPernaDireita){
        glRotatef(gPhi1, 1, 0, 0);
        phi2 = gPhi2_R;
    } else {
        glRotatef(-gPhi1, 1, 0, 0);
        phi2 = gPhi2_L;
    }
    DesenhaCuboGL({0, 0, 0}, posRelativa, escala, *gCorCorpo);
    glTranslatef(0, 0, -tamPernas);
    glRotatef(-phi2, 1, 0, 0);
    DesenhaCuboGL({0, 0, 0}, posRelativa, escala, *gCorCorpo);
    glTranslatef(0, 0, -tamPernas-rCabeca/4);
    Esfera pe = Esfera({0, 0, 0}, rPe, 5);
    pe.DesenhaComCor(Cor((RGB){0,0,0}));
    glPopMatrix();

    pe.free_obj();
}

void Lutador::DesenhaPescoco(D3 pos)
{
    D3 escala = {rCabeca / 3, rCabeca / 3, rCabeca*2};
    D3 posRelativa = {0, 0, -0.5};

    glPushMatrix();
    glTranslatef(pos.X, pos.Y, pos.Z);
    DesenhaCuboGL({0, 0, 0}, posRelativa, escala, *gCorCorpo);
    glPopMatrix();
}


void Lutador::DesenhaLutador(D3 pos, Cor *cor, GLfloat theta,
                             GLfloat theta1_R, GLfloat theta2_R,
                             GLfloat theta1_L, GLfloat theta2_L,
                             GLfloat rCab, GLfloat tBracos,
                             GLfloat rLvs, GLfloat rClsao,
                             bool isMM)
{

    glPushMatrix();
    glTranslatef(pos.X, pos.Y, pos.Z);
    glRotatef(gTheta, 0, 0, 1);
    //DesenhaRaioColisao(0, 0);
    DesenhaNariz({0, rCabeca + (rNariz / 2)});
    DesenhaBraco({rCabeca, 0, rCabeca * (-2)}, (-85 + theta1_R), theta2_R, tamBracos, rLuvas); // DIREITA
    DesenhaBraco({-rCabeca, 0, rCabeca * (-2)}, 85 - theta1_L, -theta2_L, tamBracos, rLuvas);  // ESQUERDA
    DesenhaCabeca({0, 0, 0});
   
    DesenhaPescoco({0, 0, -rCabeca/2});

    DesenhaCorpo({0, 0, rCabeca * ((GLfloat)-1.5)});
    DesenhaPerna({rCabeca - (tamPernas/4), 0, rCabeca * ((GLfloat)-4.5)}, true);
    DesenhaPerna({-rCabeca + (tamPernas/4), 0, rCabeca * ((GLfloat)-4.5)}, false);
    glPopMatrix();

    if (!isMM)
    {
        gdiffTheta1_R = gTheta1_R_Ant - gTheta1_R;
        gdiffTheta1_L = gTheta1_L_Ant - gTheta1_L;
        gTheta1_R_Ant = gTheta1_R;
        gTheta1_L_Ant = gTheta1_L;
    }
}

bool Lutador::colisaoTelaX(GLfloat dXY)
{
    bool colisaoTela;
    GLfloat tam = -dXY * sin(gTheta * toRad);
    if (gPos.X > 0)
    {
        tam += gPos.X + rCabeca;
    }
    else
    {
        tam += gPos.X - rCabeca;
    }
    return abs(tam) > (TAM_ARENA_X / 2) ? true : false;
}

bool Lutador::colisaoTelaY(GLfloat dXY)
{
    GLfloat tam = dXY * cos(gTheta * toRad);
    if (gPos.Y > 0)
    {
        tam += gPos.Y + rCabeca;
    }
    else
    {
        tam += gPos.Y - rCabeca;
    }
    return abs(tam) >= (TAM_ARENA_Y / 2) ? true : false;
}

bool Lutador::colisaoLut(GLfloat dXY)
{
    GLfloat dX = -dXY * sin(gTheta * toRad);
    GLfloat dY = dXY * cos(gTheta * toRad);
    D3 posOp;
    GLfloat dirOp;
    gOponente->getXYZT(posOp, dirOp);

    dX = gPos.X + dX - posOp.X;
    dY = gPos.Y + dY - posOp.Y;

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

GLfloat Lutador::calculaAngJoelho()
{
    GLfloat aux;
    aux = (gPhi1 + LIM_SUP_PHI_1) * 2;
    return sin(aux * toRad) * LIM_SUP_PHI_2;
}

void Lutador::Move(GLfloat dXY, GLfloat dTheta)
{
    dXY *= VEL_MOVE;
    dTheta *= VEL_GIRO;
    gTheta += dTheta;

    if (dXY != 0)
    {
        if (!colisaoX(dXY) && !colisaoLut(dXY))
        {
            gPos.X -= dXY * sin(gTheta * toRad);
        }

        if (!colisaoY(dXY) && !colisaoLut(dXY))
        {
            gPos.Y += dXY * cos(gTheta * toRad);
        }

        // ANDANDO PRA FRENTE
        bool pFrente = dXY > 0;

        if ((gPhi1 == LIM_INF_PHI_1 && pFrente) || (gPhi1 == LIM_SUP_PHI_1 && !pFrente))
        {
            gLadoAnda = true;
        }
        else if ((gPhi1 == LIM_INF_PHI_1 && !pFrente) || (gPhi1 == LIM_SUP_PHI_1 && pFrente))
        {
            gLadoAnda = false;
        }

        dXY = ((gLadoAnda && pFrente) || (!gLadoAnda && !pFrente)) ? (fabs(dXY)/VEL_PERNA) : -(fabs(dXY)/VEL_PERNA);

        if ((gPhi1 += dXY) > LIM_SUP_PHI_1)
        {
            gPhi1 = LIM_SUP_PHI_1;
        }
        else if ((gPhi1 += dXY) < LIM_INF_PHI_1)
        {
            gPhi1 = LIM_INF_PHI_1;
        }
        else
        {
            gPhi1 += dXY;
        }

        if (gLadoAnda)
        {
            gPhi2_L = 0;
            gPhi2_R = calculaAngJoelho();
        }
        else
        {
            gPhi2_R = 0;
            gPhi2_L = calculaAngJoelho();
        }

        //printf("gPhi1: %.2f\n", gPhi1);
        //printf("gPhi2_R: %.2f gPhi2_L: %.2f\n", gPhi2_R, gPhi2_L);
    }

    if (gTheta > 360 || gTheta < -360)
    {
        gTheta = fmod(gTheta, 360);
    }

    //printf("DXY: %.2f\n", dXY);
    //printf("gPos.X: %.2f gPos.Y: %.2f\n", gPos.X, gPos.Y);
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

void Lutador::getPosLuvaR(D3 &posLuvaR)
{
    GLfloat **mtx_lut = identityMatrix(N_MTX);
    GLfloat **aux;

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, gTheta1_R, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos - 2, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, gTheta2_R + gTheta1_R + 2, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    mtx_lut = translateMatrix(mtx_lut, rCabeca, 0, 0, N_MTX);
    mtx_lut = rotateMatrix(mtx_lut, 0, 0, gTheta, N_MTX);
    mtx_lut = translateMatrix(mtx_lut, gPos.X, gPos.Y, 0, N_MTX);

    posLuvaR.X = mtx_lut[0][0];
    posLuvaR.Y = mtx_lut[1][0];

    GLfloat dZ = (gTheta1_R + 50) / 135 * 2 * tamBracos;
    dZ = sin(30 * toRad) * dZ;
    posLuvaR.Z = gPos.Z - (2 * rCabeca) + dZ;

    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

void Lutador::getPosLuvaL(D3 &posLuvaL)
{
    GLfloat **mtx_lut = identityMatrix(N_MTX);
    GLfloat **aux;

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, 180 - gTheta1_L, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    aux = translateMatrix(identityMatrix(N_MTX), tamBracos - 2, 0, 0, N_MTX);
    aux = rotateZmatrix(aux, 180 - gTheta1_L - gTheta2_L + 1, N_MTX);

    mtx_lut = translateMatrix(mtx_lut, aux[0][0], aux[1][0], aux[2][0], N_MTX);

    mtx_lut = translateMatrix(mtx_lut, -rCabeca, 0, 0, N_MTX);
    mtx_lut = rotateMatrix(mtx_lut, 0, 0, gTheta, N_MTX);
    mtx_lut = translateMatrix(mtx_lut, gPos.X, gPos.Y, 0, N_MTX);

    posLuvaL.X = mtx_lut[0][0];
    posLuvaL.Y = mtx_lut[1][0];

    GLfloat dZ = (gTheta1_L + 50) / 135 * 2 * tamBracos;
    dZ = sin(30 * toRad) * dZ;
    posLuvaL.Z = gPos.Z - (2 * rCabeca) + dZ;
    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

void Lutador::getPosNariz(D3 &posNariz)
{
    GLfloat **mtx = translateMatrix(identityMatrix(N_MTX), 0, rCabeca, 0, N_MTX);
    mtx = rotateMatrix(mtx, 0, 0, -gTheta, N_MTX);
    posNariz.X = mtx[0][0] + gPos.X;
    posNariz.Y = mtx[0][1] + gPos.Y;
    posNariz.Z = gPos.Z;

    //Circulo *circ = new Circulo(rLuvas, 100, xL, yL);
    //circ->desenhaCompleto(new Cor(0,1,1));
}

bool Lutador::acerto(bool &acerto_ant)
{
    D3 posLuva;
    D3 posOp;
    GLfloat dirOp;
    gOponente->getXYZT(posOp, dirOp);

    // LUVA DIREITA
    getPosLuvaR(posLuva);
    posOp.Z = posLuva.Z;
    GLfloat dtR = dist(posLuva, posOp);

    // LUVA ESQUERDA
    getPosLuvaL(posLuva);
    posOp.Z = posLuva.Z;
    GLfloat dtL = dist(posLuva, posOp);

    //printf("\n gdiffTheta1_R: %f \n gdiffTheta1_L: %f\n", gdiffTheta1_R, gdiffTheta1_L);

    bool bracoRCab = (dtR < rLuvas + rCabeca);
    bool bracoLCab = (dtL < rLuvas + rCabeca);
    bool acertoCab = ((bracoRCab && gdiffTheta1_R < 0) || (bracoLCab && gdiffTheta1_L < 0));

    // NARIZ OPONENTE
    gOponente->getPosNariz(posOp);

    // LUVA DIREITA NARIZ
    getPosLuvaR(posLuva);
    posOp.Z = posLuva.Z;
    dtR = dist(posLuva, posOp);

    // LUVA ESQUERDA NARIZ
    getPosLuvaL(posLuva);
    posOp.Z = posLuva.Z;
    dtL = dist(posLuva, posOp);

    //printf("\n dtR: %f\n dtL: %f\n", dtL, dtR);

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