#include "plano.h"

Plano::Plano(D3 dimensao, posPlano pos)
{
    int i;
    vtx = new Vertice[4];

    switch (pos)
    {
    case X:
        vtx[1].ponto = {dimensao.Z, -dimensao.X, -dimensao.Y};
        vtx[2].ponto = {dimensao.Z, -dimensao.X, +dimensao.Y};
        vtx[3].ponto = {dimensao.Z, +dimensao.X, +dimensao.Y};
        vtx[4].ponto = {dimensao.Z, +dimensao.X, -dimensao.Y};

        for (i = 0; i < 4; i++)
        {
            vtx[i].normal.X = dimensao.Z > 0 ? -1 : 1;
            vtx[i].normal.Y = 0;
            vtx[i].normal.Z = 0;
        }
        break;
    case Y:
        vtx[1].ponto = {-dimensao.X, dimensao.Z, -dimensao.Y};
        vtx[2].ponto = {-dimensao.X, dimensao.Z, +dimensao.Y};
        vtx[3].ponto = {+dimensao.X, dimensao.Z, +dimensao.Y};
        vtx[4].ponto = {+dimensao.X, dimensao.Z, -dimensao.Y};

        for (i = 0; i < 4; i++)
        {
            vtx[i].normal.X = 0;
            vtx[i].normal.Y = dimensao.Z > 0 ? -1 : 1;
            vtx[i].normal.Z = 0;
        }
        break;
    case Z:
        vtx[1].ponto = {-dimensao.X, -dimensao.Y, dimensao.Z};
        vtx[2].ponto = {-dimensao.X, +dimensao.Y, dimensao.Z};
        vtx[3].ponto = {+dimensao.X, +dimensao.Y, dimensao.Z};
        vtx[4].ponto = {+dimensao.X, -dimensao.Y, dimensao.Z};

        for (i = 0; i < 4; i++)
        {
            vtx[i].normal.X = 0;
            vtx[i].normal.Y = 0;
            vtx[i].normal.Z = dimensao.Z > 0 ? -1 : 1;
        }
        break;

    default:
        break;
    }
}
