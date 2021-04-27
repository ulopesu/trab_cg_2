#include "cor.h"

Cor::Cor(RGB rgb)
{   
    vCor = (GLfloat*) malloc(sizeof(GLfloat)*4); 
    vCor[0] = rgb.R;
    vCor[1] = rgb.G;
    vCor[2] = rgb.B;
    vCor[3] = 1;
}

Cor::Cor(string nameCor)
{   
    vCor = (GLfloat*) malloc(sizeof(GLfloat)*4); 
    if (nameCor == "red")
    {
        vCor[0] = 1;
        vCor[1] = 0.2;
        vCor[2] = 0.2;
    }
    else if (nameCor == "green")
    {
        vCor[0] = 0.2;
        vCor[1] = 1;
        vCor[2] = 0.2;
    }
    else if (nameCor == "blue")
    {
        vCor[0] = 0.2;
        vCor[1] = 0.2;
        vCor[2] = 1;
    } else {
        vCor[0] = 1;
        vCor[1] = 1;
        vCor[2] = 1;
    }
    vCor[3] = 1;
}

GLfloat Cor::getR()
{
    return vCor[0];
};
GLfloat Cor::getG()
{
    return vCor[1];
};
GLfloat Cor::getB()
{
    return vCor[2];
};

void Cor::setRGB(RGB rgb)
{
    vCor[0] = rgb.R;
    vCor[1] = rgb.G;
    vCor[2] = rgb.B;
};

GLfloat* Cor::Cor2Vetor(){ 
    return vCor;
}