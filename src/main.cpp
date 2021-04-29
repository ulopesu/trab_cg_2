#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "desenhos.h"
#include "matrix.h"
#include "tinyxml/tinyxml.h"

#define N_MTX 4

#define INC_KEY 2
#define INC_KEYIDLE 1

#define TOTAL_PONTOS_WIN 10

// VELOCIDADE DO SOCO DO BOOT
#define VEL_SOCO_BOOT 20

GLfloat winWidth, winHeight;

// ESTADOS DO JOGO
bool FIM = false;
bool modoNoturno = false;

// SVG CONFIG
GLfloat arenaX, arenaY, arenaWidth, arenaHeight;
Cor *arenaCor, *lut1cor, *lut2cor;
GLfloat lut1x, lut1y, lut1rCabeca;
GLfloat lut2x, lut2y, lut2rCabeca;

// TEXTURAS
GLuint texturas[10];

// MOUSE CONFIG
bool onDrag = false;
GLfloat mouseClick_X = 0;
GLfloat mouseClick_Y = 0;
GLfloat mouseX = 0;
GLfloat mouseY = 0;
int mouseButton;
int mouseState;
bool ladoMouseRL;

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
double camDist = 200;
double camXYAngle = 0;
int tipoCam = 1;
int camAngle = 60;
int camUPAngle = 0;

