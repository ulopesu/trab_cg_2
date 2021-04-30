#ifndef DESENHOS_H
#define DESENHOS_H

#include "plano.h"
#include "lutador.h"
#include "imgLoader/imageloader.h"

// DIMENSOES
// Obs: todos os valores abaixo seram multiplicados por lut1rCabeca
#define ALT_GRADE ALT_CAB_LUT * 2

enum Texturas
{
    Chao,
    Paredes,
    Ceu,
    Fundos,
};

void RasterChars(D3 pos, const char *text, Cor cor);
void imprimeTexto(D3 pos, const char *text, Cor cor);
void imprimePlacar(Lutador *lutador1, Lutador *lutador2);
void imprimeReload();
void imprimeVitoria(Lutador *lut, bool &FIM);
GLuint LoadTextureRAW( const char * filename );
void desenhaArenaMM(GLfloat arenaWidth, GLfloat arenaHeight);
void desenhaArena(GLfloat arenaWidth, GLfloat arenaHeight, GLfloat lut1rCabeca, GLuint *texturas);

#endif /* DESENHOS_H */

