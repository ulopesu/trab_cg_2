#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "plano.h"
#include "lutador.h"
#include "matrix.h"
#include "tinyxml/tinyxml.h"

#define N_MTX 4

#define INC_KEY 2
#define INC_KEYIDLE 1

// DIMENSOES
// Obs: todos os valores abaixo seram multiplicados por lut1rCabeca
#define ALT_GRADE ALT_CAB_LUT * 3

#define TOTAL_PONTOS_WIN 10

// VELOCIDADE DO SOCO DO BOOT
#define VEL_SOCO_BOOT 20

GLfloat winWidth, winHeight;

// FIM DO JOGO
int FIM = false;

// SVG CONFIG
GLfloat arenaX, arenaY, arenaWidth, arenaHeight;
Cor *arenaCor, *lut1cor, *lut2cor;
GLfloat lut1x, lut1y, lut1rCabeca;
GLfloat lut2x, lut2y, lut2rCabeca;

// MOUSE CONFIG
bool onDrag = false;
GLfloat mouseClick_X = 0;
GLfloat mouseClick_Y = 0;
GLfloat mouseX = 0;
GLfloat mouseY = 0;
int mouseState;
bool ladoMouse;

int keyStatus[256];

// TEMPO DO ACERTO DO ULTIMO SOCO DO BOOT
long long tSocoBoot = 0;

// OUTRAS CONFIG DOS JOGADORES
string nome1("PLAYER 1");
string nome2("PLAYER 2");

Lutador *lutador1;
Lutador *lutador2;
GLfloat dSocoBoot = 0;
LadoSoco lSocoBoot = DIREITA;
bool goBackSoco = false;

bool lut1_acerto_ant = false;
bool lut2_acerto_ant = false;

void *font = GLUT_BITMAP_9_BY_15;

// CAMERA CONFIG
double camDist = 50;
double camXYAngle = 0;
double camXZAngle = 0;
int tipoCam = 0;
int camAngle = 60;

static char str[1000];

long long timeMS(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

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

void imprimePlacar()
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
    D3 pos = {0.7, 0.05, 0};

    imprimeTexto(pos, str, cor);

}

void imprimeVitoria(Lutador *lut)
{
    sprintf(str, "| %s !!! WIN !!! |", lut->getNome().c_str());

    Cor cor = Cor((RGB){1, 1, 1});
    D3 pos = {0.38, 0.93, 0};

    imprimeTexto(pos, str, cor);

    FIM = true;
}

void atualizaLadoMouse()
{
    GLfloat xLut, yLut, dirLut;
    lutador1->getXYT(xLut, yLut, dirLut);
    ladoMouse = (mouseX > mouseClick_X);
}

void drag(int _x, int _y)
{
    if (!FIM)
    {
        mouseX = (GLfloat)_x - (arenaWidth / 2);
        _y = arenaHeight - _y;
        mouseY = (GLfloat)_y - (arenaHeight / 2);
        atualizaLadoMouse();

        if (!mouseState && ladoMouse)
        {
            lutador1->controleSoco(fabs(mouseX - mouseClick_X), DIREITA);
        }
        else if (!mouseState && !ladoMouse)
        {
            lutador1->controleSoco(fabs(mouseX - mouseClick_X), ESQUERDA);
        }
        lutador1->darSoco();

        glutPostRedisplay();
    }
}

void mouse(int button, int state, int _x, int _y)
{
    if (!FIM)
    {
        mouseX = (GLfloat)_x - (arenaWidth / 2);
        _y = arenaHeight - _y;
        mouseY = (GLfloat)_y - (arenaHeight / 2);

        mouseState = state;
        atualizaLadoMouse();
        if (!mouseState && ladoMouse)
        {
            mouseClick_X = mouseX;
        }
        else if (!mouseState && !ladoMouse)
        {
            mouseClick_X = mouseX;
        }
        //printf("\nX: %f.Y: %f.", x, y);
        //printf("\nSTATE: %d.", state);
        glutPostRedisplay();
    }
}

