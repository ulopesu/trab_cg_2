#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <string>


#include "lutador.h"
#include "matrix.h"
#include "tinyxml/tinyxml.h"


#define INC_KEY 2
#define INC_KEYIDLE 1

#define N_MTX 4

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

void RasterChars(Coordenada pos, const char *text, Cor *cor)
{
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glColor3f(cor->getR(), cor->getG(), cor->getB());
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

void imprimeTexto(Coordenada pos, const char * text, Cor *cor)
{
    glMatrixMode (GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity ();
            glOrtho (0, 1, 0, 1, -1, 1);
            RasterChars(pos, text, cor);    
        glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void imprimePlacar()
{
    int ptsLut1 = lutador1->getPontos();
    int ptsLut2 = lutador2->getPontos();
    sprintf(str, "| Player 1:  %d    |    Player 2:  %d |", ptsLut1, ptsLut2);

    Cor *cor = new Cor(1, 1, 1);
    Coordenada pos = {0.3, 0.97, 0};

    imprimeTexto(pos, str, cor);
    free(cor);
}

void imprimeReload()
{
    sprintf(str, "Press (R) to RELOAD!");

    Cor *cor = new Cor(1, 1, 1);
    Coordenada pos = {0.4, 0.5, 0};

    imprimeTexto(pos, str, cor);

    free(cor);
}

void imprimeVitoria(Lutador *lut)
{
    sprintf(str, "| %s !!! WIN !!! |", lut->getNome().c_str());

    Cor *cor = new Cor(1, 1, 1);
    Coordenada pos = {0.38, 0.93, 0};

    imprimeTexto(pos, str, cor);

    FIM = true;
    free(cor);
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

void keyPress(unsigned char key, int x, int y)
{
    static bool textureEnebled = true;
    static bool lightingEnebled = true;
    static bool smoothEnebled = true;

    switch (key)
    {
    case '1':
        tipoCam = 0;
        break;
    case '2':
        tipoCam = 1;
        break;
    case '3':
        tipoCam = 2;
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
            free(lutador1);
            free(lutador2);
            Coordenada pos1 = {lut1x, lut1y, 0};
            Coordenada pos2 = {lut2x, lut2y, 0};

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
    case 27:
        free(arenaCor);
        free(lutador1->getCor());
        free(lutador2->getCor());
        free(lutador1);
        free(lutador2);
        exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void display(void)
{   
    //glClearColor (0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();

    if (tipoCam == 0){
        //imprimeTexto({0.1, 0.1, 0}, "Static Camera at a Distance", new Cor(0,1,0));

        //gluLookAt(0,0,0, 0,0,0, 0,0,1);
    } else if (tipoCam == 1){
    } else if (tipoCam == 2){
    }




    lutador1->Desenha();
    lutador2->Desenha();
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
    glutSwapBuffers(); // Desenha the new frame of the game.
}

void ResetKeyStatus()
{
    int i;
    for (i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void changeCamera(int angle, int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(angle,
                   (GLfloat)w / (GLfloat)h, 1, 150.0);

    glMatrixMode(GL_MODELVIEW);
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
            -100, 100);                            //     Z
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    lutador1->setOponente(lutador2);
    lutador2->setOponente(lutador1);
    lutador1->dirOponente();
    lutador2->dirOponente();
    glEnable(GL_LIGHT0);
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

    Coordenada pos1 = {lut1x, lut1y, 0};
    Coordenada pos2 = {lut2x, lut2y, 0};

    lutador1 = new Lutador(nome1, pos1, lut1cor, 0, lut1rCabeca, arenaWidth, arenaHeight);
    lutador2 = new Lutador(nome2, pos2, lut2cor, 90, lut2rCabeca, arenaWidth, arenaHeight);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    winWidth = arenaWidth;
    winHeight = arenaHeight;

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(2000, 300);
    glutCreateWindow("Tranformations 2D");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);
    glutMotionFunc(drag);
    glutMouseFunc(mouse);

    init();
    glutMainLoop();

    return 0;
}