long long timeMS(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void atualizaLadoMouse()
{
    ladoMouseRL = (mouseX > mouseClick_X);
}

void drag(int _x, int _y)
{
    mouseX = (GLfloat)_x - (arenaWidth / 2);
    _y = arenaHeight - _y;
    mouseY = (GLfloat)_y - (arenaHeight / 2);

    atualizaLadoMouse();

    if (!FIM && mouseButton == 0)
    {
        if (!mouseState && ladoMouseRL)
        {
            lutador1->controleSoco(fabs(mouseX - mouseClick_X), DIREITA);
        }
        else if (!mouseState && !ladoMouseRL)
        {
            lutador1->controleSoco(fabs(mouseX - mouseClick_X), ESQUERDA);
        }
        lutador1->darSoco();
        glutPostRedisplay();
    }
    else if (mouseButton == 2 && tipoCam == 3)
    {
        camXYAngle = mouseX - mouseClick_X;



        if (((mouseY - mouseClick_Y) >= -30) && ((mouseY - mouseClick_Y) <= 30))
        {
            camUPAngle = mouseY - mouseClick_Y;
        }
    }
}

void mouse(int button, int state, int _x, int _y)
{
    mouseButton = button;
    mouseX = (GLfloat)_x - (arenaWidth / 2);
    _y = arenaHeight - _y;
    mouseY = (GLfloat)_y - (arenaHeight / 2);

    mouseState = state;

    atualizaLadoMouse();

    if (!mouseState)
    {
        mouseClick_X = mouseX;
        mouseClick_Y = mouseY;
    }
    //printf("\nX: %d. Y: %d.", _x, _y);
    //printf("\nSTATE: %d.", state);
    //printf("\nBUTTON: %d.", button);
    //printf("\n");
    glutPostRedisplay();
}

void changeCamera(int angle, int w, int h)
{

    glMatrixMode(GL_PROJECTION);

    //if(miniMap){
    //glOrtho(-(arenaWidth / 2), (arenaWidth / 2),   //     X
    //        -(arenaHeight / 2), (arenaHeight / 2), //     Y
    //        -10000, 10000);                        //     Z
    //}
    glLoadIdentity();
    gluPerspective(angle, (GLfloat)w / (GLfloat)h, 1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
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
            lutador2->setEhBoot(!lutador2->ehBoot());
        }
        break;
    case 'r':
    case 'R':
        if (FIM)
        {
            delete lutador1;
            delete lutador2;
            D3 pos1 = {lut1x, lut1y, lut1rCabeca * ALT_CAB_LUT};
            D3 pos2 = {lut2x, lut2y, lut2rCabeca * ALT_CAB_LUT};

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
    case 'n':
    {
        modoNoturno = !modoNoturno;
        if (modoNoturno)
        {
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            glDisable(GL_LIGHT4);
            glEnable(GL_LIGHT5);
            glEnable(GL_LIGHT6);
        }
        else
        {
            glDisable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            glEnable(GL_LIGHT2);
            glEnable(GL_LIGHT3);
            glEnable(GL_LIGHT4);
        }
        break;
    }
    case 27:
        delete arenaCor;
        delete lutador1;
        delete lutador2;
        exit(0);
        break;
    default:
        //printf("\nKEY: %c -> INT: %d\n", key, key);
        break;
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void configLuz()
{
    GLfloat dArena = -50;
    GLfloat dLux = 0.3;
    GLfloat corLuz[] = {dLux, dLux, dLux, 1};
    // LUZ 1 - CONFIG
    GLfloat luz0POS[] = {0, 0, lut1rCabeca * (ALT_GRADE - dArena), 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT0, GL_POSITION, luz0POS);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corLuz);

    GLfloat luz1POS[] = {-(arenaWidth / 2) + dArena, -(arenaHeight / 2) + dArena, lut1rCabeca * (ALT_GRADE - dArena), 0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT1, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT1, GL_POSITION, luz1POS);

    // LUZ 2 - CONFIG
    GLfloat luz2POS[] = {-(arenaWidth / 2) + dArena, (arenaHeight / 2) - dArena, lut1rCabeca * (ALT_GRADE - dArena), 0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT2, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT2, GL_POSITION, luz2POS);

    // LUZ 3 - CONFIG
    GLfloat luz3POS[] = {(arenaWidth / 2) - dArena, -(arenaHeight / 2) + dArena, lut1rCabeca * (ALT_GRADE - dArena), 0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT3, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT3, GL_POSITION, luz3POS);

    // LUZ 4 - CONFIG
    GLfloat luz4POS[] = {(arenaWidth / 2) - dArena, (arenaHeight / 2) - dArena, lut1rCabeca * (ALT_GRADE - dArena), 0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT4, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT4, GL_POSITION, luz4POS);

    // CONFIG HOLOFOTES
    D3 posLut1, posLut2;
    lutador1->getXYZ(posLut1);

    GLfloat luz5DIR[] = {luz0POS[0] - posLut1.X, luz0POS[1] - posLut1.Y, luz0POS[2] - posLut1.Z, 1};
    GLfloat normal = sqrt(
        luz5DIR[0] * luz5DIR[0] +
        luz5DIR[1] * luz5DIR[1] +
        luz5DIR[2] * luz5DIR[2]);

    luz5DIR[0] /= normal;
    luz5DIR[1] /= normal;
    luz5DIR[2] /= normal;

    glLightfv(GL_LIGHT0, GL_AMBIENT, corLuz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corLuz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corLuz);
    glLightfv(GL_LIGHT5, GL_POSITION, luz0POS);
    glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 100.0);
    glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);
    glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, luz5DIR);

    lutador2->getXYZ(posLut2);

    GLfloat luz6DIR[] = {luz0POS[0] - posLut1.X, luz0POS[1] - posLut1.Y, luz0POS[2] - posLut1.Z, 1};
    normal = sqrt(
        luz6DIR[0] * luz6DIR[0] +
        luz6DIR[1] * luz6DIR[1] +
        luz6DIR[2] * luz6DIR[2]);

    luz6DIR[0] /= normal;
    luz6DIR[1] /= normal;
    luz6DIR[2] /= normal;

    glLightfv(GL_LIGHT6, GL_POSITION, luz0POS);
    glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 10.0);
    glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0);
    glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, luz6DIR);
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    imprimePlacar(lutador1, lutador2);

    if (lutador1->getPontos() >= TOTAL_PONTOS_WIN)
    {
        imprimeVitoria(lutador1, FIM);
    }
    else if (lutador2->getPontos() >= TOTAL_PONTOS_WIN)
    {
        imprimeVitoria(lutador2, FIM);
    }

    if (FIM)
    {
        imprimeReload();
    }

    if (tipoCam == 1)
    {
        D3 posJog1, olharPara;
        lutador1->getXYZ(posJog1);
        GLfloat dX, dY, theta = lutador1->getTheta();

        dX = -sin(theta * toRad);
        dY = cos(theta * toRad);

        olharPara.X = dX * arenaWidth;
        olharPara.Y = dY * arenaHeight;
        olharPara.Z = posJog1.Z + (lut1rCabeca / 4);

        gluLookAt(
            posJog1.X + dX * (lut1rCabeca / 2),
            posJog1.Y + dY * (lut1rCabeca / 2),
            olharPara.Z,
            olharPara.X,
            olharPara.Y,
            olharPara.Z / 2,
            0, 0, 1);
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
        D3 posJog1;
        lutador1->getXYZ(posJog1);
        GLfloat dX, dY, dZ, meiolutZ, theta = lutador1->getTheta();

        dX = -sin((theta + camXYAngle) * toRad);
        dY = cos((theta + camXYAngle) * toRad);

        dZ = sin(camUPAngle * toRad);

        meiolutZ = ((lut1rCabeca+1) * ALT_CAB_LUT / 2);

        gluLookAt(
            posJog1.X - dX * camDist,
            posJog1.Y - dY * camDist,
            100+(meiolutZ + dZ * camDist),
            posJog1.X,
            posJog1.Y,
            meiolutZ,
            0, 0, 1);
    }
    else if (tipoCam == 4)
    {
    }

    configLuz();

    desenhaArena(arenaWidth, arenaHeight, lut1rCabeca, texturas);

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
    //glDepthFunc(GL_LEQUAL);

    ResetKeyStatus();

    texturas[Chao] = LoadTextureRAW("../texturas/ground1.bmp");
    texturas[Ceu] = LoadTextureRAW("../texturas/ceu.bmp");
    texturas[Paredes] = LoadTextureRAW("../texturas/paredes.bmp");

    D3 pos1 = {lut1x, lut1y, lut1rCabeca * ALT_CAB_LUT};
    D3 pos2 = {lut2x, lut2y, lut2rCabeca * ALT_CAB_LUT};

    lutador1 = new Lutador(nome1, pos1, lut1cor, 0, lut1rCabeca, arenaWidth, arenaHeight);
    lutador2 = new Lutador(nome2, pos2, lut2cor, 90, lut2rCabeca, arenaWidth, arenaHeight);

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

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

    winWidth = arenaWidth;
    winHeight = arenaHeight;

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(2000, 300);
    glutCreateWindow("BOX 3D");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardUpFunc(keyup);
    glutMotionFunc(drag);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
