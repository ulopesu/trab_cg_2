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
    vCor = (GLfloat*) malloc(sizeof(GLfloat)*4); 
    if (nameCor == "red")
    {
        R = vCor[0] = 1;
        G = vCor[1] = 0.2;
        B = vCor[2] = 0.2;
    }
    else if (nameCor == "green")
    {
        R = vCor[0] = 0.2;
        G = vCor[1] = 1;
        B = vCor[2] = 0.2;
    }
    else if (nameCor == "blue")
    {
        R = vCor[0] = 0.2;
        G = vCor[1] = 0.2;
        B = vCor[2] = 1;
    } else {
        R = vCor[0] = 1;
        G = vCor[1] = 1;
        B = vCor[2] = 1;
    }
    vCor[3] = 1;
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