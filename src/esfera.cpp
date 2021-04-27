#include "esfera.h"

Esfera::Esfera(D3 xyz, GLfloat r, GLfloat s)
{   
    pos = xyz;
    raio = r;
    space = s;

    numVtx = (180 / space) * (2 + 360 / (2 * space)) * 4;
    vtx = new Vertice[numVtx];

    int n;
    double vR, lVR;
    double hR, lHR;
    double norm;
    n = 0;
    for (vR = 0; vR <= 180 - space; vR += space)
    {
        for (hR = 0; hR <= 360 + 2 * space; hR += 2 * space)
        {
            lVR = vR;
            lHR = hR;

            vtx[n].ponto.X = raio * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Y = raio * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Z = raio * cos(lVR / 180 * M_PI);

            vtx[n].V = lVR / 180;
            vtx[n].U = lHR / 360;

            norm = sqrt(
                vtx[n].ponto.X * vtx[n].ponto.X +
                vtx[n].ponto.Y * vtx[n].ponto.Y +
                vtx[n].ponto.Z * vtx[n].ponto.Z);

            vtx[n].normal.X = vtx[n].ponto.X / norm;
            vtx[n].normal.Y = vtx[n].ponto.Y / norm;
            vtx[n].normal.Z = vtx[n].ponto.Z / norm;
            n++;

            lVR = vR + space;
            lHR = hR;
            
            vtx[n].ponto.X = raio * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Y = raio * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Z = raio * cos(lVR / 180 * M_PI);

            vtx[n].V = lVR / 180;
            vtx[n].U = lHR / 360;

            norm = sqrt(vtx[n].ponto.X * vtx[n].ponto.X + vtx[n].ponto.Y * vtx[n].ponto.Y + vtx[n].ponto.Z * vtx[n].ponto.Z);

            vtx[n].normal.X = vtx[n].ponto.X / norm;
            vtx[n].normal.Y = vtx[n].ponto.Y / norm;
            vtx[n].normal.Z = vtx[n].ponto.Z / norm;
            n++;

            lVR = vR;
            lHR = hR + space;

            vtx[n].ponto.X = raio * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Y = raio * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Z = raio * cos(lVR / 180 * M_PI);

            vtx[n].V = lVR / 180;
            vtx[n].U = lHR / 360;

            norm = sqrt(vtx[n].ponto.X * vtx[n].ponto.X + vtx[n].ponto.Y * vtx[n].ponto.Y + vtx[n].ponto.Z * vtx[n].ponto.Z);

            vtx[n].normal.X = vtx[n].ponto.X / norm;
            vtx[n].normal.Y = vtx[n].ponto.Y / norm;
            vtx[n].normal.Z = vtx[n].ponto.Z / norm;
            n++;

            lVR = vR + space;
            lHR = hR + space;

            vtx[n].ponto.X = raio * sin(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Y = raio * cos(lHR / 180 * M_PI) * sin(lVR / 180 * M_PI);
            vtx[n].ponto.Z = raio * cos(lVR / 180 * M_PI);

            vtx[n].V = lVR / 180;
            vtx[n].U = lHR / 360;

            norm = sqrt(vtx[n].ponto.X * vtx[n].ponto.X + vtx[n].ponto.Y * vtx[n].ponto.Y + vtx[n].ponto.Z * vtx[n].ponto.Z);

            vtx[n].normal.X = vtx[n].ponto.X / norm;
            vtx[n].normal.Y = vtx[n].ponto.Y / norm;
            vtx[n].normal.Z = vtx[n].ponto.Z / norm;
            n++;
        }
    }
}