void changeCamera(int angle, int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    gluPerspective(angle, (GLfloat)w / (GLfloat)h, 1, 150.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyPress(unsigned char key, int x, int y)
{
    static bool textureEnebled = true;
    static bool lightingEnebled = true;
    static bool smoothEnebled = true;

    //camXYAngle += x - lastX;
    //camXZAngle += y - lastY;
    //
    //camXYAngle = (int)camXYAngle % 360;
    //camXZAngle = (int)camXZAngle % 360;

    switch (key)
    {
    case '1':
        tipoCam = 1;
        break;
    case '2':
        tipoCam = 2;
        break;
    case '3':
        tipoCam = 3;
        break;
    case '4':
        tipoCam = 4;
        break;
    case 'a':
    case 'A':
        keyStatus[(int)('a')] = 1;
        break;
    case 'd':
    case 'D':
        keyStatus[(int)('d')] = 1;
        break;
    case 'w':
    case 'W':
        keyStatus[(int)('w')] = 1;
        break;
    case 's':
    case 'S':
        keyStatus[(int)('s')] = 1;
        break;
    case ' ':
        if (!FIM)
        {
            lutador2->ehBoot() ? lutador2->setEhBoot(false) : lutador2->setEhBoot(true);
        }
        break;
    case 'r':
    case 'R':
        if (FIM)
        {
            delete lutador1;
            delete lutador2;
            D3 pos1 = {lut1x, lut1y, 0};
            D3 pos2 = {lut2x, lut2y, 0};

            lutador1 = new Lutador(nome1, pos1, lut1cor, 0, lut1rCabeca, arenaWidth, arenaHeight);
            lutador2 = new Lutador(nome2, pos2, lut2cor, 90, lut2rCabeca, arenaWidth, arenaHeight);
            lutador1->setOponente(lutador2);
            lutador2->setOponente(lutador1);
            lutador1->dirOponente();
            lutador2->dirOponente();
            FIM = false;
        }
        break;
    case 'l':
        if (lightingEnebled)
        {
            glDisable(GL_LIGHTING);
        }
        else
        {
            glEnable(GL_LIGHTING);
        }
        lightingEnebled = !lightingEnebled;
        break;
    case '+':
    {
        int inc = camAngle >= 180 ? 0 : 1;
        camAngle += inc;
        changeCamera(camAngle, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    }
    case '-':
    {
        int inc = camAngle <= 5 ? 0 : 1;
        camAngle -= inc;
        changeCamera(camAngle, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    }
    case 27:
        delete arenaCor;
        delete lutador1;
        delete lutador2;
        exit(0);
        break;
    default:
        printf("\nKEY: %c -> INT: %d\n", key, key);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void desenhaArena(){
    Plano chao = Plano({arenaWidth/2, arenaHeight/2, 0}, Z);
    chao.DesenhaComCor(Cor((RGB){0.1,0.1,0.8}));
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    imprimePlacar();

    if (lutador1->getPontos() >= TOTAL_PONTOS_WIN)
    {
        imprimeVitoria(lutador1);
    }
    else if (lutador2->getPontos() >= TOTAL_PONTOS_WIN)
    {
        imprimeVitoria(lutador2);
    }

    if (FIM)
    {
        imprimeReload();
    }

    if (tipoCam == 1)
    {
        D3 posJog1, olharPara;
        lutador1->getXYZ(posJog1);

        GLfloat theta = lutador1->getTheta();
        olharPara.X = sin(theta * toRad);
        olharPara.Y = cos(theta * toRad);
        olharPara.Z = posJog1.Z;

        gluLookAt(posJog1.X, posJog1.Y, 0, 0, 0, 0, 0, 0, 1);
    }
    else if (tipoCam == 2)
    {
        //imprimeTexto({0.1, 0.1, 0}, "Static Camera at a Distance", new Cor(0,1,0));
        D3 posJog1;
        D3 posJog2;
        lutador1->getXYZ(posJog1);
        lutador2->getXYZ(posJog2);
        gluLookAt(posJog1.X, posJog1.Y, posJog1.Z, 0, -arenaHeight / 2, -lut1rCabeca * ALT_CAB_LUT, 0, 0, 1);
        //GLfloat theta = atan2(pos.Y, pos.X) * fromRad + 90;
    }
    else if (tipoCam == 3)
    {
    }
    else
    {
        glTranslatef(0, 0, -lut1rCabeca * ALT_GRADE);
        glRotatef(camXZAngle, 1, 0, 0);
        glRotatef(camXYAngle, 0, 1, 0);
    }

    GLfloat luzDIF[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat luzSPE[] = {1.0, 1.0, 1.0, 1.0};

    // LUZ 0 - CONFIG
    GLfloat luz0POS[] = {0, 0, lut1rCabeca * 7, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, luz0POS);

    // LUZ 1 - CONFIG
    GLfloat luz1POS[] = {-arenaWidth, -arenaHeight, lut1rCabeca * (ALT_GRADE + 2), 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDIF);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzSPE);
    glLightfv(GL_LIGHT1, GL_POSITION, luz1POS);

    // LUZ 2 - CONFIG
    GLfloat luz2POS[] = {-arenaWidth, arenaHeight, lut1rCabeca * (ALT_GRADE + 2), 1.0};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDIF);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luzSPE);
    glLightfv(GL_LIGHT2, GL_POSITION, luz2POS);

    // LUZ 3 - CONFIG
    GLfloat luz3POS[] = {arenaWidth, -arenaHeight, lut1rCabeca * (ALT_GRADE + 2), 1.0};
    glLightfv(GL_LIGHT3, GL_DIFFUSE, luzDIF);
    glLightfv(GL_LIGHT3, GL_SPECULAR, luzSPE);
    glLightfv(GL_LIGHT3, GL_POSITION, luz3POS);

    // LUZ 4 - CONFIG
    GLfloat luz4POS[] = {arenaWidth, arenaHeight, lut1rCabeca * (ALT_GRADE + 2), 1.0};
    glLightfv(GL_LIGHT4, GL_DIFFUSE, luzDIF);
    glLightfv(GL_LIGHT4, GL_SPECULAR, luzSPE);
    glLightfv(GL_LIGHT4, GL_POSITION, luz4POS);

    desenhaArena();
    lutador1->Desenha();
    lutador2->Desenha();

    glutSwapBuffers();
}

void ResetKeyStatus()
{
    int i;
    for (i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void reshape(int w, int h)
{

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    changeCamera(camAngle, w, h);
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);

    ResetKeyStatus();
    glClearColor(arenaCor->getR(), arenaCor->getG(), arenaCor->getB(), 1.0f);

    glMatrixMode(GL_PROJECTION);
    glOrtho(-(arenaWidth / 2), (arenaWidth / 2),   //     X
            -(arenaHeight / 2), (arenaHeight / 2), //     Y
            -10000, 10000);                        //     Z
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lutador1->setOponente(lutador2);
    lutador2->setOponente(lutador1);
    lutador1->dirOponente();
    lutador2->dirOponente();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
}

void idle(void)
{
    if (!FIM)
    {
        double inc = INC_KEYIDLE;
        long long diffTime;

        //MOVIMENTO DO BOOT
        if (lutador2->ehBoot())
        {
            lutador2->moveBoot();

            if (dSocoBoot <= 0)
            {
                if (lSocoBoot == DIREITA)
                {
                    lSocoBoot = ESQUERDA;
                }
                else
                {
                    lSocoBoot = DIREITA;
                }
                goBackSoco = true;
            }
            else if (dSocoBoot > arenaHeight / 2)
            {
                goBackSoco = false;
            }

            if (goBackSoco == true)
            {
                dSocoBoot += VEL_SOCO_BOOT;
            }
            else
            {
                dSocoBoot -= VEL_SOCO_BOOT;
            }

            lutador2->controleSoco(dSocoBoot, lSocoBoot);
        }

        // PONTUACAO DO LUTADOR 1
        if (lutador1->acerto(lut1_acerto_ant) && lutador1->getSocoStatus())
        {
            lutador1->addPontos(1);
        }

        // PONTUACAO DO BOOT
        if (lutador2->acerto(lut2_acerto_ant) && lutador2->getSocoStatus())
        {
            lutador2->addPontos(1);
        }

        // CONTROLE DE TECLAS
        if (keyStatus[(int)('a')])
        {
            lutador1->Move(0, inc);
        }
        if (keyStatus[(int)('d')])
        {
            lutador1->Move(0, -inc);
        }
        if (keyStatus[(int)('w')])
        {
            lutador1->Move(inc, 0);
        }
        if (keyStatus[(int)('s')])
        {
            lutador1->Move(-inc, 0);
        }
        if (mouseState)
        {
            lutador1->controleSoco(1, TODOS);
            lutador1->darSoco();
        }

        if (lutador2->ehBoot())
        {
            lutador2->darSoco();
        }

        glutPostRedisplay();
    }
}

void lerXML(const char *fileName)
{

    TiXmlDocument doc(fileName);

    if (doc.LoadFile())
    {
        printf("\nLeitura do arquivo: %s -> OK!\n", fileName);
    }
    else
    {
        printf("Failed to load file: \"%s\"\n\n", fileName);
        exit(EXIT_FAILURE);
    }

    TiXmlElement *raiz = doc.RootElement();

    char *fill;
    GLfloat aux, dX, dY;

    //  PARSE DA ARENA:
    TiXmlElement *arena = raiz->FirstChildElement("rect");
    arena->QueryFloatAttribute("x", &arenaX);
    arena->QueryFloatAttribute("y", &arenaY);
    arena->QueryFloatAttribute("width", &arenaWidth);
    arena->QueryFloatAttribute("height", &arenaHeight);
    dX = arenaWidth / 2;  //  CORRECAO X
    dY = arenaHeight / 2; //  CORRECAO Y

    fill = strdup(arena->Attribute("fill"));
    arenaCor = new Cor(fill);

    //  PARSE DOS LUTADORES
    //  1
    TiXmlElement *lut1 = arena->NextSiblingElement("circle");
    lut1->QueryFloatAttribute("cx", &aux);
    lut1x = (aux - arenaX) - dX;
    lut1->QueryFloatAttribute("cy", &aux);
    lut1y = (aux - arenaY - dY) * (-1);
    lut1->QueryFloatAttribute("r", &lut1rCabeca);
    fill = strdup(lut1->Attribute("fill"));
    lut1cor = new Cor(fill);

    //  2
    TiXmlElement *lut2 = lut1->NextSiblingElement("circle");

    lut2->QueryFloatAttribute("cx", &aux);
    lut2x = (aux - arenaX) - dX;
    lut2->QueryFloatAttribute("cy", &aux);
    lut2y = (aux - arenaY - dY) * (-1);
    lut2->QueryFloatAttribute("r", &lut2rCabeca);
    fill = strdup(lut2->Attribute("fill"));
    lut2cor = new Cor(fill);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf("\n%s ->  OK!\n", argv[1]);
    }
    else
    {
        printf("\nSem diretorio.\n\n");
        return 1;
    }

    lerXML(argv[1]);

    //printf("\n arenaWidth: %f      arenaHeight: %f\n", arenaWidth, arenaHeight);
    //printf("\n lut1x: %f      lut1y: %f\n", lut1x, lut1y);

    D3 pos1 = {lut1x, lut1y, lut1rCabeca * ALT_CAB_LUT};
    D3 pos2 = {lut2x, lut2y, lut2rCabeca * ALT_CAB_LUT};

    lutador1 = new Lutador(nome1, pos1, lut1cor, 0, lut1rCabeca, arenaWidth, arenaHeight);
    lutador2 = new Lutador(nome2, pos2, lut2cor, 90, lut2rCabeca, arenaWidth, arenaHeight);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

    winWidth = arenaWidth;
    winHeight = arenaHeight;

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(2000, 300);
    glutCreateWindow("Tranformations 2D");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    //glutReshapeFunc (reshape);
    glutKeyboardUpFunc(keyup);
    glutMotionFunc(drag);
    glutMouseFunc(mouse);

    init();
    glutMainLoop();

    return 0;
}
