#include "plano.h"

Plano::Plano(D3 dimensao, posPlano posNormal)
{   
    numVtx=4;
    pos = {0,0,0};
    int i;
    vtx = new Vertice[numVtx];

    vtx[0].V = 0;
    vtx[0].U = 0;

    vtx[1].V = 0;
    vtx[1].U = 1;

    vtx[2].V = 1;
    vtx[2].U = 1;

    vtx[3].V = 1;
    vtx[3].U = 0;

    switch (posNormal)
    {
    case X:
        vtx[0].ponto = {dimensao.Z, -dimensao.X, -dimensao.Y};
        vtx[1].ponto = {dimensao.Z, -dimensao.X, +dimensao.Y};
        vtx[2].ponto = {dimensao.Z, +dimensao.X, +dimensao.Y};
        vtx[3].ponto = {dimensao.Z, +dimensao.X, -dimensao.Y};

        for (i = 0; i < numVtx; i++)
        {
            vtx[i].normal.X = dimensao.Z > 0 ? -1 : 1;
            vtx[i].normal.Y = 0;
            vtx[i].normal.Z = 0;
        }
        break;
    case Y:
        vtx[0].ponto = {-dimensao.X, dimensao.Z, -dimensao.Y};
        vtx[1].ponto = {-dimensao.X, dimensao.Z, +dimensao.Y};
        vtx[2].ponto = {+dimensao.X, dimensao.Z, +dimensao.Y};
        vtx[3].ponto = {+dimensao.X, dimensao.Z, -dimensao.Y};

        for (i = 0; i < numVtx; i++)
        {
            vtx[i].normal.X = 0;
            vtx[i].normal.Y = dimensao.Z > 0 ? -1 : 1;
            vtx[i].normal.Z = 0;
        }
        break;
    case Z:
        vtx[0].ponto = {-dimensao.X, -dimensao.Y, dimensao.Z};
        vtx[1].ponto = {-dimensao.X, +dimensao.Y, dimensao.Z};
        vtx[2].ponto = {+dimensao.X, +dimensao.Y, dimensao.Z};
        vtx[3].ponto = {+dimensao.X, -dimensao.Y, dimensao.Z};

        for (i = 0; i < numVtx; i++)
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
