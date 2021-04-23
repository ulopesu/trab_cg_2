#include "cor.h"

Cor::Cor(GLfloat r, GLfloat g, GLfloat b)
{   
    vCor = (GLfloat*) malloc(sizeof(GLfloat)*4); 
    R = vCor[0] = r;
    G = vCor[1] = g;
    B = vCor[2] = b;
    vCor[3] = 1;
}

Cor::Cor(string nameCor)
{
    if (nameCor == "red")
    {
        R = 1;
        G = 0;
        B = 0;
    }
    else if (nameCor == "green")
    {
        R = 0;
        G = 1;
        B = 0;
    }
    else if (nameCor == "blue")
    {
        R = 0;
        G = 0;
        B = 1;
    } else {
        R = 1;
        G = 1;
        B = 1;
    }
}

GLfloat Cor::getR()
{
    return R;
};
GLfloat Cor::getG()
{
    return G;
};
GLfloat Cor::getB()
{
    return B;
};

void Cor::setRGB(GLfloat r, GLfloat g, GLfloat b)
{
    R = vCor[0] = r;
    G = vCor[1] = g;
    B = vCor[2] = b;
};

GLfloat* Cor::Cor2Vetor(){ 
    return vCor;
}